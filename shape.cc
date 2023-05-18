/*!
  \file   shape.cc
  \author Charly  Guardia 70%, Gauthier de Mercey 30%
  \date   avril 2023
  \version 2
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
		}
		if (((abs(c2.centre.x-s1.centre.x)>(s1.longueur_cote/2))and
			(abs(c2.centre.y-s1.centre.y)>(s1.longueur_cote/2)))
								   and(l>(c2.rayon+epsil_zero))){
			collision = false;
		}
	}else{
		if ((abs(c2.centre.x-s1.centre.x)<(s1.longueur_cote/2+c2.rayon))
			and(abs(c2.centre.y-s1.centre.y)<(s1.longueur_cote/2+c2.rayon))){
			collision = true;
		}
		if (((abs(c2.centre.x-s1.centre.x)>(s1.longueur_cote/2))and
			 (abs(c2.centre.y-s1.centre.y)>(s1.longueur_cote/2)))
											  and(l>=(c2.rayon))){
			collision = false;
		}		
	}
	return collision;
}

//dessine un robot spatial
void draw_circle_spatial(const Circle& c) {
	draw_circle(c.rayon, c.centre.x, c.centre.y, 4);
	draw_dot(c.centre.x, c.centre.y);
}

//dessine un robot neutraliseur
void draw_circle_neutr(const Circle& c, double orientation, int color) {
	draw_circle(c.rayon, c.centre.x, c.centre.y, color);
	draw_dot(c.centre.x, c.centre.y);
	draw_line(c.rayon, c.centre.x, c.centre.y, orientation);
}

//dessine un robot réparateur
void draw_circle_rep(const Circle& c) {
	fill_circle(c.rayon, c.centre.x, c.centre.y, 2);
	draw_circle(c.rayon, c.centre.x, c.centre.y, 1);
}

//dessine une particule
void draw_square(const Square& s) {
	draw_filled_square(s.longueur_cote, s.centre.x, s.centre.y);
}

double s2d_norm(S2d pos){
	return sqrt(pos.x*pos.x + pos.y*pos.y);
}

double s2d_prod_scal(S2d v1, S2d v2){
	return v1.x*v2.y + v1.x*v2.y;
}

S2d s2d_add_scaled_vector(S2d pos, const S2d& pos_to_goal, double scaling){
	pos.x = pos.x + scaling*pos_to_goal.x;
	pos.y += scaling*pos_to_goal.y;
	return pos;
}

bool intersects(const Square& square, const Circle& cercle){

	double left = square.centre.x - (square.longueur_cote / 2);
	double right = square.centre.x + (square.longueur_cote / 2);
	double top = square.centre.y + (square.longueur_cote / 2);
	double bottom = square.centre.y - (square.longueur_cote / 2);
	
	double closestX;
	if (cercle.centre.x < left) {
		closestX = left;
	} else if (cercle.centre.x > right) {
		closestX = right;
	} else {
		closestX = cercle.centre.x;
	}

	double closestY;
	if (cercle.centre.y < bottom) {
		closestY = bottom;
	} else if (cercle.centre.y > top) {
		closestY = top;
	} else {
		closestY = cercle.centre.y;
	}
	
	double dx = cercle.centre.x - closestX;
	double dy = cercle.centre.y - closestY;
	double distance = sqrt(dx * dx + dy * dy);
	
	return distance <= cercle.rayon;
}

bool operator==(const S2d& lhs, const S2d& rhs){
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

S2d s2d_scale(const S2d& v, double scalar) {
    return {v.x * scalar, v.y * scalar};
}

S2d s2d_subtract(const S2d& a, const S2d& b) {
    return {a.x - b.x, a.y - b.y};
}
