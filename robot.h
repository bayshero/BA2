/*!
  \file   robot.h
  \author Charly Guardia 95%, Gauthier de Mercey 5%
  \date   avril 2023
  \version 2
*/

#ifndef ROBOT_H
#define ROBOT_H
#include "particule.h"
#include "shape.h"
#include <vector>
#include <memory>

typedef double Orient;       // en rd

class Robot{
protected :
	Circle cercle;
public:
	Robot(Circle c);
	Robot();
	Circle getCircle() const;
	bool superposition_r_neutre_rep(const Robot& r) const;
};

class R_reparateur;

class R_spatial: public Robot {
private :
	int nbUpdate; 		//compteur de l'avancement des mise à jours
	int nbN;			//nombre de robots neutraliseurs (total)
	int nbNr;  			//nombre de robots neutraliseurs en réserve
	int nbNs;			//nombre de robots neutraliseurs en service
	int nbNd;			//nombre de robots neutraliseur détruits
	int nbNp;			//nombre de robots neutraliseurs en panne
	int nbR;			//nombre de robots réparateurs (total)
	int nbRr;			//nombre de robots réparateurs en reserve
	int nbRs;			//nombre de robots réparateurs en service
	bool error_domain;
public :
	R_spatial();
	R_spatial(Circle c, int nbUpdate, int nbNr, int nbNs, int nbNd, int nbRr,int nbRs);
	
	int getNbUpdate() const;
	int getNbRs() const;
	int getNbRr() const;
	int getNbNs() const;
	int getNbNp() const;
	int getNbNd() const;
	int getNbNr() const;
	bool getError_domain() const;
	
	bool superposition_p_rs(const Particule& p) const;
	bool rs_in_domain() const;
	
	std::string get_as_string();
	
	void setNbUpdate(int newNbUpdate);
	void setNbNp(int newNbNp);
	void setNbNr(int newNbNr);
	void setNbRr(int newNbRr);
	void setNbNs(int newNbNs);
	void setNbRs(int newNbRs);
	void setNbNd(int newNbNd);
	
	
	void delete_rs();
	
	void draw_robot_spatial();
};


class R_neutraliseur : public Robot {
private:
	double orientation;		//angle entre (-pi et pi)
	int k_update;
	bool panne;
	int c_n;
	S2d goal;
	bool bool_goal;
	bool in_collision_with_particle = false;
    bool in_collision_with_neutr_robot = false;
    bool in_collision_with_rep_robot = false;
    int collisionParticleIndex;
	double angle_data_in_collision;

public:
	R_neutraliseur(Circle c, double a, int k_update_, bool panne_, int c_n_);
	R_neutraliseur(Circle c);
	int getKupdate() const;
	int getPanne() const;
	S2d getGoal() const;
	bool getBoolGoal() const;
	Orient getOrientation() const;
	
	void setGoal(S2d newGoal);
	void setBoolGoal(bool boolGoal);
	void setCollisionParticleIndex(int index);
	void setInCollisionWithParticle(bool state);
	
	bool isInCollisionWithParticle() const { return in_collision_with_particle; }//CHANGER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool isInCollisionWithNeutrRobot() const { return in_collision_with_neutr_robot; }
    bool isInCollisionWithRepRobot() const { return in_collision_with_rep_robot; }
	int getCollisionParticleIndex() const { return collisionParticleIndex; }
	double getAngleDeltaInCollision() const {return angle_data_in_collision; }
	
	bool superposition_r_neutre(const R_neutraliseur& r) const;
	bool superposition_p_r_neutraliseur(const Particule& r) const;
	bool error_attribut(const R_spatial& rs) const;
	
	std::string get_as_string();
	void draw_robot_neutr();
	
	void move_neutr_to(const std::vector<Particule>& particules,
					   const std::vector<R_neutraliseur>& robots_neutr,
					   const std::vector<R_reparateur>& robots_rep);
					   
	void move_neutr_to_type1(const std::vector<Particule>& particules,
							 const std::vector<R_neutraliseur>& robots_neutr, 
							 const std::vector<R_reparateur>& robots_rep);

};

class R_reparateur : public Robot {
private :
	S2d goal;
	bool bool_goal;
public :
	R_reparateur(Circle c);
	bool superposition_r_reparateur(const R_reparateur& r) const;
	bool superposition_p_r_reparateur(const Particule& r) const;
	
	std::string get_as_string();
	void draw_robot_rep();
	void move_rep_to(const std::vector<Particule>& particules,
					 const std::vector<R_neutraliseur>& robots_neutr, 
					 const std::vector<R_reparateur>& robots_rep);
					 
	S2d getGoal() const;
	bool getBoolGoal() const;
	
	void setGoal(S2d newGoal);
	void setBoolGoal(bool boolGoal);
};

S2d s2d_scale(const S2d& v, double scalar);
S2d s2d_subtract(const S2d& a, const S2d& b);

#endif	
		
	
