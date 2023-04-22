#include "graphic.h"
#include "constantes.h"
#include <cairomm/context.h>
#include <giomm/resource.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <iostream>

void empty_world(const Cairo::RefPtr<Cairo::Context>& cr, unsigned int taille) {
    // Set line width and color
    cr->set_line_width(1.0);
    cr->set_source_rgb(0.0, 0.0, 0.0); //couleur bordure du monde

    // Draw the border rectangle
    cr->rectangle(0, 0, taille, taille);
    cr->stroke();
}
