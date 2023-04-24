/*!
  \file   robot.h
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#ifndef ROBOT_H
#define ROBOT_H
#include "particule.h"
#include "shape.h"

class Robot{
protected :
	Circle cercle;
public:
	Robot(Circle c);
	Robot();
	Circle GetCircle() const;
	bool superposition_r_neutre_rep(const Robot& r) const;
};

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
	R_spatial(Circle c, int nbUpdate, int nbNr, int nbNs, int nbNd, int nbNp, int nbRr,int nbRs);
	
	int GetNbUpdate() const;
	int GetNbRs() const;
	int GetNbRr() const;
	int GetNbNs() const;
	int GetNbNp() const;
	int GetNbNd() const;
	int GetNbNr() const;
	bool GetError_domain() const;
	
	bool superposition_p_rs(const Particule& p) const;
	bool rs_in_domain() const;
	
	std::string get_as_string();
	
	void setNbUpdate(int newNbUpdate);
};


class R_neutraliseur : public Robot {
private:
	double orientation;		//angle entre (-pi et pi)
	int k_update;
	bool panne;
	S2d but; 				//cible du robot 
	int c_n;
public:
	R_neutraliseur(Circle c, double a, int k_update_, bool panne_, int c_n_);
	int GetKupdate() const;
	bool superposition_r_neutre(const R_neutraliseur& r) const;
	bool superposition_p_r_neutraliseur(const Particule& r) const;
	bool error_attribut(const R_spatial& rs) const;
	std::string get_as_string();

};

class R_reparateur : public Robot {
private :
	S2d but; 				//cible du robot	
public :
	R_reparateur(Circle c);
	bool superposition_r_reparateur(const R_reparateur& r) const;
	bool superposition_p_r_reparateur(const Particule& r) const;
	std::string get_as_string();
};

void draw_robot_rep(const R_reparateur& r);
void draw_robot_neutr(const R_neutraliseur& r);
void draw_robot_spatial(const R_spatial& r);


#endif	
		
	
