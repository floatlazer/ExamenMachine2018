#pragma once
#include "convolution.hpp"

class repoussage : public convolution
{
public:
	repoussage() : 
		convolution( 1, {-2.,-1., 0.,
	                     -1., 1., 1.,
	                      0., 1., 2.})
	{}
};
