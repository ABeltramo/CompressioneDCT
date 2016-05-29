//
//  bitmap_plus.hpp
//  CompressioneDCT
//
//  Created on 17/05/16.
//

#ifndef bitmap_plus_h
#define bitmap_plus_h

#include "bitmap_image.hpp"
#include <vector>
using namespace std;

// Ridefinisco la libreria trovata online per aggiungere dei metodi che mancavano
// Questi metodi semplificano molto la lettura del codice nel main
class bitmap_plus : public bitmap_image{
public:
	struct blocco{		// Rappresenta il singolo blocco su cui opera la DCT2
		double * data;	// Contiene solamente un vettore di dati
	};
	
	/******************************************************
	 * Costruttori
	 ******************************************************/
	bitmap_plus(const std::string& filename): bitmap_image(filename){}
	bitmap_plus(): bitmap_image(){}
	
	/******************************************************
	 * 1 - Funzioni per modificare la dimensione di un BMP
	 ******************************************************/
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
	// Usata per ricopiare le ultime righe/colonne quando allargo l'immagine
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
	
	/******************************************************
	 * 2 - Funzione per manipolare il canale Y (CbCr)
	 ******************************************************/
	double* _cb;				// Le mantengo salvate
	double* _cr;				// Dovrò aggiungerle al termine della modifica
	inline double* get_ycbcr(){ //Convert an RGB value to YCbCr and return only Y value
		double* y  = new double [this->pixel_count()];
		_cb = new double [this->pixel_count()];
		_cr = new double [this->pixel_count()];
		export_ycbcr(y,_cb,_cr);
		return y;
	}
	
	/******************************************************
	 * 3 - Funzione per ricostruire un immagine partendo da
	 *	   un vettore di blocchetti
	 ******************************************************/
	inline void import_block(vector<blocco*> blocchi, int blockSize){
		double *y = new double[this->pixel_count()];
		this->clear();										// Per sicurezza: rimuovo tutto il contenuto dall'immagine
		int count = 0;										// Contatore di dove mi trovo nel vettore
		int lineBlock = width_/blockSize;					// Quanti blocchi ci sono in una riga
		for(int h=0; h<height_; h++){						// Scandisco l'altezza
			for(int w=0;w<width_;w++){						// Scandisco la larghezza
				//				N° blocco	 + (H			  * Blocchi su una riga)
				int region = (w / blockSize) + ((h/blockSize) * lineBlock) ;
				int pixel = ((h*blockSize) + (w % blockSize)) %(blockSize*blockSize);
				y[count++] = blocchi[region]->data[pixel];	// Stendo i pixel in un vettore rispettando il posizionamento
			}
		}
		import_ycbcr(y, _cb, _cr);							// Finalmente importo i Cb e Cr vecchi ed aggiungo il nuovo Y
	}
};

#endif /* bitmap_plus_h */
