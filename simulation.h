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
	void zone_a_risque_verifier(Particule& p);
	bool detruire_particule();
	void creation_robots();
	
	void robots_neutr_cible();
	void robots_rep_cible();
	void triParticule();
	
	void robot_bouge();
	void panne_destroy();
	void robot_rentre_maison();
	void verifie_SiCibleExiste();
	bool verifie_si_spawn_vide();
	void r_neutr_check();
	bool is_particle_targeted(const S2d& particle_center);
	
	void draw_world();
	
	
private :
	static vector<Particule> particules;
	static vector<R_neutraliseur> robots_neutr;
	static vector<R_reparateur> robots_rep;
	static R_spatial rs;
	bool bool_error; //false si il y a une erreur détectée dans la simulation

};

#endif

