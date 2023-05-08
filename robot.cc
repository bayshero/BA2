/*!
  \file   robot.cc
  \author Charly Guardia 90%, Gauthier de Mercey 10%
  \date   avril 2023
  \version 2
*/

#include <iostream>
#include "shape.h"
#include "message.h"
#include "robot.h"
#include "particule.h"
#include "constantes.h"
#include <cmath>

using namespace std;

Robot::Robot(Circle c)
	: cercle(c) {}	

Robot::Robot() {}							

R_spatial::R_spatial(Circle c, int nbUpdate_, int nbNr_, int nbNs_, int nbNd_, 
					 int nbRr_, int nbRs_)
	: Robot(c), nbUpdate(nbUpdate_), nbNr(nbNr_), nbNs(nbNs_), nbNd(nbNd_), 
				nbRr(nbRr_), nbRs(nbRs_)
{
	nbN= nbNr_+nbNs_+nbNd_; nbR=nbRr_+nbRs_;
	if (!rs_in_domain()){
		cout<<message::spatial_robot_ouside(cercle.centre.x, cercle.centre.y);
		error_domain = false;
	} else {
		error_domain = true;
	}
}

R_spatial::R_spatial() 
	: Robot() {}

R_reparateur::R_reparateur(Circle c)
	: Robot(c), goal({0,0}), bool_goal(false) {}

R_neutraliseur::R_neutraliseur(Circle c, double a, int k_update_, bool panne_,int c_n_)
	: Robot(c), orientation(a), k_update(k_update_) , panne(panne_), c_n(c_n_) 
	{
		bool_goal = false;
	}

R_neutraliseur::R_neutraliseur(Circle c)
	: Robot(c), orientation(0), k_update(0), panne(false), c_n(0), goal({0,0}), 
	  bool_goal(false) {}

Circle Robot::getCircle() const {
    return cercle;
}

int R_neutraliseur::getKupdate() const{
	return k_update;
}

int R_neutraliseur::getPanne() const{
	return panne;
}

int R_spatial::getNbUpdate() const{
	return nbUpdate;
}

int R_spatial::getNbRs() const{
	return nbRs;
}

int R_spatial::getNbRr() const{
	return nbRr;
}

int R_spatial::getNbNs() const{
	return nbNs;
}

int R_spatial::getNbNp() const{
	return nbNp;
}

int R_spatial::getNbNd() const{
	return nbNd;
}

int R_spatial::getNbNr() const{
	return nbNr;
}

bool R_spatial::getError_domain() const{
	return error_domain;
}

//vérifie que deux robots neutraliseur ne se superposent pas
bool R_neutraliseur::superposition_r_neutre(const R_neutraliseur& r) const{
	if (collision_cc(cercle, r.cercle,false)) {
		cout << message::neutralizers_superposition(cercle.centre.x, cercle.centre.y, 
													r.cercle.centre.x, 
													r.cercle.centre.y);
		return false;											
	}
	return true;
}

//vérifie que deux robots réparateurs ne se superposent pas
bool R_reparateur::superposition_r_reparateur(const R_reparateur& r) const{
	if (collision_cc(cercle, r.cercle, false)) {
		cout << message::repairers_superposition(cercle.centre.x, cercle.centre.y, 
												r.cercle.centre.x, r.cercle.centre.y);
		return false;										
	}
	return true;
}

//vérifie qu'un robot neutralisateur et réparateur ne superposent pas
bool Robot::superposition_r_neutre_rep(const Robot& r) const{
	if (collision_cc(cercle, r.cercle, false)) {
		cout<<message::repairer_neutralizer_superposition(cercle.centre.x, 
														cercle.centre.y, 
														r.cercle.centre.x, 
														r.cercle.centre.y);
		return false;												
	}
	return true;
}

//vérifie si une particule et un robot réparateur ne se superposent pas
bool R_reparateur::superposition_p_r_reparateur(const Particule& p) const{
	if (collision_cs(cercle, p.getSquare(),false)) {
		cout<<message::particle_robot_superposition(p.getSquare().centre.x, 
													p.getSquare().centre.y, 
													p.getSquare().longueur_cote, 
													cercle.centre.x,
													cercle.centre.y, cercle.rayon);
		return false;											
	}
	return true;
}

