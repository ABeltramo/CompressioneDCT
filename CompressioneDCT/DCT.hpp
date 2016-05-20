//
//  DCT.hpp
//  CompressioneDCT
//
//  Created by Ale on 18/05/16.
//  Copyright © 2016 ABeltramo. All rights reserved.
//

#ifndef DCT_h
#define DCT_h

#include <fftw3.h>

// La normalizzazione è necessaria in quanto i dati di FFTW3
// Differiscono da quelli di Matlab a meno di costanti
void normalizza(int m, int n, double * array){
	double norm1 = 4 * sqrt(m/2.0) * sqrt(n/2.0);
	double norm2 = sqrt(2);
	for(int i = 0; i < m; i++){			// normalizzazione globale
		for(int j = 0; j < n; j++){
			array[i*n + j] /= norm1;
		}
	}
	for(int i = 0; i < n; i++)			// normalizzazione prima riga
		array[i] /= norm2;
	for(int j = 0; j < (m*n); j+=n)		// normalizzazione prima colonna
		array[j] /= norm2;
}

// Esegue esattamente l'inverso della funzione sopra
void denormalizza(int m, int n, double * array){
	double norm1 = 4 * sqrt(m/2.0) * sqrt(n/2.0);
	double norm2 = sqrt(2);
	for(int i = 0; i < m; i++){			// normalizzazione globale
		for(int j = 0; j < n; j++)
			array[i*n + j] *= norm1;
	}
	for(int i = 0; i < n; i++)			// normalizzazione prima riga
		array[i] *= norm2;
	for(int j = 0; j < (m*n); j+=n)		// normalizzazione prima colonna
		array[j] *= norm2;
}

// DCT2
double * dct2(int height, int width, double* yChannel){
	double * out = (double *) malloc(sizeof(double) * height * width); // Alloco il vettore di output
	
	// do the fftw3 magic
	fftw_plan plan = fftw_plan_r2r_2d(height, width, yChannel, out,
									  FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE); // FFTW_REDFT10
	fftw_execute(plan);
	
	normalizza(height, width, out); // normalizzazione
	return out;
}

// DCT2 inversa
double * idct2(int height, int width, double * yChannel){
	double * out = (double *) malloc(sizeof(double) * height * width); // Alloco il vettore di output
	
	// denormalizzazione
	denormalizza(height, width, yChannel);
	
	// do the fftw3 magic
	fftw_plan plan = fftw_plan_r2r_2d(height, width, yChannel, out,
									  FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE); // FFTW_REDFT01 (01 per l'inversa)
	fftw_execute(plan);
	
	// Scala i risultati di una costante
	double scale = 4.0 * height * width;
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			out[i*width + j] /= scale;
	
	return out;
}


/*
 * TEST della DCT2:
 * Partendo da dati noti stampo i valori ottenuti dalle due iterazioni della DCT
 * Si spera che alla fine ottengo la stessa tabella di input
 */
void testDCT2(){
	double input[] = {231, 32, 233, 161, 24, 71, 140, 245,
		247, 40, 248, 245, 124, 204, 36, 107,
		234, 202, 245, 167,  9, 217, 239, 173,
		193, 190, 100, 167, 43, 180,  8, 70,
		11, 24, 210, 177, 81, 243,  8, 112,
		97, 195, 203, 47, 125, 114, 165, 181,
		193, 70, 174, 167, 41, 30, 127, 245,
		87, 149, 57, 192, 65, 129, 178, 228};
	for(int i=0;i<64;i++){
		if(i%8 == 0)
			cout << endl;
		cout << input[i] << " ";
	}
	cout << endl << endl << "DCT2:" << endl;
	double *in = input;
	double *test = dct2(8,8,in);
	for(int i=0;i<64;i++){
		if(i%8 == 0)
			cout << endl;
		cout << test[i] << " ";
	}
	cout << endl << endl << "Inversa:" << endl;
	double *out = idct2(8,8,test);
	for(int i=0;i<64;i++){
		if(i%8 == 0)
			cout << endl;
		cout << out[i] << " ";
	}
}

#endif /* DCT_h */
