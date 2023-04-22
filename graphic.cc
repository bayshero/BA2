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
    (*ptcr)->set_source_rgb(0.0, 0.0, 0.0); //couleur bordure du monde

    // Draw the border rectangle
    (*ptcr)->rectangle(0, 0, taille, taille);
    (*ptcr)->stroke();
}
