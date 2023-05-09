/*!
  \file   shape.h
  \author Charly  Guardia 70%, Gauthier de Mercey 30%
  \date   avril 2023
  \version 2
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

bool operator==(const S2d& lhs, const S2d& rhs);

//use_epsil est vrai par défaut--> epsil zero est utilisé
bool collision_cc(Circle c1, Circle c2, bool use_epsil=true);
bool collision_ss(Square c1, Square s2, bool use_epsil=true);
bool collision_cs(Circle c2, Square s1, bool use_epsil=true);

void draw_circle_rep(const Circle& c);
void draw_circle_neutr(const Circle& c, double orientation);
void draw_square(const Square& s);
void draw_circle_spatial(const Circle& c);

S2d s2d_add_scaled_vector(S2d pos, const S2d& pos_to_goal, double scaling);
double s2d_prod_scal(S2d v1, S2d v2);
double s2d_norm(S2d pos);

#endif

