#pragma once
#include "convolution.hpp"

class flou : public convolution
{
public:
	flou() : convolution( 1, { 1./9., 1./9., 1./9.,
	                           1./9., 1./9., 1./9.,
	                           1./9., 1./9., 1./9.})
	{}
};