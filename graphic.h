#ifndef GTKMM_EXAMPLE_MYAREA_H
#define GTKMM_EXAMPLE_MYAREA_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/pixbuf.h>

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

void empty_world(unsigned int taille);


#endif // GTKMM_EXAMPLE_MYAREA_H
