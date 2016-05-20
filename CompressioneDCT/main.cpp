#include <iostream>
#include <vector>
using namespace std;

#include "bitmap_plus.hpp"
typedef bitmap_plus::blocco blocco;

#include "DCT.hpp"

#include "GUI.hpp"

int N = 2;
int blockSize = N*8;
int quality = 90;
string sourceImage = "res/big_tree.bmp";

/******************************************************
 * 1 - Scala l'immagine se non è divisibile per 8N
 ******************************************************/
void scaleImage(bitmap_plus & img){
	const unsigned int height = img.height();
	const unsigned int width  = img.width();

	// L'ordine di queste due operazioni conta!
	// Non spostare!
	if(width % blockSize != 0){ // Completamento colonne
		//cout << "Larghezza: " << width << " aggiungo " << (blockSize - (width % blockSize)) << "px colonne" << endl;
		int newWidth = width + (blockSize - (width % blockSize));
		img.set_width(newWidth);
		// Clone last bit
		for(int i=0;i<height;i++)
			for(int j=width;j<newWidth;j++)
				img.set_pixel(j,i,width-1,i);
	}
	if(height % blockSize != 0){ // Completamento righe
		//cout << "Altezza: " << height << " aggiungo " << (blockSize - (height % blockSize)) << "px righe" << endl;
		int newHeight = height + (blockSize - (height % blockSize));
		img.set_height(newHeight);
		// Clone last bit
		for(int i=height;i<newHeight;i++)
			for(int j=0;j<width;j++)
				img.set_pixel(j,i,j,height-1);
	}
}

/******************************************************
 * 2 - Applica la DCT ad ogni blocco
 ******************************************************/
bitmap_plus* getSingleBlock(bitmap_plus & img,unsigned int x,unsigned int y){
	bitmap_plus *region = new bitmap_plus;
	img.region(x, y, blockSize, blockSize, *region);
	return region;
}

vector<blocco*> DCT2Apply(bitmap_plus & img){
	vector<blocco*> v_blocchi;
	for(int j=0; j<img.height(); j+=blockSize){
		for(int i=0;i<img.width();i+=blockSize){
			//Salvo il blocco
			blocco *b = new blocco;
			b->region = getSingleBlock(img,i,j); // Recupero i valori per il blocco 8N
			b->data = dct2(blockSize,blockSize,b->region->get_ycbcr()); // Calcolo da DCT
			v_blocchi.push_back(b);
		}
	}
	return v_blocchi;
}

/******************************************************
 * 3 - Quantizzazione
 ******************************************************/

static const unsigned int Q[] = {
	16,  11,  10,  16,  24,  40,  51,  61,
	12,  12,  14,  19,  26,  58,  60,  55,
	14,  13,  16,  24,  40,  57,  69,  56,
	14,  17,  22,  29,  51,  87,  80,  62,
	18,  22,  37,  56,  68, 109, 103,  77,
	24,  35,  55,  64,  81, 104, 113,  92,
	49,  64,  78,  87, 103, 121, 120, 101,
	72,  92,  95,  98, 112, 100, 103,  99
};

void quantizza(vector<blocco*> immagine){
	double qf;
	int Q1[64];
	if(quality >= 50)
		qf = (double)(200-(2*quality))/100;
	else
		qf = (double)(5000/quality)/100;
	// round(qf*Q)
	for(int i=0;i<64;i++){
		Q1[i] = round(qf*Q[i]);
	}
	cout << endl << endl;
	int QN[blockSize][blockSize];
	for(int i=0;i<64;i++){			// Per ogni elemento in Q1
		int rigIni = (i/8)*N;
		int rigFin = (((i/8)*N)+N);
		int colIni = (i%8)*N;
		int colFin = (((i%8)*N)+N);
		for(int j=rigIni;j<rigFin;j++)		// Blocchettino riga
			for(int k=colIni; k<colFin; k++)	// Blocchettino colonna
				QN[j][k] = Q1[i];
	}
	int QNLin[blockSize*blockSize];
	int pos=0;
	for(int i=0;i<blockSize;i++)
		for(int j=0;j<blockSize;j++)
			QNLin[pos++] = QN[i][j];
	
	//Quantizzo ogni blocco
	for(int i=0;i<immagine.size();i++){ // per ogni blocco
		for(int j=0;j<blockSize*blockSize;j++){ // Per ogni pixel
			double val = immagine[i]->data[j];
			immagine[i]->data[j] = (double)(1.0*round(val/QNLin[j]))*QNLin[j];
		}
	}
}

/******************************************************
 * 4- DCT Inversa
 ******************************************************/

void IDCT2Apply(vector<blocco*> v_blocchi){
	for(int i=0;i<v_blocchi.size();i++){
		v_blocchi[i]->data = idct2(blockSize,blockSize,v_blocchi[i]->data);		// Calcolo la DCT inversa
	}
}

void compute(){
	bitmap_plus image(sourceImage);
	
	if (!image){
		cout << "Error - Failed to open: input.bmp\n";
		return;
	}
	/*//DEBUG
	 double* y = getSingleBlock(image,0,0)->get_ycbcr();					// Recupero i valori per il canale Y
	 for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
	 cout << endl;
		cout << y[i] << " ";
	 }
	 cout << endl;
	 */
	scaleImage(image);
	image.save_image("before.bmp");
	image.get_ycbcr();
	vector<blocco*> blocchi = DCT2Apply(image);
	/*//DEBUG
	 for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
	 cout << endl;
		cout << blocchi[0]->data[i] << " ";
	 }
	 */
	quantizza(blocchi);
	/*//DEBUG
	 for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
	 cout << endl;
		cout << blocchi[0]->data[i] << " ";
	 }
	 */
	
	IDCT2Apply(blocchi);
	/*//DEBUG
	 for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
	 cout << endl;
		cout << blocchi[0]->data[i] << " ";
	 }
	 */
	image.import_block(blocchi,blockSize);
	image.save_image("after.bmp");
}

/******************************************************
 * Main
 ******************************************************/
int main(int argc, char *argv[]){
	return start(argc,argv);
}