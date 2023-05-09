/*!
  \file   particule.h
  \author Charly Guardia 0%, Gauthier de Mercey 100%
  \date   avril 2023
  \version 2
*/

#ifndef PARTICULES_H
#define PARTICULES_H
#include "shape.h"

class Particule {
private:
	Square s;
	bool error_initialisation;
	bool deja_ciblee;
public:
	Particule(Square s);  				//constructeur
	
	double getLongueur() const;
	Square getSquare() const;
	bool getError_initialisation() const;
	bool getDeja_ciblee() const;
	
	void setDeja_ciblee(bool newDeja_ciblee);
	
	bool taille_min();
	bool in_domaine();
	void particule_error();
	bool superposition_p(const Particule& p1) const;
	std::string get_as_string();
	void resetDeja_ciblee();
	
	void draw_particule();
};

/*
class ContaminatedZone {
private:
    Square zone;
    const Particule& linked_particle;
public:
    ContaminatedZone(const Particule& particle, double risk_factor);
    bool is_active() const;
    void update();
};
*/

#endif
