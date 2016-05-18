//
//  DCT.hpp
//  CompressioneDCT
//
//  Created by Ale on 18/05/16.
//  Copyright © 2016 ABeltramo. All rights reserved.
//

#ifndef DCT_h
#define DCT_h

#include <complex.h>
#include <fftw3.h>
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void normalizza(int m, int n, double * array)
{
	double norm1 = 4 * sqrt(m/2.0) * sqrt(n/2.0);
	double norm2 = sqrt(2);
	
	// normalizzazione globale
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++){
			array[i*n + j] /= norm1;
			//array[i*n + j] /= norm2;
		}
	}
	
	// normalizzazione prima riga
	for(int i = 0; i < n; i++)
		array[i] /= norm2;
	
	// normalizzazione prima colonna
	for(int j = 0; j < (m*n); j+=n)
		array[j] /= norm2;
}

void denormalizza(int m, int n, double * array){
	double norm1 = 4 * sqrt(m/2.0) * sqrt(n/2.0);
	double norm2 = sqrt(2);
	
	// normalizzazione globale
	for(int i = 0; i < m; i++){
		for(int j = 0; j < n; j++)
			array[i*n + j] *= norm1;
	}
	
	// normalizzazione prima riga
	for(int i = 0; i < n; i++)
		array[i] *= norm2;
	
	// normalizzazione prima colonna
	for(int j = 0; j < (m*n); j+=n)
		array[j] *= norm2;
}

double * dct2(int height, int width, double* yChannel){
	// setup input e output
	double * out = (double *) malloc(sizeof(double) * height * width);
	
	// do the fftw3 magic
	fftw_plan plan = fftw_plan_r2r_2d(height, width, yChannel, out, FFTW_REDFT10,
									  FFTW_REDFT10, FFTW_ESTIMATE);
	fftw_execute(plan);
	
	// normalizzazione
	normalizza(height, width, out);
	return out;
}

void testDCT2(){
	double input[] = {231, 32, 233, 161, 24, 71, 140, 245,
		247, 40, 248, 245, 124, 204, 36, 107,
		234, 202, 245, 167,  9, 217, 239, 173,
		193, 190, 100, 167, 43, 180,  8, 70,
		11, 24, 210, 177, 81, 243,  8, 112,
		97, 195, 203, 47, 125, 114, 165, 181,
		193, 70, 174, 167, 41, 30, 127, 245,
		87, 149, 57, 192, 65, 129, 178, 228};
	double *in = input;
	double *test = dct2(8,8,in);
	for(int i=0;i<64;i++){
		if(i%8 == 0)
			cout << endl;
		cout << test[i] << " ";
	}
}

#endif /* DCT_h */
