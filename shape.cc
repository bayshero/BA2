/*!
  \file   shape.cc
  \author Charly  Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#include "shape.h"
#include "graphic.h"
#include <math.h> 
using namespace std;


//superposition cercle-cercle
bool collision_cc(Circle c1, Circle c2, bool use_epsil) {
	bool collision(false);
	if (use_epsil){
		if (sqrt(pow(c2.centre.x-c1.centre.x,2)+pow(c2.centre.y-c1.centre.y,2))
											<((c1.rayon+c2.rayon)+epsil_zero)){ 
			collision=true;
		}
	}else{
		if (sqrt(pow(c2.centre.x-c1.centre.x,2)+pow(c2.centre.y-c1.centre.y,2))
														<((c1.rayon+c2.rayon))){ 
			collision=true;
		}
	}
	return collision;
}

//superposition carre-carre
bool collision_ss(Square s1, Square s2, bool use_epsil){
	bool collision(false);
	if (use_epsil){
		if (abs(s2.centre.x-s1.centre.x)<(s1.longueur_cote/2+s2.longueur_cote/2+
			epsil_zero)&&(abs(s2.centre.y-s1.centre.y)
			<(s1.longueur_cote/2+s2.longueur_cote/2+epsil_zero))){
			collision = true;
		}else{
			if (abs(s2.centre.x-s1.centre.x)<(s1.longueur_cote/2+s2.longueur_cote/2)
				&&(abs(s2.centre.y-s1.centre.y)
				<(s1.longueur_cote/2+s2.longueur_cote/2))){
				collision = true;
			}
		}
	}
	return collision;
}

//supersotion cercle-carre
bool collision_cs(Circle c2, Square s1, bool use_epsil){
	double l(sqrt(pow(abs(c2.centre.x-s1.centre.x)-(s1.longueur_cote/2),2))
			+pow(abs(c2.centre.y-s1.centre.y)-(s1.longueur_cote/2),2));
	bool collision(false);
	if (use_epsil) {
		if ((abs(c2.centre.x-s1.centre.x)<(s1.longueur_cote/2+c2.rayon+epsil_zero))and
			(abs(c2.centre.y-s1.centre.y)<(s1.longueur_cote/2+c2.rayon+epsil_zero))){
			collision = true;
		}else if (((abs(c2.centre.x-s1.centre.x)>(s1.longueur_cote/2))
					and(abs(c2.centre.y-s1.centre.y)>(s1.longueur_cote/2)))
					and(l>(c2.rayon+epsil_zero))){
			collision = false;
		}
	}else{
		if ((abs(c2.centre.x-s1.centre.x)<(s1.longueur_cote/2+c2.rayon))
			and(abs(c2.centre.y-s1.centre.y)<(s1.longueur_cote/2+c2.rayon))){
			collision = true;
		}else if (((abs(c2.centre.x-s1.centre.x)>(s1.longueur_cote/2))and
					(abs(c2.centre.y-s1.centre.y)>(s1.longueur_cote/2)))
					and(l>(c2.rayon))){
			collision = false;
		}		
	}
	return collision;
}

void draw_circle_spatial(const Circle& c) {
	draw_circle(c.rayon, c.centre.x, c.centre.y, 4);
	draw_dot(c.centre.x, c.centre.y);
}

void draw_circle_neutr(const Circle& c, double orientation) {
	draw_circle(c.rayon, c.centre.x, c.centre.y, 1);
	draw_dot(c.centre.x, c.centre.y);
	draw_line(c.rayon, c.centre.x, c.centre.y, orientation);
}

void draw_circle_rep(const Circle& c) {
	fill_circle(c.rayon, c.centre.x, c.centre.y, 2);
	draw_circle(c.rayon, c.centre.x, c.centre.y, 1);
}

void draw_square(const Square& s) {
	draw_filled_square(s.longueur_cote, s.centre.x, s.centre.y);
}

