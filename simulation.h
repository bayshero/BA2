/*!
  \file   simulation.h
  \author Charly Guardia 60%, Gauthier de Mercey 40%
  \date   avril 2023
  \version 2
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
	
	//lecture du fichier
	void lecture(string file_name);
	void lire_ligne(string ligne);
	
	//fonction pour vérifier les erreurs
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
	
	//fonctions getters
	vector<Particule> getParticules() const;
	R_spatial getRs() const;
	bool getError_simu() const;
	
	void delete_simu();
	void save(string save_filename);
	
	//fonctions setters
	void setRsNbUpdate(int newNbUpdate);
	void set_nbNp();
	
	//simulation
	void lance_simulation();
	void desintegration_particules();
	
	void robots_neutr_cible();
	void triParticule();
	
private :

	bool bool_error; //false si il y a une erreur détectée dans la simulation

};

void draw_world();

#endif

