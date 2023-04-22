#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

void graphic_empty_world();
void set_color(int indice);
void set_color_light(int indice);
void fill_unitsquare(double x, double y, int indice);
void fill_diamond(double x, double y);
void thin_square(double x, double y, double size,int indice);
void thick_line(double x, double y, double size, int indice);
void thick_line_light(double x, double y, double size, int indice);


#endif
