/*!
  \file   simulation.cc
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#include "constantes.h"
#include "simulation.h"
#include "shape.h"
#include "robot.h"
#include "particule.h"
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include "message.h"

using namespace std;

vector<Particule> Simulation::particules;
vector<R_reparateur> Simulation::robots_rep;
vector<R_neutraliseur> Simulation::robots_neutr;
R_spatial Simulation::rs;

//lit le fichier text reçu 
void Simulation::lecture(const char* file_name){	
	string ligne;
	ifstream fichier(file_name);
	if (!fichier.fail()){			//message d'erreur
		while(getline(std::ws(fichier),ligne)){
			if (ligne[0]=='#') {
				continue;
			}
			lire_ligne(ligne);
		}
	}else exit(EXIT_FAILURE);
}

//décode une ligne du fichier text reçu
void Simulation::lire_ligne(string ligne){
	enum Section_lecture {NBP_,PARTICULE, R_SPAT,R_REP,R_NEUTR,FIN};
	istringstream data(ligne);
	Circle c1;
	Square s1;
	double x, y, d_particule, orient;  //a = orientation, c_n =type de coordination
	int nbUpdate, nbNr, nbNs,nbNd, nbNp(0), nbRr, nbRs, nbP, c_n, k_update_panne;
	string panne_str;
	static int section(NBP_), i(0);  //première ligne à lire (cond. initiale)
	switch(section){
		case NBP_:
			if(!(data>>nbP)) exit(EXIT_FAILURE);
			else{
				section= PARTICULE;
				break;
			}
		case PARTICULE:
			if(!(data>>x>>y>>d_particule)) exit(EXIT_FAILURE);
			else {
				s1.centre.x=x;
				s1.centre.y=y;
				s1.longueur_cote=d_particule;
				Particule c(s1);
				particules.push_back(c);  //ajoute chaque particule à son vector
				++i;
				if (i==nbP){
					section=R_SPAT;
					i=0;
				}
				break;
			}
		case R_SPAT:
			if(!(data>>x>>y>>nbUpdate>>nbNr>>nbNs>>nbNd>>nbRr>>nbRs)){
				exit(EXIT_FAILURE);
			}else{
				c1.rayon = r_spatial;
				c1.centre.x=x;
				c1.centre.y=y;
				R_spatial rs_(c1, nbUpdate, nbNr, nbNs, nbNd, nbNp, nbRr, nbRs);
				rs= rs_;	
				section = R_REP;
				break;
			}
		case R_REP:
			if (!(data>>x>>y)) exit(EXIT_FAILURE);
			else {
				c1.rayon= r_reparateur;
				c1.centre.x=x;
				c1.centre.y=y;
				R_reparateur rr(c1);
				robots_rep.push_back(rr);
				++i;
				if (i==nbRs){
					section=R_NEUTR;
					i=0;
				}
				break;
			}
		case R_NEUTR:
			if (!(data>>x>>y>>orient>>c_n>>panne_str>>k_update_panne)){     
				exit(EXIT_FAILURE);
			}else{
				bool panne_ =(panne_str=="true");
				c1.rayon=r_neutraliseur;
				c1.centre.x=x;
				c1.centre.y=y;
				robots_neutr.push_back(R_neutraliseur(c1, orient, k_update_panne, 
														panne_));
				++i;
				if (i==nbNs) section= FIN; 
				break;
			}
		case FIN:
			break;
	}		
}

//parcourt le vector de particules
void Simulation::parcourir_p() const {
	for (unsigned int i(0); i<particules.size(); i++){
		for (unsigned int j(i+1); j<particules.size(); j++){
			particules[i].superposition_p(particules[j]);	
		}
	}
}

//parcourt le vector de robots neutraliseurs
void Simulation::parcourir_r_neutre() const {
	for (unsigned int i(0); i<robots_neutr.size(); ++i){
		for (unsigned int j(i+1); j<robots_neutr.size(); ++j){
			robots_neutr[i].superposition_r_neutre(robots_neutr[j]);
		}
	}
}

//parcourt le vector de robots réparateurs
void Simulation::parcourir_r_rep() const {
	for (unsigned int i(0); i<robots_rep.size(); ++i){
		for (unsigned int j(i+1); j<robots_rep.size(); ++j){
			robots_rep[i].superposition_r_reparateur(robots_rep[j]);
		}
	}
}

//parcourt les vectors de robots rép/neutr.
void Simulation::parcourir_r_neutre_rep() const{
	for (unsigned int i(0); i<robots_rep.size(); ++i){
		for (unsigned int j(0); j<robots_neutr.size(); ++j){
			robots_rep[i].superposition_r_neutre_rep(robots_neutr[j]);
		}
	}
}

//parcourt les vectors de particules/robots rép
void Simulation::parcourir_p_r_rep() const{
	for (unsigned int i(1); i<robots_rep.size(); ++i){
		for (unsigned int j(0); j<particules.size(); ++j){
			robots_rep[i].superposition_p_r_reparateur(particules[j]);
		}
	}
}

//parcourt les vectors de particules/robots neutr.
void Simulation::parcourir_p_r_neutre() const{
	for (unsigned int i(1); i<robots_neutr.size(); ++i){
		for (unsigned int j(0); j<particules.size(); ++j){
			robots_neutr[i].superposition_p_r_neutraliseur(particules[j]);
		}
	}
}

//parcourt le vector de robot neutr. (pour comparer le nbUpdate au kUpdate)
void Simulation::taille_attribut_check() const{ 
	for(unsigned int i(0);i<robots_neutr.size();++i){
		robots_neutr[i].error_attribut(rs);
	}
}

//parcourt le vector de particule (pour tester la superposition entre Rs et particule)
void Simulation::parcourir_p_rs() const{
	for (unsigned int i(0); i<particules.size(); i++){
		rs.superposition_p_rs(particules[i]);
	}
}

R_spatial Simulation::GetRs() const{
	return rs;
}

vector<Particule> Simulation::GetParticules() const{
	return particules;
}

//envoie "correct file" si aucune erreur detectee
void Simulation::fin_succes(){
	cout<<message::success();

}

//fait appel à toutes les fonctions permettant de tester les erreurs
void Simulation::error_check(){
	
	parcourir_p();
	parcourir_r_neutre();
	parcourir_r_rep();
	parcourir_r_neutre_rep();
	taille_attribut_check();
	parcourir_p_rs();
	parcourir_p_r_neutre();
	parcourir_p_r_rep();	
	fin_succes(); 	
}

void Simulation::save(const char* save_filename) {
    std::ofstream output_file(save_filename);

    if (!output_file.is_open()) {
        std::cerr << "Error: Unable to open file for writing: " << save_filename << std::endl;
        return;
    }

    // Write updated values to the file
    // Replace this with your actual data and format
    output_file << "Updated values go here" << std::endl;

    output_file.close();
}


void draw_world(){
    for (unsigned int i(0); i<Simulation::particules.size(); ++i){
        draw_particule(Simulation::particules[i]);
    }
    for (unsigned int j(0); j<Simulation::robots_neutr.size(); ++j){
		draw_robot_neutr(Simulation::robots_neutr[j]);
	}
	for (unsigned int k(0); k<Simulation::robots_rep.size(); ++k){
		draw_robot_rep(Simulation::robots_rep[k]);
	}
	draw_robot_spatial(Simulation::rs);
}




	

	





