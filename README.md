# Compressione DCT  [![Build Status](https://travis-ci.org/ABeltramo/CompressioneDCT.svg?branch=master)](https://travis-ci.org/ABeltramo/CompressioneDCT) [![License: MIT](https://img.shields.io/badge/License-MIT-lightgrey.svg)](https://opensource.org/licenses/MIT)

Implementazione in C++ della compressione JPEG tramite la DCT2
Il progetto è stato compilato su Debian 8 (jessie)
Installazione dipendenze:

	sudo make install-dep
Compilazione del progetto:

	make
Per avviare l'eseguibile, basta fare doppio click o in alternativa:

	CompressioneDCT/compressioneDCT	

Il progetto XCode consente di avere un punto di partenza per OSX.  
Prima di poter compilare però è necessario installare le dipendenze necessarie.  
A tale scopo è semplice utilizzare l'utility port  
[macports.com](https://www.macports.org/)

	sudo port install gtk3 +quartz	
	sudo port install fftw-3 +universal

Una volta terminata l'installazione sarà possibile compilare ed eseguire il progetto.

Non esiste ad oggi una semplice utility per eseguire le stesse operazioni su Windows.
Essendo il codice portabile e le librerie sono state compilate per Windows sicuramente
è possibile compilare il progetto anche su questa piattaforma.
Bisogna però oltre ad installare le librerie, aggiungere a mano i link necessari
per il compilatore C++ in modo da compilare correttamente.

License
-------
	The MIT License (MIT)

	Copyright (c) 2015 Alessandro Beltramo

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
