#pragma once
#include <cassert>
#include <iostream>

#include "lodepng.h"
#include "color.hpp"

struct image
{
	image() = default;
	image( unsigned h, unsigned w ) :
		width(w), height(h), pixels(w*h)
	{}
	image( const image& img ) = default;
	image( image&& img ) = default;
	~image() = default;

	image& operator = ( const image& ) = default;
	image& operator = ( image&& ) = default;

	color::rgba& operator() ( unsigned i, unsigned j )
	{
		assert(i<height);
		assert(j<width);

		return pixels[i*width+j];
	}

	color::rgba operator()  ( unsigned i, unsigned j ) const
	{
		assert(i<height);
		assert(j<width);

		return pixels[i*width+j];		
	}

	// -------------------------------------------------------------
	unsigned width, height;
	std::vector<color::rgba> pixels;
};
inline image load_image( const char* filename )
{
	image img;
	std::vector<unsigned char> img_data;
	unsigned error = lodepng::decode(img_data, img.width, img.height, filename);
	if(error != 0)
  	{
    	std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    	exit(1);
  	}	
  	img.pixels.resize(img_data.size()/4);
	for ( std::size_t i = 0; i < img.pixels.size(); ++i ) {
		img.pixels[i] = color::rgba{double(img_data[4*i+0])/255., double(img_data[4*i+1])/255.,
							 		double(img_data[4*i+2])/255., double(img_data[4*i+3])/255.};
	}
	return img;
}

inline unsigned save_image( const char* filename, const image& img )
{
	std::vector<unsigned char> img_data(img.pixels.size()*4);
	for ( unsigned i = 0; i < img.pixels.size(); ++i )
	{
		const auto& col = img.pixels[i];
		img_data[4*i+0] = static_cast<unsigned char>(255.*(col[color::rgba::red]  +0.000001));
		img_data[4*i+1] = static_cast<unsigned char>(255.*(col[color::rgba::green]+0.000001));
		img_data[4*i+2] = static_cast<unsigned char>(255.*(col[color::rgba::blue] +0.000001));
		img_data[4*i+3] = static_cast<unsigned char>(255);
	}
	return lodepng::encode(filename, img_data, img.width, img.height);
}
