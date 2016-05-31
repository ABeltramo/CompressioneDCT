//
//  GUI.hpp
//  CompressioneDCT
//
//  Created on 19/05/16.
//

#ifndef GUI_h
#define GUI_h

#include <gtk/gtk.h>
#include "Compressione.hpp"

GtkWindow *window;
GtkBuilder *builder;

double ScaleImg = 1;
void setImages(double scale=1){
	int normalSize = 1000;
	//Img1
	GtkImage *Img1 = (GtkImage*) gtk_builder_get_object (builder, "Img1");
	GdkPixbuf *pixImg1 = gdk_pixbuf_new_from_file_at_scale("before.bmp", normalSize*scale, normalSize*scale, true, NULL);
	gtk_image_set_from_pixbuf(Img1, pixImg1);
	
	//Img2
	GtkImage *Img2 = (GtkImage*) gtk_builder_get_object (builder, "Img2");
	GdkPixbuf *pixImg2 = gdk_pixbuf_new_from_file_at_scale("after.bmp", normalSize*scale, normalSize*scale, true, NULL);
	gtk_image_set_from_pixbuf(Img2,pixImg2);
}

// Quando si preme il bottone di start bisogna:
// 1- Recuperare i valori impostati
// 2- Aprire il file scelto
// 3- Avviare la computazione
void btnStartFunc(GtkWidget *widget, gpointer data){
	GtkAdjustment * scaleQuality = (GtkAdjustment *)gtk_builder_get_object (builder, "adjustment1");
	int quality = gtk_adjustment_get_value(scaleQuality);
	GtkAdjustment * scaleN = (GtkAdjustment *)gtk_builder_get_object (builder, "adjustment2");
	int N = gtk_adjustment_get_value(scaleN);
	GtkFileChooserButton * btnFile = (GtkFileChooserButton *)gtk_builder_get_object (builder, "BtnScegliFile");
	string file;
	if(gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btnFile)) != NULL)
		file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(btnFile));
	else{
		GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
		GtkWidget * dialog = gtk_message_dialog_new (window,
										 flags,
										 GTK_MESSAGE_ERROR,
										 GTK_BUTTONS_NONE,
										 "Seleziona prima un file da aprire");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		return;
	}
	startCompute(N,quality,file);
	ScaleImg = 1;
	setImages();
}

void btnAumentaFunc(){	// Aumenta lo zoom delle immagini
	ScaleImg += 0.25;
	setImages(ScaleImg);
}

void btnRiduciFunc(){	// Riduci lo zoom delle immagini
	ScaleImg -= 0.25;
	setImages(ScaleImg);
}

int start (int   argc, char *argv[]){
	gtk_init (&argc, &argv);
	
	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "res/GUI.glade", NULL);					// Recupero l'impostazione grafica dal file
	
	window = (GtkWindow*) gtk_builder_get_object (builder, "MainWindow");		// Istanzio l'oggetto window
	g_signal_connect (window, "destroy",G_CALLBACK(gtk_main_quit), NULL);		// Associo alla chiusura della finestra il termine del programma
	
	GtkButton * btnStart = (GtkButton *)gtk_builder_get_object (builder, "BtnApplica");
	g_signal_connect (btnStart, "clicked", G_CALLBACK (btnStartFunc), NULL);	// Associo al bottone la funzione btnStartFunc
	
	GtkButton * btnAumenta = (GtkButton *)gtk_builder_get_object (builder, "BtnAumenta");
	g_signal_connect (btnAumenta, "clicked", G_CALLBACK (btnAumentaFunc), NULL);// Associo al bottone la funzione btnAumentaFunc
	
	GtkButton * BtnRiduci = (GtkButton *)gtk_builder_get_object (builder, "BtnRiduci");
	g_signal_connect (BtnRiduci, "clicked", G_CALLBACK (btnRiduciFunc), NULL);	// Associo al bottone la funzione btnRiduciFunc
	
	gtk_main ();																// Avvia il processo grafico
	
	return 0;
}

#endif /* GUI_h */