//vérifie si une particule et un robot neutraliseur ne se superposent pas
bool R_neutraliseur::superposition_p_r_neutraliseur(const Particule& p) const{
	if (collision_cs(cercle, p.getSquare(),false)) {
		cout<<message::particle_robot_superposition(p.getSquare().centre.x, 
													p.getSquare().centre.y,
													p.getSquare().longueur_cote, 
													cercle.centre.x, 
													cercle.centre.y, cercle.rayon);
		return false;											
	}
	return true;
}

//vérifie que le k_update_panne d'un neutraliseur soit inférieur ou égal à nbUpdate 
bool R_neutraliseur::error_attribut(const R_spatial& rs) const{
	if(k_update>rs.getNbUpdate()){
		cout<<message::invalid_k_update(cercle.centre.x, cercle.centre.y, 
										k_update, rs.getNbUpdate());
		return false;								
	}
	return true;
}

//vérifie si une particule et le robot spatial ne se superposent pas
bool R_spatial::superposition_p_rs(const Particule& p) const{
	if (collision_cs(cercle, p.getSquare(),false)) {
		cout<<message::particle_robot_superposition(p.getSquare().centre.x, 
													p.getSquare().centre.y, 
													p.getSquare().longueur_cote, 
													cercle.centre.x, 
													cercle.centre.y, cercle.rayon);
		return false;										
	}
	return true;
}

//vérifie que le robot spatial soit contenu dans le domaine
bool R_spatial::rs_in_domain() const{
	S2d centre = cercle.centre;
	double x = centre.x;
	double y = centre.y;
	double r = cercle.rayon;
	if ((x - r >= -dmax) && (x + r <= dmax) && (y - r >= -dmax) && (y + r <= dmax)){
		return true;
	}else{
		return false;
	}
}

//écrit une ligne avec les données du robot spatial
string R_spatial::get_as_string() {
	string line = to_string(cercle.centre.x) + " " + to_string(cercle.centre.y) 
											 + " " + to_string(nbUpdate)
											 + " " + to_string(nbNr)
											 + " " + to_string(nbNs)
											 + " " + to_string(nbNd)
											 + " " + to_string(nbRr)
											 + " " + to_string(nbRs);
	return line;
}

//écrit une ligne avec les données du robot neutraliseur
string R_neutraliseur::get_as_string(){
	string panne_str;
	if (panne){
		panne_str = "true";
	} else {
		panne_str = "false";
	}
	string line = to_string(cercle.centre.x) + " " + to_string(cercle.centre.y)
											 + " " + to_string(orientation)
											 + " " + to_string(c_n)
											 + " " + panne_str
											 + " " + to_string(k_update);
	
	return line;
}

//écrit une ligne avec les données du robot réparateur
string R_reparateur::get_as_string(){
	string line = to_string(cercle.centre.x) + " " + to_string(cercle.centre.y);
	
	return line;
}

void R_spatial::setNbUpdate(int newNbUpdate){
	nbUpdate = newNbUpdate;
}

void R_spatial::setNbNp(int newNbNp){
	nbNp = newNbNp;
}

//appelle la fonction de dessin du robot réparateur
void R_reparateur::draw_robot_rep(){
	draw_circle_rep(cercle);
}

//appelle la fonction de dessin du robot neutraliseur
void R_neutraliseur::draw_robot_neutr(){
    draw_circle_neutr(cercle, orientation);
}

//appelle la fonction de dessin du robot spatial
void R_spatial::draw_robot_spatial(){
	draw_circle_spatial(cercle);
}

//réinitialise les données stockées dans le robot spatial
void R_spatial::delete_rs(){
	S2d pos = {0,0};
	cercle = {0,pos};
	nbUpdate=0;		
	nbN=0;	
	nbNr=0;
	nbNs=0;
	nbNd=0;
	nbNp=0;
	nbR=0;
	nbRr=0;
	nbRs=0;
	error_domain=true;
}

