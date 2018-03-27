#pragma once
# include <algorithm>
# include <cassert>
# include "image.hpp"

class convolution
{
public:
	convolution( unsigned order, const std::vector<double>& coefs ) :
		m_order(order), m_dim(2*order+1), m_coefs(coefs)
	{
		assert(m_dim*m_dim == coefs.size() );
	}

	virtual image apply_on( const image& img_in ) const {
		image img_out(img_in.height, img_in.width);
		for ( int i_picture = 0; i_picture < img_in.height; i_picture++ )
		{
			for ( int j_picture = 0; j_picture < img_in.width; j_picture++ )
			{
				double c = 0.;
				for ( int i_convolution = -int(m_order); i_convolution <= int(m_order); ++i_convolution )
					for ( int j_convolution = -int(m_order); j_convolution <= int(m_order); ++j_convolution )
				{
					double a = m_coefs[(i_convolution+m_order)*m_dim+j_convolution+m_order];
					int i_in = std::min(unsigned(std::max(i_picture+i_convolution,0)),img_in.height-1);
					int j_in = std::min(unsigned(std::max(j_picture+j_convolution,0)),img_in.width-1 );
					double v = color::hsva{img_in(i_in,j_in)}[color::hsva::value];
					c += a * v;
				}
				c = std::max(std::min(c,1.),0.);
				color::hsva colh{img_in(i_picture , j_picture)};
				colh[color::hsva::value] = c;
				img_out(i_picture,j_picture) = color::rgba(colh);
			}
		}
		return img_out;
	}
private:
	unsigned m_order, m_dim;
	std::vector<double> m_coefs;
};