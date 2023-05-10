/*!
  \file   graphic.h
  \author Charly Guardia 0%, Gauthier de Mercey 100%
  \date   avril 2023
  \version 2
*/

#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>
#include "constantes.h"
#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
void empty_world();
void draw_filled_square(double longueur_cote, double x, double y);
void set_color(int indice);
void draw_dot(double x, double y);
void draw_circle(double rayon, double x, double y, int indice);
void draw_line(double rayon, double x, double y, double orientation);
void fill_circle(double rayon, double x, double y, int indice);

#endif // GTKMM_EXAMPLE_MYAREA_H
