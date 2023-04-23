/*!
  \file   simulation.h
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#ifndef SIMULATION_H
#define SIMULATION_H

#include "robot.h"
#include "particule.h"
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Simulation{
public :
	void lecture(const char* file_name);
	void lire_ligne(string ligne);
	vector<Particule> GetParticules() const;
	void parcourir_p() const;
	void parcourir_r_neutre() const;
	void parcourir_r_rep() const;
	void parcourir_r_neutre_rep() const;
	void parcourir_p_r_rep() const;
	void parcourir_p_r_neutre() const; 
	void taille_attribut_check() const;
	void parcourir_p_rs() const;
	void error_check();
	R_spatial GetRs() const;
	void fin_succes();
	void save(const char* save_filename);
	static vector<Particule> particules;
	static vector<R_neutraliseur> robots_neutr;
	static vector<R_reparateur> robots_rep;
	static R_spatial rs;
private :
	
			
	bool bool_error; //false si il y a une erreur détectée dans la simulation
};

void draw_world();

#endif

