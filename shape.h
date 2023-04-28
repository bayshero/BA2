/*!
  \file   shape.h
  \author Charly  Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/



#ifndef SHAPE_H
#define SHAPE_H
#include <iostream>

constexpr double epsil_zero(0.125);

struct S2d {
	double x=0.0;
	double y=0.0;
};

struct Circle{
	double rayon;
	S2d centre;
};

struct Square{
	S2d centre;
	double longueur_cote;
};

//use_epsil est vrai par défaut--> epsil zero est utilisé
bool collision_cc(Circle c1, Circle c2, bool use_epsil=true);
bool collision_ss(Square c1, Square s2, bool use_epsil=true);
bool collision_cs(Circle c2, Square s1, bool use_epsil=true);

void draw_circle_rep(const Circle& c);
void draw_circle_neutr(const Circle& c, double orientation);
void draw_square(const Square& s);
void draw_circle_spatial(const Circle& c);

#endif

