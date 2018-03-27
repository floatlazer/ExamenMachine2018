#pragma once
#include "convolution.hpp"

class contrast : public convolution
{
public:
	contrast() : convolution( 1, { -1./6.,-2./3., -1./6.,
	                               -2./3.,26./6., -2./3.,
	                               -1./6.,-2./3., -1./6.})
	{}
};