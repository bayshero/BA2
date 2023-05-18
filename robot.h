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
	double d_zone_a_eviter;
	bool bool_goal;
	bool in_collision_with_particle;
    bool in_collision_with_neutr_robot;
    bool in_collision_with_rep_robot;
    bool in_collision_with_zone;
	bool collision_orientation;
    int collisionParticleIndex;
	double angle_data_in_collision;
	bool r_rep_en_chemin;
	bool oriented;
	bool oriented2;
	bool rentre_maison;
	Square part_cible;

public:
	R_neutraliseur(Circle c, double a, int k_update_, bool panne_, int c_n_);
	R_neutraliseur(Circle c, int c_n);
	int getKupdate() const;
	int getPanne() const;
	S2d getGoal() const;
	bool getBoolGoal() const;
	Orient getOrientation() const;
	bool getRepEnChemin() const;
	int getC_n() const;
	int getGoalParticleIndex() const;
	bool getOriented() const{return oriented;}
	bool getOriented2() const{return oriented2;}
	double getD_zone_a_eviter() const;
	
	void setGoal(S2d newGoal);
	void setBoolGoal(bool boolGoal);
	void setPanne(bool newPanne);
	void setKupdate(int newKupdate);
	void setCollisionParticleIndex(int index);
	void setInCollisionWithParticle(bool state);
	void setRepEnchemin(bool newRepEnChemin);
	void setC_n(int newC_n);
	void setGoalParticleIndex(int newGoalParticleIndex);
	void setOriented(bool newOriented);
	void setOriented2(bool newOriented2);
	void setCollisionOrientation(bool newCollisionOrientation);
	void setRentreMaison(bool newRentreMaison);
	void setD_zone_a_eviter(double newD_zone_a_eviter);
	void setParticuleCible(Square newParticuleCible);
	
	bool isInCollisionWithParticle() const { return in_collision_with_particle; }//CHANGER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    bool isInCollisionWithNeutrRobot() const { return in_collision_with_neutr_robot; }
    bool isInCollisionWithRepRobot() const { return in_collision_with_rep_robot; }
	int getCollisionParticleIndex() const { return collisionParticleIndex; }
	double getAngleDeltaInCollision() const {return angle_data_in_collision; }
	bool isAlignedWithParticle(const S2d& particle_center) const;
	
	bool superposition_r_neutre(const R_neutraliseur& r) const;
	bool superposition_p_r_neutraliseur(const Particule& r) const;
	bool error_attribut(const R_spatial& rs) const;
	
	std::string get_as_string();
	void draw_robot_neutr();
	
	void move_neutr_to(const std::vector<Particule>& particules,
					   const std::vector<R_neutraliseur>& robots_neutr,
					   const std::vector<R_reparateur>& robots_rep);
	
	void normalize_angle(Orient& delta_a);
	
	//type de coordination 0				   
	void type0(const std::vector<Particule>& particules,
			   const std::vector<R_neutraliseur>& robots_neutr, 
			   const std::vector<R_reparateur>& robots_rep);
						 
	Orient update_orientation(const std::vector<Particule>& particules);
	bool is_orientation_correct0(const Orient& goal_a);
	
	void update_collision_status0(const Circle& new_circle, const std::vector<Particule>& particules,
                                 const std::vector<R_neutraliseur>& robots_neutr,
                                 const std::vector<R_reparateur>& robots_rep);
                                 
	void translate_towards_goal0(const std::vector<Particule>& particules,
                                 const std::vector<R_neutraliseur>& robots_neutr,
                                 const std::vector<R_reparateur>& robots_rep);
    //type de coordination 1                           
    void type1(const std::vector<Particule>& particules,
			   const std::vector<R_neutraliseur>& robots_neutr, 
			   const std::vector<R_reparateur>& robots_rep);	
	void update_collision_status1(const Circle& new_circle, const std::vector<Particule>& particules,
                                  const std::vector<R_neutraliseur>& robots_neutr,
                                  const std::vector<R_reparateur>& robots_rep);
                                 					                              
    bool is_orientation_correct1(const Orient& goal_a);      
                          
	void translate_towards_goal1(const std::vector<Particule>& particules,
                                 const std::vector<R_neutraliseur>& robots_neutr,
                                 const std::vector<R_reparateur>& robots_rep);
                                 
	//type de coordination 2
	void type2(const std::vector<Particule>& particules,
			   const std::vector<R_neutraliseur>& robots_neutr, 
			   const std::vector<R_reparateur>& robots_rep);
			   
	void translate_towards_goal2(const std::vector<Particule>& particules,
                                 const std::vector<R_neutraliseur>& robots_neutr,
                                 const std::vector<R_reparateur>& robots_rep);
	
};

class R_reparateur : public Robot {
private :
	S2d goal;
	bool bool_goal;
	int panne_index;
public :
	R_reparateur(Circle c);
	bool superposition_r_reparateur(const R_reparateur& r) const;
	bool superposition_p_r_reparateur(const Particule& r) const;
	
	std::string get_as_string();
	void draw_robot_rep();
	void move_rep_to(const std::vector<Particule>& particules,
					 std::vector<R_neutraliseur>& robots_neutr, 
					 const std::vector<R_reparateur>& robots_rep);
					 
	S2d getGoal() const;
	bool getBoolGoal() const;
	int getPanneIndex() const;
	
	void setGoal(S2d newGoal);
	void setBoolGoal(bool boolGoal);
	void setPanneIndex(int newIndex);
};

S2d s2d_scale(const S2d& v, double scalar);
S2d s2d_subtract(const S2d& a, const S2d& b);

Orient get_desired_orientation(const Circle& robot_circle, const Square& particle_square);

#endif	
		
	
