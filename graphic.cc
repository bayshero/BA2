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

void empty_world(unsigned int taille) {
    // Set line width and color
    (*ptcr)->set_line_width(1.0);
    (*ptcr)->set_source_rgb(0.01, 0.01, 0.01); //couleur bordure du monde

    // Draw the border rectangle
    (*ptcr)->rectangle(0, 0, taille, taille);
    (*ptcr)->stroke();
}
/*
void set_color(int indice){
	(*ptcr)->set_source_rgb(Couleur[indice%6][0],Couleur[indice%6][1], Couleur[indice%6][2]);
}
*/
void draw_square_border(double longueur_cote, double x, double y) {
    // Set line width and color
    (*ptcr)->set_line_width(2.0);
    (*ptcr)->set_source_rgb(1.0, 0.0, 0.0); //couleur bordure du monde

    // Draw the border rectangle
    (*ptcr)->rectangle(x, y, longueur_cote, longueur_cote);
    (*ptcr)->stroke();
}

void fill_square(double longueur_cote, double x, double y){
   // Set the source color for the fill
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5); // Gray color for fill
    // Draw the filled rectangle
    (*ptcr)->rectangle(x, y, longueur_cote, longueur_cote);
    (*ptcr)->fill();
}

void draw_circle_neutr_border(double rayon, double x, double y){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI); // full circle
	(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);    
	(*ptcr)->set_line_width(0.5);
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
	(*ptcr)->set_source_rgb(0.0, 0.0, 1.0);    
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->stroke();
}

