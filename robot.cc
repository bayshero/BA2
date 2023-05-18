/*!
  \file   robot.cc
  \author Charly Guardia 50%, Gauthier de Mercey 50%
  \date   mai 2023
  \version 3
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
		oriented = false;
		oriented2 = false;
		in_collision_with_particle= false;
		in_collision_with_neutr_robot= false;
		in_collision_with_rep_robot= false;
		in_collision_with_zone= false;
		collision_orientation = false;
		rentre_maison = false;
	}

R_neutraliseur::R_neutraliseur(Circle c, int c_n)
	: Robot(c), orientation(0), k_update(0), panne(false), c_n(c_n), goal({0,0}), 
	  bool_goal(false), oriented(false), oriented2(false)
	{
	in_collision_with_particle= false;
	in_collision_with_neutr_robot= false;
	in_collision_with_rep_robot= false;
	in_collision_with_zone= false;
	collision_orientation = false;
	rentre_maison = false;
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
			   or in_collision_with_rep_robot or collision_orientation){
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
							   const std::vector<R_reparateur>& robots_rep){

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

//type de coordination 0
void R_neutraliseur::type0(const std::vector<Particule>& particules,
                           const std::vector<R_neutraliseur>& robots_neutr,
                           const std::vector<R_reparateur>& robots_rep) {
							   
	Orient goal_a = update_orientation(particules);

	if (is_orientation_correct0(goal_a)) {
		translate_towards_goal0(particules, robots_neutr, robots_rep);
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
            Orient desired_orientation = get_desired_orientation
										 (particules[i].getSquare());
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

bool R_neutraliseur::is_orientation_correct0(const Orient& goal_a) {
    return abs(orientation - goal_a) <= epsil_alignement;
}

void R_neutraliseur::translate_towards_goal0(const std::vector<Particule>& particules,
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
	update_collision_status0(new_circle, particules, robots_neutr, robots_rep);
	
    if (!in_collision_with_particle && !in_collision_with_neutr_robot &&
		!in_collision_with_rep_robot) {
        cercle.centre = new_position;
    }
}

void R_neutraliseur::update_collision_status0(const Circle& new_circle, 
									   const std::vector<Particule>& particules,
                                       const std::vector<R_neutraliseur>& robots_neutr,
                                       const std::vector<R_reparateur>& robots_rep) {
    in_collision_with_particle = false;
    in_collision_with_neutr_robot = false;
    in_collision_with_rep_robot = false;
  
	
    for (size_t i = 0; i < particules.size(); ++i) {
        if (collision_cs(cercle, particules[i].getSquare())) {
            in_collision_with_particle = true;
            collision_orientation = true;
            part_cible = particules[i].getSquare();
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


 
//type de coordination 1
void R_neutraliseur::type1(const std::vector<Particule>& particules,
						   const std::vector<R_neutraliseur>& robots_neutr, 
						   const std::vector<R_reparateur>& robots_rep){
								 
	S2d updated_pos_to_goal;
    updated_pos_to_goal.x = goal.x - cercle.centre.x;
    updated_pos_to_goal.y = goal.y - cercle.centre.y;
    Orient goal_a = Orient(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    Orient delta_a = goal_a - orientation;
    normalize_angle(delta_a);
	
	if ((!oriented) or (rentre_maison)){
		if (abs(delta_a) <= vrot_max * delta_t) {
			orientation = goal_a;
		} else {
			orientation += ((delta_a > 0) ? 1. : -1.) * vrot_max * delta_t;
		}
	}
	
    //Particule part = particules[goalParticleIndex];
    
    if (!oriented or rentre_maison){
		if (is_orientation_correct0(goal_a)) {
			//s'oriente pour aller vers la particule
			translate_towards_goal1(particules, robots_neutr, robots_rep);
		}
	} else {
		//le robot s'arrête juste avant la zone à risque avant de s'orienter 
		if (is_orientation_correct1(goal_a)){
			//dès qu'il a la bonne orientation, le robot peut avancer et détruire
			//la particule
			translate_towards_goal1(particules, robots_neutr, robots_rep);
		}
	}
}


bool R_neutraliseur::is_orientation_correct1(const Orient& goal_a){
	
	Orient desired_orientation = get_desired_orientation(part_cible);
	double delta_a = desired_orientation - orientation;
	normalize_angle(delta_a);
	
	if (abs(delta_a) <= vrot_max * delta_t) {
			orientation = desired_orientation;
	} else {
		orientation += ((delta_a > 0) ? 1. : -1.) * vrot_max * delta_t;
	}
		
	if (abs(delta_a) < epsil_alignement) {
		oriented2 = true;
		return true;
	}
	
	return false;
}

void R_neutraliseur::translate_towards_goal1(const std::vector<Particule>& particules,
                                       const std::vector<R_neutraliseur>& robots_neutr,
                                       const std::vector<R_reparateur>& robots_rep){
	S2d new_position;											 
	if (!oriented2 or rentre_maison){
		//avance vers le centre de la cible								 
		S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
		double norm(s2d_norm(pos_to_goal));
		if (norm <= (vtran_max * delta_t)) {
			new_position = goal;
		} else {
			new_position = s2d_add_scaled_vector(cercle.centre, pos_to_goal,
												(vtran_max * delta_t) / norm);
		}
    } else {
		//une fois aligné avec une des faces de la particule, le robot peut avancer
		//tout droit
		S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
		double norm(s2d_norm(pos_to_goal));
		S2d new_pos_to_goal;
		new_pos_to_goal.x = cos(orientation) *norm;
		new_pos_to_goal.y = sin(orientation) *norm;

		if (norm <= (vtran_max * delta_t)) {
			new_position = goal;
		} else {
			new_position = s2d_add_scaled_vector(cercle.centre, new_pos_to_goal,
												(vtran_max * delta_t) / norm);
		}
	}
    Circle new_circle = {cercle.rayon, new_position};
	update_collision_status1(new_circle, particules, robots_neutr, robots_rep);
	
	if (!in_collision_with_particle && !in_collision_with_neutr_robot && 
		!in_collision_with_rep_robot && !in_collision_with_zone) {
        cercle.centre = new_position;
    }				
}

void R_neutraliseur::update_collision_status1(const Circle& new_circle,
                                       const std::vector<Particule>& particules,
                                       const std::vector<R_neutraliseur>& robots_neutr,
                                       const std::vector<R_reparateur>& robots_rep) {
	in_collision_with_particle = in_collision_with_neutr_robot =
	in_collision_with_rep_robot = in_collision_with_zone = false;
	Square s1 = part_cible;
	s1.longueur_cote *= risk_factor;

	if (!oriented && collision_cs(new_circle, s1)) {
		in_collision_with_zone = oriented = true;
	}

	for (size_t i = 0; i < particules.size() && !in_collision_with_particle; ++i) {
		if (collision_cs(new_circle, particules[i].getSquare())) {
			in_collision_with_particle = oriented = collision_orientation = true;
			collisionParticleIndex = i;
			part_cible =  particules[i].getSquare();
		}
	}

	if (!in_collision_with_particle) {
		for (const auto& r : robots_neutr) {
			if (this != &r && collision_cc(new_circle, r.getCircle())) {
				in_collision_with_neutr_robot = true;
				break;
			}
		}
		for (const auto& r : robots_rep) {
			if (collision_cc(new_circle, r.getCircle())) {
				in_collision_with_rep_robot = true;
				break;
			}
		}
	}
}

void R_neutraliseur::type2(const std::vector<Particule>& particules,
						   const std::vector<R_neutraliseur>& robots_neutr, 
						   const std::vector<R_reparateur>& robots_rep){		   
	//type2
	Orient goal_a = update_orientation(particules);
	if (abs(orientation - goal_a) < M_PI/3){
		translate_towards_goal2(particules, robots_neutr, robots_rep);
	}
	
}

void R_neutraliseur::translate_towards_goal2(const std::vector<Particule>& particules,
                                       const std::vector<R_neutraliseur>& robots_neutr,
                                       const std::vector<R_reparateur>& robots_rep){
	S2d new_position;											 
	S2d pos_to_goal = {goal.x - cercle.centre.x, goal.y - cercle.centre.y};
	double norm(s2d_norm(pos_to_goal));
	S2d new_pos_to_goal;
	new_pos_to_goal.x = cos(orientation) *norm;
	new_pos_to_goal.y = sin(orientation) *norm;

	if (norm <= (vtran_max * delta_t)) {
		new_position = goal;
	} else {
		new_position = s2d_add_scaled_vector(cercle.centre, new_pos_to_goal,
											((vtran_max*4/4) * delta_t) / norm);
	}

    Circle new_circle = {cercle.rayon, new_position};
	update_collision_status0(new_circle, particules, robots_neutr, robots_rep);
	
	if (!in_collision_with_particle && !in_collision_with_neutr_robot && 
		!in_collision_with_rep_robot) {
        cercle.centre = new_position;
    }
    //le robot doit maintenant s'orienter une nouvelle fois						
}



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
        double angle_to_center = std::atan2(particle_center.y - position.y,
											particle_center.x - position.x);
        double angle_difference = std::abs(angle_to_center - orientation);
        return angle_difference < epsil_alignement;
    }
}

Orient R_neutraliseur::get_desired_orientation(const Square& particle_square) {
	S2d updated_pos_to_goal;
	updated_pos_to_goal.x = particle_square.centre.x - cercle.centre.x;
	updated_pos_to_goal.y = particle_square.centre.y - cercle.centre.y;
    double left_boundary = particle_square.centre.x - 
						   particle_square.longueur_cote / 2;
    double right_boundary = particle_square.centre.x + 
							particle_square.longueur_cote / 2;
    double top_boundary = particle_square.centre.y + 
						  particle_square.longueur_cote / 2;
    double bottom_boundary = particle_square.centre.y - 
							 particle_square.longueur_cote / 2;
    bool in_top_left_corner = cercle.centre.x < left_boundary &&
							  cercle.centre.y > top_boundary;
    bool in_top_right_corner = cercle.centre.x > right_boundary && 
							   cercle.centre.y > top_boundary;
    bool in_bottom_left_corner = cercle.centre.x < left_boundary && 
								 cercle.centre.y < bottom_boundary;
    bool in_bottom_right_corner = cercle.centre.x > right_boundary && 
								  cercle.centre.y < bottom_boundary;
    if (in_top_left_corner || in_top_right_corner || in_bottom_left_corner || 
		in_bottom_right_corner) {
        return Orient(atan2(updated_pos_to_goal.y, updated_pos_to_goal.x));
    }
    bool on_left = cercle.centre.x <= left_boundary;
    bool on_right = cercle.centre.x >= right_boundary;
    bool on_top = cercle.centre.y <= top_boundary;
    bool on_bottom = cercle.centre.y >= bottom_boundary;
    if (on_left) {
        return Orient(0);
    } else if (on_right) {
        return Orient(M_PI);
    } else if (on_bottom) {
        return Orient(-M_PI / 2);
    } else if (on_top) {
        return Orient(M_PI / 2);
    }
    return 0;
}

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
void R_spatial::setNbUpdate(int newNbUpdate){
	nbUpdate = newNbUpdate;
}

void R_spatial::setNbNp(int newNbNp){
	nbNp = newNbNp;
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

int R_neutraliseur::getC_n() const{
	return c_n;
}

void R_neutraliseur::setC_n(int newC_n){
	c_n = newC_n;
}

void R_neutraliseur::setOriented(bool newOriented){
	oriented = newOriented;
}

void R_neutraliseur::setOriented2(bool newOriented){
	oriented2 = newOriented;
}

void R_neutraliseur::setCollisionOrientation(bool newCollisionOrientation){
	collision_orientation= newCollisionOrientation;
}

void R_neutraliseur::setRentreMaison(bool newRentreMaison){
	rentre_maison = newRentreMaison;
}

double R_neutraliseur::getD_zone_a_eviter() const{
	return d_zone_a_eviter;
}

void R_neutraliseur::setD_zone_a_eviter(double newD_zone_a_eviter){
	d_zone_a_eviter = newD_zone_a_eviter;
}

void R_neutraliseur::setParticuleCible(Square newParticuleCible){
	part_cible = newParticuleCible;
}

bool R_neutraliseur::isInCollisionWithParticle() const {
	return in_collision_with_particle; 
}

bool R_neutraliseur::isInCollisionWithNeutrRobot() const { 
	return in_collision_with_neutr_robot; 
}

bool R_neutraliseur::isInCollisionWithRepRobot() const { 
	return in_collision_with_rep_robot; 
}

int R_neutraliseur::getCollisionParticleIndex() const { 
	return collisionParticleIndex; 
}

double R_neutraliseur::getAngleDeltaInCollision() const {
	return angle_data_in_collision; 
}
