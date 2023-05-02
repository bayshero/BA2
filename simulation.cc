/*!
  \file   simulation.cc
  \author Charly Guardia 60%, Gauthier de Mercey 40%
  \date   avril 2023
  \version 2
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

static vector<Particule> particules;
static vector<R_neutraliseur> robots_neutr;
static vector<R_reparateur> robots_rep;
static R_spatial rs;

static default_random_engine e;


Simulation::Simulation(){}


//lit le fichier text reçu 
void Simulation::lecture(string file_name){
	e.seed(1); //à chaque lecture, reset la sequence de nombres aleatoires
	string ligne;
	ifstream fichier(file_name);
	if (!fichier.fail()){			//message d'erreur
		while(getline(std::ws(fichier),ligne)){
			if (ligne[0]=='#') {
				continue;
			};
			lire_ligne(ligne);
		}
		set_nbNp();
		triParticule();
	}else exit(EXIT_FAILURE);
}

//décode une ligne du fichier text reçu
void Simulation::lire_ligne(string ligne){
	enum Section_lecture {NBP_,PARTICULE, R_SPAT,R_REP,R_NEUTR,FIN};
	istringstream data(ligne);
	Circle c1;
	Square s1;
	double x, y, d_particule, orient;  //a = orientation, c_n =type de coordination
	int nbUpdate, nbNr, nbNs, nbNd, nbRr, nbRs, nbP, c_n, k_update_panne;
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
				s1 = {{x,y},d_particule};
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
				c1 = {r_spatial,{x,y}};
				R_spatial rs_(c1, nbUpdate, nbNr, nbNs, nbNd, nbRr, nbRs);
				rs= rs_;
				if ((nbNr+nbNs==0)and(nbRr+nbRs==0)){
					section = NBP_;
				 } else if (nbRr+nbRs==0){
						section=R_NEUTR;
						} else {
							section = R_REP;
						}
				break;
			}
		case R_REP:
			if (!(data>>x>>y)) exit(EXIT_FAILURE);
			else {
				c1 = {r_reparateur,{x,y}};
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
				c1 = {r_neutraliseur,{x,y}};
				robots_neutr.push_back(R_neutraliseur(c1, orient, k_update_panne, 
														panne_, c_n));
				++i;
				if (i==nbNs) {
					i = 0;
					section = NBP_; 
				}
				break;
			}
		case FIN:
			break;
	}		
}

//parcourt le vector de particules
void Simulation::parcourir_p() {
	for (unsigned int i(0); i<particules.size(); i++){
		for (unsigned int j(i+1); j<particules.size(); j++){
			if(!particules[i].superposition_p(particules[j])){
				bool_error = false;
			}	
		}
	}
}

//parcourt le vector de robots neutraliseurs
void Simulation::parcourir_r_neutre() {
	for (unsigned int i(0); i<robots_neutr.size(); ++i){
		for (unsigned int j(i+1); j<robots_neutr.size(); ++j){
			if(!robots_neutr[i].superposition_r_neutre(robots_neutr[j])){
				bool_error = false;
			}
		}
	}
}

//parcourt le vector de robots réparateurs
void Simulation::parcourir_r_rep() {
	for (unsigned int i(0); i<robots_rep.size(); ++i){
		for (unsigned int j(i+1); j<robots_rep.size(); ++j){
			if(!robots_rep[i].superposition_r_reparateur(robots_rep[j])){
				bool_error = false;
			}
		}
	}
}

//parcourt les vectors de robots rép/neutr.
void Simulation::parcourir_r_neutre_rep() {
	for (unsigned int i(0); i<robots_rep.size(); ++i){
		for (unsigned int j(0); j<robots_neutr.size(); ++j){
			if(!robots_rep[i].superposition_r_neutre_rep(robots_neutr[j])){
				bool_error = false;
			}
		}
	}
}

//parcourt les vectors de particules/robots rép
void Simulation::parcourir_p_r_rep() {
	for (unsigned int i(1); i<robots_rep.size(); ++i){
		for (unsigned int j(0); j<particules.size(); ++j){
			if(!robots_rep[i].superposition_p_r_reparateur(particules[j])){
				bool_error = false;
			}
		}
	}
}

//parcourt les vectors de particules/robots neutr.
void Simulation::parcourir_p_r_neutre() {
	for (unsigned int i(1); i<robots_neutr.size(); ++i){
		for (unsigned int j(0); j<particules.size(); ++j){
			if(!robots_neutr[i].superposition_p_r_neutraliseur(particules[j])){
				bool_error = false;
			}
		}
	}
}

//parcourt le vector de robot neutr. (pour comparer le nbUpdate au kUpdate)
void Simulation::taille_attribut_check() { 
	for(unsigned int i(0);i<robots_neutr.size();++i){
		if(!robots_neutr[i].error_attribut(rs)){
			bool_error = false;
		}
	}
}

//parcourt le vector de particule (pour tester la superposition entre Rs et particule)
void Simulation::parcourir_p_rs() {
	for (unsigned int i(0); i<particules.size(); i++){
		if (!rs.superposition_p_rs(particules[i])){
			bool_error = false;
		}
	}
}

R_spatial Simulation::getRs() const{
	return rs;
}

vector<Particule> Simulation::getParticules() const{
	return particules;
}

//envoie "correct file" si aucune erreur detectee
void Simulation::fin_succes(){
	cout<<message::success();

}

//fait appel à toutes les fonctions permettant de tester les erreurs
void Simulation::error_check(){
	bool_error = true;
	
	bool_error = rs.getError_domain();
	
	for (auto particule : particules){
		if (bool_error){
			particule.particule_error();
			bool_error = particule.getError_initialisation();
		}
	}
	parcourir_p();
	parcourir_r_neutre();
	parcourir_r_rep();
	parcourir_r_neutre_rep();
	taille_attribut_check();
	parcourir_p_rs();
	parcourir_p_r_neutre();
	parcourir_p_r_rep();
	
	if (bool_error){ 
		fin_succes();
	}
}

//réécrit dans un fichier l'état courant de la simulation
void Simulation::save(string save_filename) {
    std::ofstream file(save_filename);

    if (!file.is_open()) {
        return;
    }

    file << particules.size() << endl;

    for (auto part : particules){
		file << part.Particule::get_as_string() << endl;
	}
	file << rs.get_as_string() << endl;
	
	for(auto robot_rep : robots_rep){
		file << robot_rep.R_reparateur::get_as_string() << endl;
	}
	
	for (auto robot_neutr : robots_neutr){
		file << robot_neutr.R_neutraliseur::get_as_string() << endl;
	}

    file.close();
}

void Simulation::setRsNbUpdate(int newNbUpdate) {
    rs.setNbUpdate(newNbUpdate);
}

bool Simulation::getError_simu() const{
	return bool_error;
}

void Simulation::desintegration_particules() {
    double p(desintegration_rate);
    bernoulli_distribution b(p / particules.size());
    vector<Particule> new_particules; 
    
    for (auto particule : particules) {
		double new_longueur = (particule.getLongueur()/2) - (2*epsil_zero);
		if (b(e)) {
			//desintegration d'une particule si sa future taile > d_particule + e0
			if (new_longueur > d_particule_min + epsil_zero) {
				// Désintégration de la particule
				// Ajoute 4 nouvelles particules
				double d = particule.getLongueur() / 4;
				double pos_x = particule.getSquare().centre.x;
				double pos_y = particule.getSquare().centre.y;
				S2d centre1 = {pos_x - d, pos_y + d};
				S2d centre2 = {pos_x - d, pos_y - d};
				S2d centre3 = {pos_x + d, pos_y + d};
				S2d centre4 = {pos_x + d, pos_y - d};
				Square s1 = {centre1, new_longueur};
				Square s2 = {centre2, new_longueur};
				Square s3 = {centre3, new_longueur};
				Square s4 = {centre4, new_longueur};
				new_particules.push_back(Particule(s1));
				new_particules.push_back(Particule(s2));
				new_particules.push_back(Particule(s3));
				new_particules.push_back(Particule(s4));
			} else {
				// Ajoute la particule au nouveau vecteur (si pas de désintegration)
				new_particules.push_back(particule);
			}
		} else {
			new_particules.push_back(particule);
		}
	}

    // Remplace le vecteur original par le nouveau vecteur
    particules = new_particules;
}

void Simulation::lance_simulation() {
	//appel des fonctions en charge de lancer la simulation
    desintegration_particules();
    
    robots_neutr_cible();
    //for (auto robot_neutr : robots_neutr){
		//cout<<"***************************************"<<endl;
		//cout<<robot_neutr.getGoal().x<<" "<< robot_neutr.getGoal().y<<endl;
	//}
}

//dessine le monde courant
void draw_world(){
    for (unsigned int i(0); i<particules.size(); ++i){
        particules[i].draw_particule();
    }
    for (unsigned int j(0); j<robots_neutr.size(); ++j){
		robots_neutr[j].draw_robot_neutr();
	}
	for (unsigned int k(0); k<robots_rep.size(); ++k){
		robots_rep[k].draw_robot_rep();
	}
	rs.draw_robot_spatial();
}

//cette fonction se charge de supprimer toutes les structures de données
void Simulation::delete_simu(){
	particules.clear();
	robots_neutr.clear();
	robots_rep.clear();
	rs.delete_rs();
}

//initaliase le nb de robots en panne grâce à la variable booléenne des robots neutr.
void Simulation::set_nbNp(){
	int val(0);
	for (auto robot_neutr : robots_neutr){
		if (robot_neutr.getPanne() == true) {
			val = val + 1;
		}
	}
	rs.setNbNp(val);
}

void Simulation::robots_neutr_cible(){
	
	//pour chaque particule, trouver le robot neutraliseur le plus proche
	//et lui donner cette particule comme but
	for (const auto& particule : particules){
		//cout<<"**************************"<<endl;
		//cout <<particule.getSquare().centre.x<<" "<<particule.getSquare().centre.y<<endl;
		double minDistance = 1000000;
		R_neutraliseur* R_neutr_proche = nullptr;
		
		for (auto& robot_neutr : robots_neutr){
			double part_x = particule.getSquare().centre.x;
			double part_y = particule.getSquare().centre.y;
			double robot_x = robot_neutr.getCircle().centre.x;
			double robot_y = robot_neutr.getCircle().centre.y;
			S2d vecteur_robot_part = {part_x - robot_x, part_y - robot_y};
			double distance =  s2d_norm(vecteur_robot_part);
			
			if (distance< minDistance){
				minDistance = distance;
				R_neutr_proche = &robot_neutr;
			}
		}
		R_neutr_proche->setGoal(particule.getSquare().centre);
	}
	
}

//tri le vector de particules par ordre décroissant de taille
void Simulation::triParticule(){
	for (unsigned int i(0); i< particules.size(); ++i){
		unsigned int maxIndex = i;
		for (unsigned int j = i+ 1; j < particules.size(); ++j){
			if (particules[j].getLongueur() > particules[maxIndex].getLongueur()){
				maxIndex = j;
			}
		}
		if (maxIndex != i) {
			std::swap(particules[i], particules[maxIndex]);
		}
	}
}


