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
#include "gtk-image-viewer.h"

int start (int   argc, char *argv[]){
	/* GtkWidget is the storage type for widgets */
	GObject *window;
	
	/* This is called in all GTK applications. Arguments are parsed
	 * from the command line and are returned to the application.
	 */
	gtk_init (&argc, &argv);
	
	GtkBuilder * builder = gtk_builder_new();
	gtk_builder_add_from_file (builder, "res/GUI.glade", NULL);
	
	window = gtk_builder_get_object (builder, "MainWindow");
	
	gtk_main ();
	
	return 0;
}

#endif /* GUI_h */
