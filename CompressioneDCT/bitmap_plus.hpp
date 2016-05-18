//
//  bitmap_plus.hpp
//  CompressioneDCT
//
//  Created by Ale on 17/05/16.
//  Copyright © 2016 ABeltramo. All rights reserved.
//

#ifndef bitmap_plus_h
#define bitmap_plus_h

#include "bitmap_image.hpp"

class bitmap_plus : public bitmap_image{
public:
	bitmap_plus(const std::string& filename): bitmap_image(filename){}
	
	//Aggiunge altezza all'immagine
	inline void set_height(const unsigned int height){
		bitmap_plus clone(*this);
		setwidth_height(width_,height);
		clear();
		data_ = clone.data_;
	}
	
	//Aggiunge larghezza all'immagine
	inline void set_width(const unsigned int width){
		int newSize = width - width_;
		bitmap_plus clone(*this);
		setwidth_height(width,height_);
		clear();
		copy_from_reverse(clone,newSize,0);
	}
	
	// Lascia spazio in fondo a destra invece che all'inizio
	inline bool copy_from_reverse(const bitmap_plus& source_image,
						  const unsigned int& x_offset,
						  const unsigned int& y_offset)
	{
		for (unsigned int y = 0; y < source_image.height_; ++y)
		{
			unsigned char* itr1           = row(y);
			const unsigned char* itr2     = source_image.row(y);
			const unsigned char* itr2_end = itr2 + source_image.width_ * bytes_per_pixel_;
			
			std::copy(itr2,itr2_end,itr1);
		}
		
		return true;
	}
	
	// Copia l'informazione di un pixel in un altra locazione
	inline void set_pixel(const unsigned int x, const unsigned int y,
						  const unsigned int x_source, const unsigned int y_source)
	{
		const unsigned int y_s_offset = y_source * row_increment_;
		const unsigned int x_s_offset = x_source * bytes_per_pixel_;
		bitmap_image::set_pixel(x,y,
				  data_[y_s_offset + x_s_offset + 0],
				  data_[y_s_offset + x_s_offset + 1],
				  data_[y_s_offset + x_s_offset + 2]);
	}
};

#endif /* bitmap_plus_h */
