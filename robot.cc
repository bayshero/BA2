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
void R_neutraliseur::draw_robot_neutr()
{
	if (panne){
		draw_circle_neutr(cercle, orientation,7);
	} else if (in_collision_with_particle or in_collision_with_neutr_robot 
								   or in_collision_with_rep_robot){
		draw_circle_neutr(cercle, orientation,6);
	} else {
		draw_circle_neutr(cercle, orientation,1);
	}
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
							   std::vector<R_neutraliseur>& robots_neutr, 
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
	for (auto& r : robots_neutr){
		if (collision_cc(new_circle, r.getCircle())){
			cout<<"hey"<<endl;
			if (r.getPanne()){
				bool_goal = false;
				r.setPanne(false);
				r.setRepEnchemin(false);
				
			}
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
/*
void R_neutraliseur::type0(const std::vector<Particule>& particules,
                           const std::vector<R_neutraliseur>& robots_neutr,
                           const std::vector<R_reparateur>& robots_rep){

	S2d updated_pos_to_goal;
    updated_pos_to_goal.x = goal.x - cercle.centre.x;
    updated_pos_to_goal.y = goal.y - cercle.centre.y;
    Orient goal_a = Orient(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    Orient delta_a = goal_a - orientation;
    if (delta_a > M_PI) delta_a -= 2 * M_PI; //normalise l'angle
	if (delta_a < -M_PI) delta_a += 2 * M_PI;
	if(abs(delta_a) <= vrot_max*delta_t){
		orientation = goal_a ; 
	}
	else{
		orientation += ((delta_a > 0) ?  1. : -1.)*vrot_max*delta_t ;
	}
	if (abs(orientation - goal_a) <= epsil_alignement){
    	orientation_correct = true;
	}
}
*/

void R_neutraliseur::type0(const std::vector<Particule>& particules,
                           const std::vector<R_neutraliseur>& robots_neutr,
                           const std::vector<R_reparateur>& robots_rep) {
    Orient goal_a = update_orientation(particules);

    if (is_orientation_correct(goal_a)) {
        translate_towards_goal(particules, robots_neutr, robots_rep);
    }
}


Orient R_neutraliseur::update_orientation(const std::vector<Particule>& particules) {
    S2d updated_pos_to_goal;
    updated_pos_to_goal.x = goal.x - cercle.centre.x;
    updated_pos_to_goal.y = goal.y - cercle.centre.y;
    Orient goal_a = Orient(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    Orient delta_a = goal_a - orientation;
    normalize_angle(delta_a);

    for (size_t i = 0; i < particules.size(); ++i) {
        if (collision_cs(cercle, particules[i].getSquare())) {
            in_collision_with_particle = true;
            Orient desired_orientation = get_desired_orientation(cercle, particules[i].getSquare());
            goal_a = desired_orientation;
            delta_a = desired_orientation - orientation;
            normalize_angle(delta_a);
            break;
        }
    }

    if (abs(delta_a) <= vrot_max * delta_t) {
        orientation = goal_a;
    } else {
        orientation += ((delta_a > 0) ? 1. : -1.) * vrot_max * delta_t;
    }
    return goal_a;
}

void R_neutraliseur::normalize_angle(Orient& delta_a) {
    if (delta_a > M_PI) delta_a -= 2 * M_PI; // Normalize the angle
    if (delta_a < -M_PI) delta_a += 2 * M_PI;
}

bool R_neutraliseur::is_orientation_correct(const Orient& goal_a) {
    return abs(orientation - goal_a) <= epsil_alignement;
}

void R_neutraliseur::translate_towards_goal(const std::vector<Particule>& particules,
                                            const std::vector<R_neutraliseur>& robots_neutr,
                                            const std::vector<R_reparateur>& robots_rep) {
    S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
    double norm(s2d_norm(pos_to_goal));
    S2d new_position;
    if (norm <= (vtran_max * delta_t)) {
        new_position = goal;
    } else {
        new_position = s2d_add_scaled_vector(cercle.centre, pos_to_goal,
                                             (vtran_max * delta_t) / norm);
    }
    Circle new_circle = {cercle.rayon, new_position};

    update_collision_status(new_circle, particules, robots_neutr, robots_rep);

    if (!in_collision_with_particle && !in_collision_with_neutr_robot && !in_collision_with_rep_robot) {
        cercle.centre = new_position;
    }
}

void R_neutraliseur::update_collision_status(const Circle& new_circle, const std::vector<Particule>& particules,
                                             const std::vector<R_neutraliseur>& robots_neutr,
                                             const std::vector<R_reparateur>& robots_rep) {
    in_collision_with_particle = false;
    in_collision_with_neutr_robot = false;
    in_collision_with_rep_robot = false;

    for (size_t i = 0; i < particules.size(); ++i) {
        if (collision_cs(cercle, particules[i].getSquare())) {
            in_collision_with_particle = true;
            collisionParticleIndex = i;
            break;
        }
    }

    if (!in_collision_with_particle) {
        for (const auto& r : robots_neutr) {
            if (this != &r && collision_cc(new_circle, r.getCircle())) {
                in_collision_with_neutr_robot = true;
                return;
            }
        }
        for (const auto& r : robots_rep) {
            if (collision_cc(new_circle, r.getCircle())) {
                in_collision_with_rep_robot = true;
                return;
            }
        }
    }
}






/*

// se déplace selon son orientation courante avec limitation supplémentaire
// puis s'oriente au plus de max_delta_rt en direction de updated_pos_to_goal
void R_neutraliseur::type2(const std::vector<Particule>& particules,
                           const std::vector<R_neutraliseur>& robots_neutr,
                           const std::vector<R_reparateur>& robots_rep) {
    // Update position with movement along current orientation
    S2d init_pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
    S2d travel_dir = {cos(orientation), sin(orientation)}; // Unit vector along Xrobot
    double proj_goal = s2d_prod_scal(init_pos_to_goal, travel_dir);

    if(abs(proj_goal) > vtran_max*delta_t) {
        proj_goal = ((proj_goal > 0) ? 1 : -1)*vtran_max*delta_t;
    }

    S2d new_position = s2d_add_scaled_vector(cercle.centre, travel_dir, proj_goal);
    Circle new_circle = {cercle.rayon, new_position};

    // Check for collisions before updating position
    for (const auto& p : particules){
        if (collision_cs(new_circle, p.getSquare())){
            in_collision_with_particle = true;
            collisionParticleIndex = &p - &particules[0];
            break;
        }
    }
    for (const auto& r : robots_neutr){
        if (this != &r && collision_cc(new_circle, r.getCircle())){
            in_collision_with_neutr_robot = true;
            return;
        }
    }
    for (const auto& r : robots_rep){
        if (collision_cc(new_circle, r.getCircle())){
            in_collision_with_rep_robot = true;
            return;
        }
    }

    if (!in_collision_with_particle && !in_collision_with_neutr_robot && !in_collision_with_rep_robot) {
        cercle.centre = new_position;
    }

    // Update orientation
    S2d updated_pos_to_goal;
    updated_pos_to_goal.x = goal.x - cercle.centre.x;
    updated_pos_to_goal.y = goal.y - cercle.centre.y;
    Orient goal_a(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    Orient delta_a(goal_a - orientation);

    if (abs(delta_a) <= vrot_max * delta_t) {
        orientation = goal_a;
    } else {
        orientation += ((delta_a > 0) ? 1. : -1.) * vrot_max * delta_t;
    }

    // Particle destruction
    if (in_collision_with_particle) {
        Orient desired_orientation = get_desired_orientation(cercle, particules[collisionParticleIndex].getSquare());
        double angle_difference = std::abs(orientation - desired_orientation);
        if (angle_difference < epsil_alignement) {
            particules.erase(particules.begin() + collisionParticleIndex);
            setBoolGoal(false);
            collisionParticleIndex = -1;
        }
    }
}



*/



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

void R_neutraliseur::setCollisionParticleIndex(int index){ 
	collisionParticleIndex = index;
}

void R_neutraliseur::setInCollisionWithParticle(bool state){
	in_collision_with_particle = state;
}

void R_neutraliseur::setPanne(bool newPanne){
	panne = newPanne;
}

void R_neutraliseur::setKupdate(int newKupdate){
	k_update = newKupdate;
}

int R_reparateur::getPanneIndex() const{
	return panne_index;
}

void R_reparateur::setPanneIndex(int newIndex){
	panne_index = newIndex;
}

bool R_neutraliseur::getRepEnChemin() const{
	return r_rep_en_chemin;
}

void R_neutraliseur::setRepEnchemin(bool newRepEnChemin){
	r_rep_en_chemin = newRepEnChemin;
}
/*
bool R_neutraliseur::isAlignedWithParticle(const S2d& particle_center) const {
    S2d position = cercle.centre;

    // Check if the robot is on a side of the particle
    if (position.x == particle_center.x) {
        // Robot is on the left or right side
        double angle_difference = std::abs(orientation);
        return angle_difference < epsil_alignement;
    } else if (position.y == particle_center.y) {
        // Robot is on the top or bottom side
        double angle_difference = std::abs(std::abs(orientation) - M_PI / 2);
        return angle_difference < epsil_alignement;
    } else {
        // Robot is on a corner of the particle
        double angle_to_center = std::atan2(particle_center.y - position.y, particle_center.x - position.x);
        double angle_difference = std::abs(angle_to_center - orientation);
        return angle_difference < epsil_alignement;
    }
}
*/

Orient get_desired_orientation(const Circle& robot_circle, const Square& particle_square) {
	S2d updated_pos_to_goal;
	updated_pos_to_goal.x = particle_square.centre.x - robot_circle.centre.x;
	updated_pos_to_goal.y = particle_square.centre.y - robot_circle.centre.y;

	
    double left_boundary = particle_square.centre.x - particle_square.longueur_cote / 2;
    double right_boundary = particle_square.centre.x + particle_square.longueur_cote / 2;
    double top_boundary = particle_square.centre.y + particle_square.longueur_cote / 2;
    double bottom_boundary = particle_square.centre.y - particle_square.longueur_cote / 2;

    // Check if the robot is in one of the corners
    bool in_top_left_corner = robot_circle.centre.x < left_boundary && robot_circle.centre.y > top_boundary;
    bool in_top_right_corner = robot_circle.centre.x > right_boundary && robot_circle.centre.y > top_boundary;
    bool in_bottom_left_corner = robot_circle.centre.x < left_boundary && robot_circle.centre.y < bottom_boundary;
    bool in_bottom_right_corner = robot_circle.centre.x > right_boundary && robot_circle.centre.y < bottom_boundary;

    if (in_top_left_corner || in_top_right_corner || in_bottom_left_corner || in_bottom_right_corner) {
        return Orient(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    }

    bool on_left = robot_circle.centre.x <= left_boundary;
    bool on_right = robot_circle.centre.x >= right_boundary;
    bool on_top = robot_circle.centre.y <= top_boundary;
    bool on_bottom = robot_circle.centre.y >= bottom_boundary;

    if (on_left) {
        return Orient(0);
    } else if (on_right) {
        return Orient(M_PI);
    } else if (on_bottom) {
        return Orient(-M_PI / 2);
    } else if (on_top) {
        return Orient(M_PI / 2);
    }
}






