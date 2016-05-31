//
//  Compressione.hpp
//  CompressioneDCT
//
//  Created on 22/05/16.
//

#ifndef Compressione_h
#define Compressione_h

#include "bitmap_plus.hpp"
typedef bitmap_plus::blocco blocco;

#include "DCT.hpp"

int N = 1;
int blockSize = N*8;
int quality = 85;
string sourceImage = "res/cathedral.bmp";
bool debug = false;

/******************************************************
 * 0- Funzioni di Debug
 ******************************************************/

void debugPrint(vector<blocco*> blocchi){
	for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
			cout << endl;
		cout << blocchi[0]->data[i] << " ";
	}
	cout << endl;
}

void debugPrint(double * array){
	for(int i=0;i<blockSize*blockSize;i++){
		if(i%blockSize == 0)
			cout << endl;
		cout << array[i] << " ";
	}
	cout << endl;
}

/******************************************************
 * 1 - Scala l'immagine se non è divisibile per 8N
 ******************************************************/
void scaleImage(bitmap_plus & img){
	if(debug) cout << endl << "scaleImage()" << endl;
	const unsigned int height = img.height();
	const unsigned int width  = img.width();
	
	// L'ordine di queste due operazioni conta!
	// Non spostare!
	if(width % blockSize != 0){ // Completamento colonne
		if(debug) cout << "Larghezza: " << width << " aggiungo " << (blockSize - (width % blockSize)) << "px colonne" << endl;
		int newWidth = width + (blockSize - (width % blockSize));
		img.set_width(newWidth);
		// Clone last bit
		for(int i=0;i<height;i++)
			for(int j=width;j<newWidth;j++)
				img.set_pixel(j,i,width-1,i);
	}
	if(height % blockSize != 0){ // Completamento righe
		if(debug) cout << "Altezza: " << height << " aggiungo " << (blockSize - (height % blockSize)) << "px righe" << endl;
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
double* getSingleBlock(bitmap_plus & img,unsigned int x,unsigned int y){
	bitmap_plus *region = new bitmap_plus;
	img.region(x, y, blockSize, blockSize, *region);
	double* Y_ = region->get_ycbcr();
	return Y_;
}

vector<blocco*> DCT2Apply(bitmap_plus & img){
	if(debug) cout << endl << "DCT2Apply()" << endl;
	vector<blocco*> v_blocchi;
	for(int j=0; j<img.height(); j+=blockSize){							// Per ogni blocco riga (j+=blocksize)
		for(int i=0;i<img.width();i+=blockSize){						// Per ogni blocco colonna (i+=blocksize)
			blocco *b = new blocco;										// Creo un blocco
			b->data = getSingleBlock(img,i,j);
			for(int i=0;i<blockSize*blockSize;i++)						// Scalo di 128
				b->data[i] -= 128;
			b->data = dct2(blockSize,blockSize,b->data);				// Calcolo da DCT
			v_blocchi.push_back(b);										// Salvo il blocco nel vettore
		}
	}
	return v_blocchi;
}

/******************************************************
 * 3 - Quantizzazione
 ******************************************************/

static const unsigned int Q[] = {			// Matrice di quantizzazione
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
	if(debug) cout << endl << "Q * qf = " << endl;
	double qf;
	if(quality >= 50)
		qf = (double)(200-(2*quality))/100;
	else
		qf = (double)(5000/quality)/100;
	long int Q1[64];							// Q1 contiene la matrice Q approssimata con la qualità scelta
	for(int i=0;i<64;i++){
		int ris = round(qf*Q[i]);
		if(ris == 0)							// per evitare divisioni per 0
			ris = 1;
		Q1[i] = ris;							// round(qf*Q)
	}
	
	if(debug){
		for(int i=0;i<64;i++){
			if(i%8 == 0)
				cout << endl;
			cout << Q1[i] << " ";
		}
		cout << endl << endl << "Quantizzazione:" << endl;
	}
	
	long int QN[blockSize][blockSize];
	for(int i=0;i<64;i++){						// Per ogni elemento in Q1
		int rigIni = (i/8)*N;					// * Ci sono cose *
		int rigFin = (((i/8)*N)+N);				// * che tu non   *
		int colIni = (i%8)*N;					// * puoi         *
		int colFin = (((i%8)*N)+N);				// * capire.      *
		for(int j=rigIni;j<rigFin;j++)			// Per ogni blocchettino riga
			for(int k=colIni; k<colFin; k++)	// per ogni blocchettino colonna
				QN[j][k] = Q1[i];				// * Trasformo il vettore in una matrice
	}
	long int QNLin[blockSize*blockSize];				// Ritrasformo la matrice in un vettore
	int pos=0;
	for(int i=0;i<blockSize;i++)
		for(int j=0;j<blockSize;j++)
			QNLin[pos++] = QN[i][j];
	
	//Ora posso eseguire la quantizzazione
	for(int i=0;i<immagine.size();i++){			// per ogni blocco
		for(int j=0;j<blockSize*blockSize;j++){ // Per ogni pixel
			double val = immagine[i]->data[j];
			immagine[i]->data[j] = (double)(round(val/QNLin[j]))*QNLin[j];
		}
	}
}

/******************************************************
 * 4- DCT Inversa
 ******************************************************/

void IDCT2Apply(vector<blocco*> v_blocchi){
	if(debug) cout << endl << "IDCT2Apply()" << endl;
	for(int i=0;i<v_blocchi.size();i++){
		v_blocchi[i]->data = idct2(blockSize,blockSize,v_blocchi[i]->data);		// Calcolo la DCT inversa
		for(int j=0;j<blockSize*blockSize;j++)									// Scalo di 128
			v_blocchi[i]->data[j] += 128;
	}
}

/******************************************************
 * 5- Metti tutto insieme e calcola
 ******************************************************/

void compute(){
	bitmap_plus image(sourceImage);									// 0 - Apro l'immagine da file
	if (!image){
		cout << "Error - Failed to open: " << sourceImage << endl;
		return;
	}
	
	if(debug) debugPrint(getSingleBlock(image,0,0));				// * DEBUG *
	scaleImage(image);												// 1 - Scala l'immagine per portarla alle dimensioni corrette
	image.save_image("before.bmp");									// 1.1 Salvo l'immagine ottenuta dal ridimensionamento
	image.get_ycbcr();												// 1.2 Genero i valori Y,Cb e Cr per l'immagine così creata
	
	vector<blocco*> blocchi = DCT2Apply(image);						// 2 - Divido in blocchi e applico la DCT2
	if(debug) debugPrint(blocchi);									// * DEBUG *
	
	quantizza(blocchi);												// 3 - Applico la quantizzazione ad ogni blocco (comprimo e decomprimo)
	if(debug) debugPrint(blocchi);									// * DEBUG *
	
	IDCT2Apply(blocchi);											// 4 - Applico l'inversa della DCT2
	if(debug) debugPrint(blocchi);									// * DEBUG *
	
	image.import_block(blocchi,blockSize);							// 5 - Reimporto il canale Y così modificato
	image.save_image("after.bmp");									// 5.1 Salvo l'immagine modificata
}

void startCompute(int N_ = 1,
				  int quality_ = 85,
				  string sourceImage_ = "res/cathedral.bmp"){
	N = N_;
	blockSize = 8*N;
	quality = quality_;
	sourceImage = sourceImage_;
	compute();
}

#endif /* Compressione_h */
