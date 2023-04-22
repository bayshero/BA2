/*!
  \file   robot.cc
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#include <iostream>
#include "shape.h"
#include "message.h"
#include "robot.h"
#include "particule.h"
#include "constantes.h"

using namespace std;

Robot::Robot(Circle c)
	: cercle(c) {}	

Robot::Robot() {}							

R_spatial::R_spatial(Circle c, int nbUpdate_, int nbNr_, int nbNs_, int nbNd_, 
					int NbNp_, int nbRr_, int nbRs_)
	: Robot(c), nbNr(nbNr_),nbNs(nbNs_), nbNd(nbNd_),nbRr(nbRr_),nbRs(nbRs_), 
		nbUpdate(nbUpdate_){
	nbN= nbNr_+nbNs_+nbNd_; nbR=nbRr_+nbRs_;
	if (!rs_in_domain()){
		cout<<message::spatial_robot_ouside(cercle.centre.x, cercle.centre.y);
	}
}

R_spatial::R_spatial() 
	: Robot() {}

R_reparateur::R_reparateur(Circle c)
	: Robot(c) {}

R_neutraliseur::R_neutraliseur(Circle c, double a, int k_update_, bool panne_)
	: Robot(c), orientation(a), k_update(k_update_) , panne(panne_) {}


Circle Robot::GetCircle() const {
    return cercle;
}

int R_neutraliseur::GetKupdate() const{
	return k_update;
}

int R_spatial::GetNbUpdate() const{
	return nbUpdate;
}

int R_spatial::GetNbRs() const{
	return nbRs;
}

int R_spatial::GetNbRr() const{
	return nbRr;
}

int R_spatial::GetNbNs() const{
	return nbNs;
}

int R_spatial::GetNbNp() const{
	return nbNp;
}

int R_spatial::GetNbNd() const{
	return nbNd;
}

int R_spatial::GetNbNr() const{
	return nbNr;
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
	if (collision_cs(cercle, p.GetSquare(),false)) {
		cout<<message::particle_robot_superposition(p.GetSquare().centre.x, 
													p.GetSquare().centre.y, 
													p.GetSquare().longueur_cote, 
													cercle.centre.x,
													cercle.centre.y, cercle.rayon);
		return false;											
	}
	return true;
}

//vérifie si une particule et un robot neutraliseur ne se superposent pas
bool R_neutraliseur::superposition_p_r_neutraliseur(const Particule& p) const{
	if (collision_cs(cercle, p.GetSquare(),false)) {
		cout<<message::particle_robot_superposition(p.GetSquare().centre.x, 
													p.GetSquare().centre.y,
													p.GetSquare().longueur_cote, 
													cercle.centre.x, 
													cercle.centre.y, cercle.rayon);
		return false;											
	}
	return true;
}

//vérifie que le k_update_panne d'un neutraliseur soit inférieur ou égal à nbUpdate 
bool R_neutraliseur::error_attribut(const R_spatial& rs) const{
	if(k_update>rs.GetNbUpdate()){
		cout<<message::invalid_k_update(cercle.centre.x, cercle.centre.y, 
										k_update, rs.GetNbUpdate());
		return false;								
	}
	return true;
}

//vérifie si une particule et le robot spatial ne se superposent pas
bool R_spatial::superposition_p_rs(const Particule& p) const{
	if (collision_cs(cercle, p.GetSquare(),false)) {
		cout<<message::particle_robot_superposition(p.GetSquare().centre.x, 
													p.GetSquare().centre.y, 
													p.GetSquare().longueur_cote, 
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


