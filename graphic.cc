/*!
  \file   graphic.h
  \author Charly Guardia 0%, Gauthier de Mercey 100%
  \date   avril 2023
  \version 2
*/

#include "graphic.h"
#include <iostream>

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}

void empty_world() {
	(*ptcr)->set_source_rgb(1.0, 1.0, 1.0 );
	(*ptcr)->paint();
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

//initialise la couleur
void set_color(int indice){
	if (indice ==1){ //black
		(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
	}
	else if (indice == 2){ //green
		(*ptcr)->set_source_rgb(0.0, 1.0, 0.0);		
	}
	else if (indice == 3){ //red
		(*ptcr)->set_source_rgb(1.0, 0.0, 0.0);
	}
	else if (indice == 4){ //turquoise
		(*ptcr)->set_source_rgb(0.0, 0.8, 1.0);
	}
	else if (indice == 5){ //gray
		(*ptcr)->set_source_rgb(0.5, 0.5, 0.5);
	}
	else if (indice == 6){ //violet
		(*ptcr)->set_source_rgb(0.5, 0, 0.5);
	}
	else if (indice == 7){ //orange
		(*ptcr)->set_source_rgb(1, 0.45, 0);
	}
}	

//dessine un carre rempli
void draw_filled_square(double longueur_cote, double x, double y) {
	(*ptcr)->set_line_width(1.5);
	(*ptcr)->set_source_rgb(1.0, 0.0, 0.0);

	// cherche le coin en haut a gauche
	double x_top_left = x - longueur_cote / 2.0;
	double y_top_left = y - longueur_cote / 2.0;

	// dessine le bord du rectangle 
	(*ptcr)->rectangle(x_top_left, y_top_left, longueur_cote, longueur_cote);
	(*ptcr)->stroke();
    
	(*ptcr)->set_source_rgb(0.5, 0.5, 0.5); // Gray color for fill
	// dessine le rectangle rempli
	(*ptcr)->rectangle(x_top_left, y_top_left, longueur_cote, longueur_cote);
	(*ptcr)->fill();
}

//dessine un cercle
void draw_circle(double rayon, double x, double y, int indice){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI); // full circle
	set_color(indice); 
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->stroke();
}

//dessine un cercle rempli
void fill_circle(double rayon, double x, double y, int indice){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, rayon, 0.0, 2.0 * M_PI);	
	set_color(indice);
	(*ptcr)->fill_preserve();
}

//dessine ligne d'orientation
void draw_line(double rayon, double x, double y, double orientation){
	double x2 = x + rayon * cos(orientation);
	double y2 = y + rayon * sin(orientation);
	
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->set_source_rgb(0.0, 1.0, 0.0); 
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x2, y2);
	(*ptcr)->stroke();
}

//dessine un point
void draw_dot(double x, double y){
	(*ptcr)->save();
	(*ptcr)->arc(x, y, 0.2, 0.0, 2.0 * M_PI);  
	(*ptcr)->set_line_width(0.5);
	(*ptcr)->fill_preserve();
	(*ptcr)->stroke();
}
