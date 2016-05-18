#include <iostream>
using namespace std;

#include "bitmap_plus.hpp"

int N = 2;
int blockSize = N*8;
int quality = 80;
string sourceImage = "img/cathedral.bmp";

void scaleImage(bitmap_plus & img){
	const unsigned int height = img.height();
	const unsigned int width  = img.width();

	// L'ordine di queste due operazioni conta!
	// Non spostare!
	if(width % blockSize != 0){ // Completamento colonne
		cout << "Larghezza: " << width << " aggiungo " << (blockSize - (width % blockSize)) << "px colonne" << endl;
		int newWidth = width + (blockSize - (width % blockSize));
		img.set_width(newWidth);
		// Clone last bit
		for(int i=0;i<height;i++)
			for(int j=width;j<newWidth;j++)
				img.set_pixel(j,i,width-1,i);
	}
	if(height % blockSize != 0){ // Completamento righe
		cout << "Altezza: " << height << " aggiungo " << (blockSize - (height % blockSize)) << "px righe" << endl;
		int newHeight = height + (blockSize - (height % blockSize));
		img.set_height(newHeight);
		// Clone last bit
		for(int i=height;i<newHeight;i++)
			for(int j=0;j<width;j++)
				img.set_pixel(j,i,j,height-1);
	}
	else
		cout << "L'immagine è già un multiplo di " << blockSize << endl;
}


int main()
{
	bitmap_plus image(sourceImage);
	
	if (!image)
	{
		cout << "Error - Failed to open: input.bmp\n";
		return 1;
	}
	
	scaleImage(image);
	
	image.save_image("result.bmp");
	
	return 0;
}