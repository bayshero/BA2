#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

void empty_world(unsigned int taille, double xmin, double ymin, double xmax, double ymax);

//void set_color(int indice);
void draw_square_border(double longueur_cote, double x, double y);
void fill_square(double longueur_cote, double x, double y);
void draw_circle_neutr_border(double rayon, double x, double y);
void draw_circle_rep_border(double rayon, double x, double y);
void draw_circle_spatial_border(double rayon, double x, double y);
#endif // GTKMM_EXAMPLE_MYAREA_H
