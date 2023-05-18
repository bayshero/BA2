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
#include <algorithm>

using namespace std;

vector<Particule> Simulation::particules;
vector<R_neutraliseur> Simulation::robots_neutr;
vector<R_reparateur> Simulation::robots_rep;
R_spatial Simulation::rs;

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
				if (nbP==0){
					section = R_SPAT;
				} else {
					section= PARTICULE;
				}
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
				if ((nbNs==0)and(nbRs==0)){
					section = NBP_;
				 } else if (nbRs==0){
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
				zone_a_risque_verifier(particule);
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

//appel des fonctions en charge de lancer la simulation
void Simulation::lance_simulation() {
	//desintegration des particules + tri du vector par ordre décroissant de taille
    desintegration_particules(); 
    verifie_SiCibleExiste();
    r_neutr_check();
    triParticule();
    
    //update nombre de robots en panne + destruction si trop longtemps en panne
    set_nbNp();
    panne_destroy();
    
    //robot spatial prend des décisions
	creation_robots();
    robots_neutr_cible(); //SEG FAUTL
    robots_rep_cible();
    
    //mouvement robots
    robot_bouge();
    
    cout<<"***********************"<<endl;
    
    
    cout<<"robot neutr://////////////////////"<<endl;
    for (auto& robot_neutr : robots_neutr){
		//cout<<"booleen panne: "<<robot_neutr.getPanne()<<endl;
		cout<<"pos, x: "<<robot_neutr.getCircle().centre.x<<" y : "<<robot_neutr.getCircle().centre.y<<endl;
		cout<<"goal, x: "<<robot_neutr.getGoal().x<<" y : "<<robot_neutr.getGoal().y<<endl;
		cout<<"orientation: "<<robot_neutr.getOrientation()<<endl;
		cout<<"oriented1: "<<robot_neutr.getOriented()<<" | oriented2: "<<robot_neutr.getOriented2()<<endl;
		cout<<"------------------------------"<<endl;
	}
	/*
	cout<<"robot rep://////////////////"<<endl;
	for (auto& robot_rep : robots_rep){
		cout<<"robot rep booleen : "<<robot_rep.getBoolGoal()<<endl;
		cout<<"goal, x: "<<robot_rep.getGoal().x<<" y : "<<robot_rep.getGoal().y<<endl;
	}
	*/
	/*
	cout<<"particules://////////////////"<<endl;
	for (auto& particule : particules){
		cout<<"particule, x: "<<particule.getSquare().centre.x<<" y : "<<particule.getSquare().centre.y<<endl;
		cout<<"bool"<<particule.getDeja_ciblee()<<endl;
	}
	*/
	
	bool target_destroyed = detruire_particule();
	robot_rentre_maison();
	
	
	if (!target_destroyed){
		robots_neutr_cible();
	}
}

//dessine le monde courant
void Simulation::draw_world(){
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
	for (unsigned int j(0); j<particules.size();++j){
		if (!particules[j].getDeja_ciblee()) {
			double part_x = particules[j].getSquare().centre.x;
			double part_y = particules[j].getSquare().centre.y;
			Square part_forme = particules[j].getSquare();
			double temps_min = 1000000;
			int minDistanceIndex = -1;
			for (unsigned int i(0); i<robots_neutr.size();++i){
				if (!robots_neutr[i].getBoolGoal() && !is_particle_targeted(particules[j].getSquare().centre)){
					//calcul de la distance séparant particule-robot
					double robot_x = robots_neutr[i].getCircle().centre.x;
					double robot_y = robots_neutr[i].getCircle().centre.y;
					S2d vecteur_robot_part = {part_x - robot_x, part_y - robot_y};
					double distance =  s2d_norm(vecteur_robot_part);
					//calcul de l'écart angulaire
					Orient angle_robot = robots_neutr[i].getOrientation();
					Orient angle_particule = atan2(vecteur_robot_part.y,
							  vecteur_robot_part.x);
					Orient angle_diff = angle_particule - angle_robot;
					if (angle_diff > M_PI) angle_diff -= 2 * M_PI; //normalise l'angle
					if (angle_diff < -M_PI) angle_diff += 2 * M_PI;
					double temps_rotation = abs(angle_diff) / vrot_max;
					double temps_translation = distance / vtran_max;
					double temps_total = temps_rotation + temps_translation;
					if (temps_total < temps_min){
						temps_min = temps_total;
						minDistanceIndex = i;
					}
				}
			}
			if (minDistanceIndex !=-1){
				robots_neutr[minDistanceIndex].setGoal(particules[j].getSquare().centre);
				robots_neutr[minDistanceIndex].setBoolGoal(true);
				robots_neutr[minDistanceIndex].setOriented(false);
				robots_neutr[minDistanceIndex].setOriented2(false);
				robots_neutr[minDistanceIndex].setRentreMaison(false);
				particules[j].setDeja_ciblee(true);
				robots_neutr[minDistanceIndex].setParticuleCible(part_forme);
			}
		}
	}
}

void Simulation::robots_rep_cible(){
	
	int i(0);
	//pour chaque robot reparateur, trouver le robot neutraliseur en panne le plus 
	//proche et lui donner cette particule comme but
	for (auto& robot_neutr : robots_neutr){
		if (robot_neutr.getPanne() and !robot_neutr.getRepEnChemin()){
			double robot__neutr_x = robot_neutr.getCircle().centre.x;
			double robot_neutr_y = robot_neutr.getCircle().centre.y;
			double minDistance = 10000;
			int minDistanceIndex = -1;

			for (unsigned int i(0); i<robots_rep.size();++i){
				if (!robots_rep[i].getBoolGoal()){
					double robot_rep_x = robots_rep[i].getCircle().centre.x;
					double robot_rep_y = robots_rep[i].getCircle().centre.y;
					S2d vecteur_robot_part = {robot_rep_x - robot__neutr_x, robot_rep_y
											  - robot_neutr_y};
					double distance =  s2d_norm(vecteur_robot_part);
					if (distance < minDistance){
						minDistance = distance;
						minDistanceIndex = i;
					}
				}
			}
			//si première condition non remplie, le robot neutr. ne peut être sauvé
			int update_courant = rs.getNbUpdate() - robot_neutr.getKupdate();
			if (((minDistance < (max_update-update_courant)*vtran_max)) and 
												 (minDistanceIndex != -1)){
				robots_rep[minDistanceIndex].setGoal(robot_neutr.getCircle().centre);
				robots_rep[minDistanceIndex].setBoolGoal(true);
				//robots_rep[minDistanceIndex].setPanneIndex(i);
				robot_neutr.setRepEnchemin(true);
			} 
		}
		++i;
	}
}

void Simulation::creation_robots(){
	
	if (!verifie_si_spawn_vide()){
		//on ne crée aucun robot, si un robot neutraliseur se trouve dans le robot
		//spatial
		return;
	}
	
	int i_n = rs.getNbNs() + rs.getNbNd();
	unsigned int nbRobot_panne(rs.getNbNp());
	//creation de robots lorsque compteur de mises a jour est multiple de modulo_update
	if ((rs.getNbUpdate()%modulo_update)==0){
		//on crée un robot neutr. s'il y a plus de particules que de neutraliseurs
		if (rs.getNbNr() > 0){
			//IL DOIT AU MOINS Y AVOIR 1 ROBOT NEUTR PAR TYPE DE COORDINATION //A FAIRE
			//il doit rester des robots neutr. en reserve pour en créer
			if (particules.size() > robots_neutr.size()){
				Circle c1 = {r_neutraliseur,rs.getCircle().centre};
				cout<<"CREATION D'UN ROBOT DE TYPE : "<<i_n%3<<endl;
				R_neutraliseur rn(c1,i_n%3);
				robots_neutr.push_back(rn);
				rs.setNbNr(rs.getNbNr()-1);
				rs.setNbNs(rs.getNbNs()+1);
			}
			//on crée un robot rep. s'il y a plus de robots en panne que de robots rep
		} else if ((nbRobot_panne) > robots_rep.size()){
			//il doit rester des robots rep. en reserve pour en créer
			if (rs.getNbRr() > 0) {
				Circle c2 = {r_reparateur,rs.getCircle().centre};
				R_reparateur rr(c2);
				robots_rep.push_back(rr);
				rs.setNbRr(rs.getNbRr()-1);
				rs.setNbRs(rs.getNbRs()+1);
			}
		}
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

void Simulation::robot_bouge(){
	
	for (auto& robot_rep : robots_rep){
		robot_rep.move_rep_to(particules,
							   robots_neutr, 
							   robots_rep);
	}
	
	for (auto& robot_neutr : robots_neutr){
		if (!robot_neutr.getPanne()){
			if (robot_neutr.getC_n() == 0){
				robot_neutr.type0(particules,
								  robots_neutr, 
								  robots_rep);
			}
			
			if (robot_neutr.getC_n() == 1){
				robot_neutr.type1(particules,
								  robots_neutr, 
								  robots_rep);
			}
			if (robot_neutr.getC_n() == 2){
				robot_neutr.type2(particules,
								  robots_neutr, 
								  robots_rep);
			}
		}
	}   
}

void Simulation::panne_destroy(){
	for (int i = robots_neutr.size() - 1; i >= 0; --i){
		if (robots_neutr[i].getPanne()){
			unsigned int update_courant(rs.getNbUpdate()-robots_neutr[i].getKupdate());
			if ((update_courant)>= max_update){
				robots_neutr.erase(robots_neutr.begin()+i);
				rs.setNbNd(rs.getNbNd()+1);
				rs.setNbNp(rs.getNbNp()-1);
				rs.setNbNs(rs.getNbNs()-1);
			}
		}
	}
}

bool Simulation::detruire_particule() {
	
    bool target_destroyed = false;

    for (int i = particules.size() - 1; i >= 0; --i) {
        for (auto& robot_neutr : robots_neutr) {
            if (robot_neutr.isInCollisionWithParticle() and (robot_neutr.getCollisionParticleIndex() == i)) {
                //Verifie si le robot est bien aligné à la particule(bonne orientation)
                Orient desired_orientation = get_desired_orientation(robot_neutr.getCircle(), particules[i].getSquare());
                double angle_difference = std::abs(robot_neutr.getOrientation() - desired_orientation);
                robot_neutr.setInCollisionWithParticle(false);
                if (angle_difference < epsil_alignement) {
                    // Vérifie si la particule détruite correspond à sa cible
                    //originale
                    if (particules[i].getSquare().centre == robot_neutr.getGoal()) {
                        target_destroyed = true;
                    }
                    // Supprime la particule de son vector
                    particules.erase(particules.begin() + i);
                    robot_neutr.setBoolGoal(false);
                    robot_neutr.setCollisionParticleIndex(-1);
                    robot_neutr.setOriented(false);
                    robot_neutr.setOriented2(false);
                    robot_neutr.setCollisionOrientation(false);
                    break;
                }
            }
        }
    }
	//Si la particule cible n'est pas détruit, reset son attribut deja_ciblee
	
    if (!target_destroyed) {
        for (auto& particule : particules) {
            if (particule.getDeja_ciblee()) {
                particule.resetDeja_ciblee();
            }
        }
    }
    
    return target_destroyed;
}


void Simulation::robot_rentre_maison(){
	double rs_x = rs.getCircle().centre.x;
	double rs_y = rs.getCircle().centre.y;
	
	//si un robot neutraliseur n'a plus de particules à exterminer,
	//il rentre au robot spatial
	for (int i = robots_neutr.size() - 1; i >= 0; --i){
		if (!robots_neutr[i].getBoolGoal()){
			robots_neutr[i].setGoal(rs.getCircle().centre);
			robots_neutr[i].setRentreMaison(true);
		}
		double rn_goal_x = robots_neutr[i].getGoal().x;
		double rn_goal_y = robots_neutr[i].getGoal().y;
		Circle c1 = {0, robots_neutr[i].getCircle().centre};
		if (collision_cc(c1, rs.getCircle())){
			if ((rs_x == rn_goal_x) and (rs_y == rn_goal_y)) {
				robots_neutr.erase(robots_neutr.begin()+i);
				rs.setNbNr(rs.getNbNr()+1);
				rs.setNbNs(rs.getNbNs()-1);
			}
		}
	}
	//si un robot reparateur n'a plus de robots neutraliseur à réparer,
	//il rentre au robot spatial
	for (int i = robots_rep.size() - 1; i >= 0; --i){
		if (!robots_rep[i].getBoolGoal()){
			robots_rep[i].setGoal(rs.getCircle().centre);
		}
		double rr_goal_x = robots_rep[i].getGoal().x;
		double rr_goal_y = robots_rep[i].getGoal().y;
		Circle c2 = {0, robots_rep[i].getCircle().centre};
		if (collision_cc(c2, rs.getCircle())){
			if ((rs_x == rr_goal_x) and (rs_y == rr_goal_y)) {
				robots_rep.erase(robots_rep.begin()+i);
				rs.setNbRr(rs.getNbRr()+1);
				rs.setNbRs(rs.getNbRs()-1);
			}
		}
	}
}

void Simulation::verifie_SiCibleExiste(){
	for (auto& robot_neutr : robots_neutr){
		bool boolExiste = false;
		double rn_goal_x = robot_neutr.getGoal().x;
		double rn_goal_y = robot_neutr.getGoal().y;
		for (auto& particule : particules){
			double p_x = particule.getSquare().centre.x;
			double p_y = particule.getSquare().centre.y;
			if ((rn_goal_x == p_x) and (rn_goal_y == p_y)){
				boolExiste = true;
			}
		}
		if (!boolExiste){
			robot_neutr.setBoolGoal(false);
		}
	}
}

void Simulation::r_neutr_check(){
	//vérifie si le robot neutraliseur qu'un robot réparateur devait soigner est encore
	//là, s'il n'existe plus (max update), reset boolGoal du robot réparateur.
	//Cela arrive quand un robot réparateur n'arrive pas à temps au robot neutr. car
	//il a été stoppé par un obstacle.
	for (auto& robot_rep : robots_rep){
		bool boolExiste = false;
		double rr_goal_x = robot_rep.getGoal().x;
		double rr_goal_y = robot_rep.getGoal().y;
		for (auto& robot_neutr : robots_neutr){
			double rn_x = robot_neutr.getCircle().centre.x;
			double rn_y = robot_neutr.getCircle().centre.y;
			if ((rr_goal_x == rn_x) and (rr_goal_y == rn_y)){
				boolExiste = true;
			}
		}
		if (!boolExiste){
			robot_rep.setBoolGoal(false);
		}
	}
}

void Simulation::zone_a_risque_verifier(Particule& p){
	double d_particule = p.getLongueur();
	double d_zone = d_particule * risk_factor;
	Square zone_a_risque = {p.getSquare().centre, d_zone};
	
	for (auto& robot_neutr : robots_neutr){
		Circle c1 = robot_neutr.getCircle();
		// Vérifie si une partie du cercle se trouve dans la zone à risque
		if ((intersects(zone_a_risque, c1)) and (!robot_neutr.getPanne())){
			//si c'est le cas, le robot tombe en panne et le robot mémorise
			//nb_update dans sa variable k_update
			robot_neutr.setPanne(true);
			robot_neutr.setKupdate(rs.getNbUpdate());
		}
	}
}

bool Simulation::verifie_si_spawn_vide(){
	//renvoie true si aucun robot ne se trouve dans le robot spatial
	Circle rs_c = rs.getCircle();
	
	for (auto& robot_neutr : robots_neutr){
		if (collision_cc(rs_c, robot_neutr.getCircle())){
			return false;
		}
	}
	for (auto& robot_rep : robots_rep){
		if (collision_cc(rs_c, robot_rep.getCircle())){
			return false;
		}
	}
	
	return true;
}

bool Simulation::is_particle_targeted(const S2d& particle_center) {
    for (const auto& robot_neutr : robots_neutr) {
        if (robot_neutr.getBoolGoal() &&
            robot_neutr.getGoal().x == particle_center.x &&
            robot_neutr.getGoal().y == particle_center.y) {
            return true;
        }
    }
    return false;
}


  
