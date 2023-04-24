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
#include <random>

using namespace std;

class Simulation{
public :
	Simulation();

	void lecture(const char* file_name);
	void lire_ligne(string ligne);
	
	void parcourir_p();
	void parcourir_r_neutre();
	void parcourir_r_rep();
	void parcourir_r_neutre_rep();
	void parcourir_p_r_rep();
	void parcourir_p_r_neutre(); 
	void taille_attribut_check();
	void parcourir_p_rs();
	void error_check();
	void fin_succes();
	
	vector<Particule> GetParticules() const;
	R_spatial GetRs() const;
	bool getError_simu() const;

	void save(string save_filename);
	
	void setRsNbUpdate(int newNbUpdate);
	//simulation
	void lance_simulation();
	void desintegration_particules();
	static vector<Particule> particules;
	static vector<R_neutraliseur> robots_neutr;
	static vector<R_reparateur> robots_rep;
	static R_spatial rs;
private :	
	bool bool_error; //false si il y a une erreur détectée dans la simulation
	default_random_engine e;
};

void draw_world();

#endif

