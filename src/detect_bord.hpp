#pragma once
#include "convolution.hpp"

class detect_bord : public convolution
{
public:
	detect_bord() : 
		convolution( 1, { 0., 1., 0.,
	                      1.,-4., 1.,
	                      0., 1., 0.})
	{}
};
