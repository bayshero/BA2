/*!
  \file   particule.cc
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/
#include <iostream>
#include "shape.h"
#include "message.h"
#include "particule.h"
#include "constantes.h"

using namespace std;

Particule::Particule(Square s)
	: s(s){}
	
double Particule::GetLongueur() const{
	return s.longueur_cote;
}

//vérifie que le coté d'une particule soit supérieur à d_particule_min
bool Particule::taille_min(){
	double taille = s.longueur_cote;
	if(taille > d_particule_min){
		return true;
	}else{
		return false;
	}
}

//vérifie qu'une particule soit bien contenue dans le domaine
bool Particule::in_domaine(){
	S2d centre = s.centre;
	double x = centre.x;
	double y = centre.y;
	double taille = s.longueur_cote;
	double demi_taille = taille / 2.0;
	if ((x - demi_taille >= -dmax) && (x + demi_taille <= dmax) && (y - demi_taille
		>= -dmax) && (y + demi_taille <= dmax)){
		return true;
	}else{
		return false;
	}
}

void Particule::particule_error(){
	if (!taille_min()){
		cout<<message::particle_too_small(s.centre.x,s.centre.y,s.longueur_cote);
		error_initialisation = false;
	} else {
		error_initialisation = true;
	}
	if (!in_domaine()){
		cout<<message::particle_outside(s.centre.x,s.centre.y,s.longueur_cote);
		if (error_initialisation){
			error_initialisation = false;
		}
	} else {
		if (error_initialisation){
			error_initialisation = true;
		}
	}
}
	
Square Particule::GetSquare() const {
	return s;
}

bool Particule::getError_initialisation() const {
	return error_initialisation;
}

//vérifie que deux particules ne se superposent pas
bool Particule::superposition_p(const Particule& p1) const{
	if (collision_ss(s, p1.s)) {
		cout << message::particle_superposition(s.centre.x, s.centre.y, 
												p1.s.centre.x, p1.s.centre.y);
		return false;
	}
	return true;
}

string Particule::get_as_string(){
	string line = to_string(s.centre.x) + " " + to_string(s.centre.y) 
										+ " " + to_string(s.longueur_cote);
	return line;
}

void draw_particule(const Particule& p){
	draw_square(p.GetSquare());
}

