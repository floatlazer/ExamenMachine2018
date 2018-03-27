#pragma once
#include "convolution.hpp"

class renforce_bord : public convolution
{
public:
	renforce_bord() : 
		convolution( 1, { 0., 0., 0.,
	                     -1., 1., 0.,
	                      0., 0., 0.})
	{}
};
