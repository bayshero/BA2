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
public:
	Particule(Square s);  				//constructeur
	double GetLongueur() const;
	bool taille_min();
	bool in_domaine();
	Square GetSquare() const;
	bool superposition_p(const Particule& p1) const;
};

void draw_particule(const Particule& p);

#endif
