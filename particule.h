/*!
  \file   particule.h
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#ifndef PARTICULES_H
#define PARTICULES_H
#include "shape.h"

class Particule {
private:
	Square s;
	bool error_initialisation;
public:
Particule(Square s);  				//constructeur
	
	double getLongueur() const;
	Square getSquare() const;
	bool getError_initialisation() const;
	
	bool taille_min();
	bool in_domaine();
	void particule_error();
	bool superposition_p(const Particule& p1) const;
	std::string get_as_string();
	
	void draw_particule();
};

#endif
