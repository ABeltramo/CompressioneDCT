//
//  GUI.hpp
//  CompressioneDCT
//
//  Created by Ale on 19/05/16.
//  Copyright © 2016 ABeltramo. All rights reserved.
//

#ifndef GUI_h
#define GUI_h

#include <gtk/gtk.h>

GtkBuilder * builder;

void setImages(){
	//Img1
	GtkImage *Img1 = (GtkImage*) gtk_builder_get_object (builder, "Img1");
	GdkPixbuf *pixImg1 = gdk_pixbuf_new_from_file("res/cathedral.bmp", NULL);
	gtk_image_set_from_pixbuf(Img1, pixImg1);
	
	//Img2
	GtkImage *Img2 = (GtkImage*) gtk_builder_get_object (builder, "Img2");
	GdkPixbuf *pixImg2 = gdk_pixbuf_new_from_file("res/cathedral.bmp", NULL);
	gtk_image_set_from_pixbuf(Img2,pixImg2);
}

int start (int   argc, char *argv[]){
	/* GtkWidget is the storage type for widgets */
	GObject *window;
	
	/* This is called in all GTK applications. Arguments are parsed
	 * from the command line and are returned to the application.
	 */
	gtk_init (&argc, &argv);
	
	builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "res/GUI.glade", NULL);
	
	window = gtk_builder_get_object (builder, "MainWindow");
	
	setImages();
	
	gtk_main ();
	
	return 0;
}

#endif /* GUI_h */
