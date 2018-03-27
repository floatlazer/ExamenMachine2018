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

	std::size_t nb_frames = number_of_frame( default_animation_root_name );
	std::cerr << "nombre de fichiers a ouvrir : " << nb_frames << std::endl;

	for ( int i = 0; i < nb_frames; ++i )
	{
		char number[4], number2[4];
		sprintf(number,"%d",int(i));
		sprintf(number2,"%03d",int(i));
		std::string filename_in  = default_animation_root_name+"-"+std::string(number)+".png";
		std::string filename_out = default_animation_root_name+"_out."+std::string(number2)+".png";
		image image_in = load_image( filename_in.c_str() );
		image image_out(image_in);
		for ( auto& f : transformations )
			image_out = f->apply_on(image_out);
		save_image(filename_out.c_str(), image_out);
	}

	return 0;
}