void R_reparateur::move_rep_to(const std::vector<Particule>& particules,
							   const std::vector<R_neutraliseur>& robots_neutr, 
							   const std::vector<R_reparateur>& robots_rep)
{

	S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y} ;
	double norm(s2d_norm(pos_to_goal));
	S2d new_position;
	if(norm <= (vtran_max*delta_t)){
		new_position = goal;
	}
	else {
		new_position = s2d_add_scaled_vector(cercle.centre, pos_to_goal,
											 (vtran_max*delta_t)/norm);
	}
	Circle new_circle = {cercle.rayon, new_position};
	
	for (const auto& p : particules){
		if (collision_cs(new_circle, p.getSquare())){
			return;
		}
	}
	for (const auto& r : robots_neutr){
		if (collision_cc(new_circle, r.getCircle())){
			return;
		}
	}
	for (const auto& r : robots_rep){
		if (this != &r && collision_cc(new_circle, r.getCircle())){
			return;
		}
	}
	cercle.centre = new_position; 
}


// se déplace selon son orientation courante avec limitation supplémentaire
// puis s'oriente au plus de max_delta_rt en direction de updated_pos_to_goal
void R_neutraliseur::move_neutr_to(const std::vector<Particule>& particules,
								   const std::vector<R_neutraliseur>& robots_neutr, 
								   const std::vector<R_reparateur>& robots_rep)
{
	//goal = {100, 100}; //test
	
	// mise à jour de la position avec un déplacement selon l'orientation courante
	S2d init_pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
	S2d travel_dir = {cos(orientation), sin(orientation)}; //vecteur unitaire selon Xrobot
	double proj_goal = s2d_prod_scal(init_pos_to_goal, travel_dir);

    if(abs(proj_goal) > vtran_max*delta_t) {
		proj_goal = ((proj_goal > 0) ? 1 : -1)*vtran_max*delta_t;
    }

    
    S2d new_position = s2d_add_scaled_vector(cercle.centre, travel_dir, proj_goal);
    Circle new_circle = {cercle.rayon, new_position};
    
    for (const auto& p : particules){
		if (collision_cs(new_circle, p.getSquare())){
			return;
		}
	}
	for (const auto& r : robots_neutr){
		if (this != &r && collision_cc(new_circle, r.getCircle())){
			return;
		}
	}
	for (const auto& r : robots_rep){
		if (collision_cc(new_circle, r.getCircle())){
			return;
		}
	}
	
	cercle.centre = new_position;
	
	// mise à jour de l'orientation
	S2d updated_pos_to_goal;
	updated_pos_to_goal.x = goal.x - cercle.centre.x;
	updated_pos_to_goal.y = goal.y - cercle.centre.y;
	Orient goal_a(atan2(updated_pos_to_goal.y,updated_pos_to_goal.x));
	Orient delta_a(goal_a - orientation);
	
	if(abs(delta_a) <= vrot_max*delta_t){
		orientation = goal_a ; 
	}
	else{
		orientation += ((delta_a > 0) ?  1. : -1.)*vrot_max*delta_t ;
	}
	
}

