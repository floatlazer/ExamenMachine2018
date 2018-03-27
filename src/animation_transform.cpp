#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include "lodepng.h"
#include "contrast.hpp"
#include "flou.hpp"
#include "renforce_bord.hpp"
#include "detect_bord.hpp"
#include "repoussage.hpp"
#include "gauss3.hpp"
#include <chrono>
#include <omp.h>
#include <mpi.h>

std::size_t number_of_frame( const std::string& root_name )
{
	std::size_t nb_files = 0;
	bool can_open = true;
	while (can_open)
	{
		char number[4];
		sprintf(number,"%d",int(nb_files));
		std::string filename = root_name+"-"+std::string(number)+".png";
		std::ifstream file(filename.c_str());
		can_open = bool(file);
		nb_files += can_open;
		file.close();
	}
	return nb_files;
}

std::vector<std::unique_ptr<convolution>> set_transformations( int nargs, char* argv[] )
{
	std::vector<std::unique_ptr<convolution>> transformations;
	if ( nargs > 2 )
	{
		transformations.reserve(nargs-2);
		for ( int i = 2; i < nargs; ++i )
		{
			if ( std::string(argv[i]) == "renforce_bord") {
				std::cout << "Rajout renforcement des bords" << std::endl;
				transformations.emplace_back(new renforce_bord);
			}
			if ( std::string(argv[i]) == "detect_bord") {
				std::cout << "Rajout detection des bords" << std::endl;
				transformations.emplace_back(new detect_bord);
			}
			if ( std::string(argv[i]) == "flou" ) {
				std::cout << "Rajout floutage" << std::endl;
				transformations.emplace_back(new flou);
			}
			if ( std::string(argv[i]) == "contrast" ) {
				std::cout << "Rajout contraste" << std::endl;
				transformations.emplace_back(new contrast);
			}
			if ( std::string(argv[i]) == "repoussage" ) {
				std::cout << "Rajout repoussage" << std::endl;
				transformations.emplace_back(new repoussage);
			}
			if ( std::string(argv[i]) == "gauss3" ) {
				std::cout << "Rajout gauss ordre 3" << std::endl;
				transformations.emplace_back(new gauss3);
			}

		}
	}
	else
	{
		transformations.reserve(2);
		transformations.emplace_back(new contrast);
		transformations.emplace_back(new detect_bord);
	}
	return transformations;
}


int main( int nargs, char* argv[] )
{
	std::string default_animation_root_name = "data/simpson/homer-simpson-animated-gif-7";
	if (nargs > 1) default_animation_root_name = std::string(argv[1]);

	auto transformations = set_transformations( nargs, argv );

    // Init MPI begin
    MPI_Init( &nargs, &argv );
    MPI_Comm globComm;
    MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
    int nbp;
    MPI_Comm_size(globComm, &nbp);
    int rank;
    MPI_Comm_rank(globComm, &rank);

    // Master
    if(rank == 0) 
    {
		std::size_t nb_frames = number_of_frame( default_animation_root_name );
		std::cerr << "nombre de fichiers a ouvrir : " << nb_frames << std::endl;

    	int nb_frames_sent = 0;
    	int nb_frames_recv = 0;
    	int finishSignal = -1;
        MPI_Status currentStatus;
        // Send first tasks
        for(int rk = 0; rk < nbp-1; rk++)
        {
        	if(rk < nb_frames)
 			{
           		MPI_Send(&rk, 1, MPI_INT, rk+1, 0, globComm); // send first tasks
           		std::cout << "Send task row "<< nb_frames_sent << " to slave " << rk+1 << std::endl;
        	    nb_frames_sent++;
        	}
        	else
            {
                MPI_Send(&finishSignal, 1, MPI_INT, rk+1, 0, globComm); // kill the rest of process
            }
        }
        // Receive finish signal and send new frame if any
        while(nb_frames_recv < nb_frames)
        {
        	int finished;
            MPI_Recv(&finished, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, globComm, &currentStatus);
            nb_frames_recv++;
            //int currentRowNum = currentStatus.MPI_TAG;
            int slave_rk = currentStatus.MPI_SOURCE;
            if(nb_frames_sent < nb_frames)
            {
                MPI_Send(&nb_frames_sent, 1, MPI_INT, slave_rk, 0, globComm); // send next line
                std::cout << "Send task frame "<< nb_frames_sent << " to slave " << slave_rk << std::endl;
                nb_frames_sent++;
            }
            else
            {
                MPI_Send(&finishSignal, 1, MPI_INT, slave_rk, 0, globComm); // kill the process
            }
        }
    }
    else // Slave
    {
    	int frame_recv = 0;
        while(frame_recv != -1)
        {
            MPI_Recv(&frame_recv, 1, MPI_INT, 0, 0, globComm, NULL);
            if(frame_recv != -1)
            {
                char number[4], number2[4];
				sprintf(number,"%d",int(frame_recv));
				sprintf(number2,"%03d",int(frame_recv));
				std::string filename_in  = default_animation_root_name+"-"+std::string(number)+".png";
				std::string filename_out = default_animation_root_name+"_out."+std::string(number2)+".png";
				image image_in = load_image( filename_in.c_str() );
				image image_out(image_in);
				for ( auto& f : transformations )
					image_out = f->apply_on(image_out);
				save_image(filename_out.c_str(), image_out);
				int finished = 0;
                MPI_Send(&finished, 1, MPI_INT, 0, frame_recv, globComm); // send finish signal to master
            }  
        }
    }
    MPI_Finalize();

	return 0;
}