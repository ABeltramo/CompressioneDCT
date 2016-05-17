#include <cstdio>
#include "bitmap_image.hpp"

int main()
{
	bitmap_image image("img/cathedral.bmp");
	
	if (!image)
	{
		printf("Error - Failed to open: input.bmp\n");
		return 1;
	}
	
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	
	unsigned int total_number_of_pixels = 0;
	
	const unsigned int height = image.height();
	const unsigned int width  = image.width();
	
	for (std::size_t y = 0; y < height; ++y)
	{
		for (std::size_t x = 0; x < width; ++x)
		{
			image.get_pixel(x,y,red,green,blue);
			if (red >= 111)
				total_number_of_pixels++;
		}
	}
	
	printf("Number of pixels with red >= 111: %d\n",total_number_of_pixels);
	return 0;
}