void R_neutraliseur::move_neutr_to_type1(const std::vector<Particule>& particules,
                                         const std::vector<R_neutraliseur>& robots_neutr,
                                         const std::vector<R_reparateur>& robots_rep){
    //goal = {100,100}; //test
    // mise à jour de l'orientation
    S2d updated_pos_to_goal;
    updated_pos_to_goal.x = goal.x - cercle.centre.x;
    updated_pos_to_goal.y = goal.y - cercle.centre.y;
    Orient goal_a(atan2(updated_pos_to_goal.y,updated_pos_to_goal.x));
    Orient delta_a(goal_a - orientation);
    if (delta_a > M_PI) delta_a -= 2 * M_PI; //normalise l'angle
	if (delta_a < -M_PI) delta_a += 2 * M_PI;

    bool orientation_correct = false;

    if(abs(delta_a) <= vrot_max*delta_t){
        orientation = goal_a ;
        if (orientation > M_PI) orientation -= 2 * M_PI; //normalise l'angle
		if (orientation < -M_PI) orientation += 2 * M_PI;
    }
    else{
        orientation += ((delta_a > 0) ?  1. : -1.)*vrot_max*delta_t ;
        if (orientation > M_PI) orientation -= 2 * M_PI; //normalise l'angle
		if (orientation < -M_PI) orientation += 2 * M_PI;
    }

    if (abs(orientation - goal_a) <= epsil_alignement){
        orientation_correct = true;
    }

    if (orientation_correct){
        //translation
        S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y} ;
        double norm(s2d_norm(pos_to_goal));
        S2d new_position;
        if(norm <= (vtran_max*delta_t)){
            new_position = goal;
        }
        else {
            new_position = s2d_add_scaled_vector(cercle.centre, pos_to_goal,
                                                 (vtran_max*delta_t)/norm);
        }
        Circle new_circle = {cercle.rayon, new_position};

		in_collision_with_particle = false;
        for (size_t i = 0; i < particules.size(); ++i) {
            if (collision_cs(new_circle, particules[i].getSquare())) {
                in_collision_with_particle = true;
                collisionParticleIndex = i;

                // Calculate the point of contact between the robot and the particle
                S2d particle_center = {
                    particules[i].getSquare().centre.x,
                    particules[i].getSquare().centre.y
                };
                S2d direction_to_robot = {new_circle.centre.x - particle_center.x,
                                          new_circle.centre.y - particle_center.y};
                double distance_to_robot = s2d_norm(direction_to_robot);

                double overlapping_distance = cercle.rayon + particules[i].getSquare().longueur_cote / 2.0 - distance_to_robot;
                S2d adjustment = s2d_scale(direction_to_robot, overlapping_distance / distance_to_robot);

                cercle.centre = s2d_subtract(new_circle.centre, adjustment);

                return;
            }
        }
        

        in_collision_with_neutr_robot = false;
        for (const auto& r : robots_neutr){
            if (this != &r && collision_cc(new_circle, r.getCircle())){
                in_collision_with_neutr_robot = true;
                break;
            }
        }
        if (in_collision_with_neutr_robot) return;

        in_collision_with_rep_robot = false;
        for (const auto& r : robots_rep){
            if (collision_cc(new_circle, r.getCircle())){
                in_collision_with_rep_robot = true;
                break;
            }
        }
        if (in_collision_with_rep_robot) return;
        cercle.centre = new_position;
    }
}

S2d s2d_scale(const S2d& v, double scalar) {
    return {v.x * scalar, v.y * scalar};
}

S2d s2d_subtract(const S2d& a, const S2d& b) {
    return {a.x - b.x, a.y - b.y};
}

S2d R_neutraliseur::getGoal() const{
	return goal;
}

void R_neutraliseur::setGoal(S2d newGoal){
	goal = newGoal;
}

bool R_neutraliseur::getBoolGoal() const{
	return bool_goal;
}


void R_neutraliseur::setBoolGoal(bool boolGoal){
	bool_goal = boolGoal;
}

S2d R_reparateur::getGoal() const{
	return goal;
}

void R_reparateur::setBoolGoal(bool boolGoal){
	bool_goal = boolGoal;
}

void R_reparateur::setGoal(S2d newGoal){
	goal = newGoal;
}

bool R_reparateur::getBoolGoal() const{
	return bool_goal;
}

Orient R_neutraliseur::getOrientation() const{
	return orientation;
}

void R_spatial::setNbNr(int newNbNr){
	nbNr = newNbNr;
}

void R_spatial::setNbRr(int newNbRr){
	nbRr = newNbRr;
}

void R_spatial::setNbNs(int newNbNs){
	nbNs = newNbNs;
}

void R_spatial::setNbRs(int newNbRs){
	nbRs = newNbRs;
}

void R_spatial::setNbNd(int newNbNd){
	nbNd = newNbNd;
}

