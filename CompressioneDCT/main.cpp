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
	
	if(height % blockSize != 0){ // Completamento righe
		cout << "Altezza: " << height << " aggiungo " << height % blockSize << "px righe" << endl;
	}
	if(width % blockSize != 0){ // Completamento colonne
		cout << "Larghezza: " << width << " aggiungo " << width % blockSize << "px colonne" << endl;
		img.set_width(width + width % blockSize);
		/*for(int i=1;i<=width % blockSize;i++){
			for(int j=0;i<height;i++){
				img.set_pixel(j, width+i ,0,0,0);
			}
		}*/
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