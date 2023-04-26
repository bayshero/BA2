#include "graphic.h"
#include "constantes.h"
#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <iostream>

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}


void empty_world() {
	// Set line width and color
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(0.01, 0.01, 0.01);
	//draw borders
    (*ptcr)->move_to(-dmax, -dmax);
    (*ptcr)->line_to(-dmax, dmax);
    (*ptcr)->line_to(dmax, dmax);
    (*ptcr)->line_to(dmax, -dmax);
    (*ptcr)->line_to(-dmax, -dmax);
    (*ptcr)->stroke();
}


void draw_filled_square(double longueur_cote, double x, double y) {
    // Set line width and color
    (*ptcr)->set_line_width(1.5);
    (*ptcr)->set_source_rgb(1.0, 0.0, 0.0);

    // Calculate the top-left corner coordinates based on the square's center
    double x_top_left = x - longueur_cote / 2.0;
    double y_top_left = y - longueur_cote / 2.0;

    // Draw the border rectangle
    (*ptcr)->rectangle(x_top_left, y_top_left, longueur_cote, longueur_cote);
    (*ptcr)->stroke();
    
	// Set the source color for the fill
    (*ptcr)->set_source_rgb(0.5, 0.5, 0.5); // Gray color for fill
    // Draw the filled rectangle
    (*ptcr)->rectangle(x_top_left, y_top_left, longueur_cote, longueur_cote);
    (*ptcr)->fill();
}


void draw_circle_neutr_border(double rayon, double x, double y, double orientation){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI); // full circle
	(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);    
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->stroke();
	
	double x2 = x + rayon * cos(orientation);
	double y2 = y + rayon * sin(orientation);
	
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->set_source_rgb(0.0, 1.0, 0.0); // Set the line color (e.g., red)
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x2, y2);
	(*ptcr)->stroke();
	
}

void draw_circle_rep_border(double rayon, double x, double y){
    (*ptcr)->save();
    (*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI); // full circle

    // Fill the circle with green color
    (*ptcr)->set_source_rgb(0.0, 1.0, 0.0);
    (*ptcr)->fill_preserve();

    // Set the border color to black
    (*ptcr)->set_source_rgb(0.0, 0.0, 0.0);    
    (*ptcr)->set_line_width(0.5);

    // Stroke the border
    (*ptcr)->stroke();

    (*ptcr)->restore();
}

void draw_circle_spatial_border(double rayon, double x, double y){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI); // full circle
	(*ptcr)->set_source_rgb(0.0, 0.8, 1.0);    
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->stroke();
}
/*
void draw_orientation_line(double rayon, double x, double y, double orientation){
    double x1 = x;
    double y1 = y;

    double x2 = x1 + rayon * cos(orientation);
    double y2 = y1 + rayon * sin(orientation);

    (*ptcr)->save();
    (*ptcr)->set_line_width(0.5);
    (*ptcr)->set_source_rgb(1.0, 0.0, 0.0); // Set the line color (e.g., red)
    (*ptcr)->move_to(x1, y1);
    (*ptcr)->line_to(x2, y2);
    (*ptcr)->stroke();
    (*ptcr)->restore();
}
*/
