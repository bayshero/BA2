//GRAPHIC.CC, BENARAFA MANON: %0 / GLASSEY ADELINE: 100%, V2
#include <iostream>
#include <cmath>
#include "graphic_gui.h"

using namespace std;

namespace {
	vector<vector<double>> Couleur={{1.,0.,0.}, {0.,1.,0.}, {0.,0.,1.}, {1.,1.,0.},
									{1.,0.,1.}, {0.,1.,1.}};
}

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);
constexpr unsigned taille_dessin(500);
constexpr short unsigned g_max(128);
constexpr double delta(taille_dessin/(double)g_max);

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr){
	ptcr = &cr;
}

void graphic_empty_world(){
	(*ptcr)->set_line_width(delta);
	(*ptcr)->set_source_rgb(1, 1, 1);
	(*ptcr)->move_to((delta/(double)2),(delta/(double)2));
	(*ptcr)->line_to((delta/(double)2),taille_dessin-(delta/(double)2));
	(*ptcr)->line_to(taille_dessin-(delta/(double)2),taille_dessin-(delta/(double)2));
	(*ptcr)->line_to(taille_dessin-(delta/(double)2),(delta/(double)2));
	(*ptcr)->line_to(0,0);	
	(*ptcr)->stroke();
	
	(*ptcr)->set_line_width(0.3);
	(*ptcr)->set_source_rgb(0.5,0.5,0.5);
	for(double i(0); i<=taille_dessin; i+=delta){
		(*ptcr)->move_to(i,0);
		(*ptcr)->line_to(i,taille_dessin);
		(*ptcr)->stroke();
	}
	for(double i(0); i<=taille_dessin; i+=delta){
		(*ptcr)->move_to(0,i);
		(*ptcr)->line_to(taille_dessin,i);
		(*ptcr)->stroke();
	}
}

void set_color(int indice){
	(*ptcr)->set_source_rgb(Couleur[indice%6][0],Couleur[indice%6][1],
							Couleur[indice%6][2]);
}

void  set_color_light(int indice){
	if (indice == 0){
		(*ptcr)->set_source_rgb(1, 0.5,0.5);
	}else{
		if (indice%6==0){
			(*ptcr)->set_source_rgb(1, 0.5,0.5);
		}
		if ((indice+5)%6==0){
			(*ptcr)->set_source_rgb(0.75, 1, 0.75);
		}
		if ((indice+4)%6==0){
			(*ptcr)->set_source_rgb(0.5, 0.5, 1);
		}
		if ((indice+3)%6==0){
			(*ptcr)->set_source_rgb(1, 1, 0.85);
		}
		if ((indice+2)%6==0){
			(*ptcr)->set_source_rgb(1, 0.75, 1);
		}
		if ((indice+1)%6==0){
			(*ptcr)->set_source_rgb(0.75, 1, 1);
		}
	}
}

void fill_unitsquare (double x, double y, int indice){
	(*ptcr)->set_line_width(delta);
	set_color(indice);
	(*ptcr)->move_to(x*delta,(y*delta)+(delta/2));
	(*ptcr)->line_to((x*delta)+delta,(y*delta)+(delta/2));
	(*ptcr)->stroke();	
}

void fill_diamond(double x, double y){
	(*ptcr)->set_line_width(delta/(sqrt(2)));
	(*ptcr)->set_source_rgb(1, 1, 1);
	(*ptcr)->move_to((x*delta)+(delta/4),(y*delta)+(delta/4));
	(*ptcr)->line_to((x*delta)+(delta*0.75),(y*delta)+(delta*0.75));
	(*ptcr)->stroke();
}	
	
void thin_square(double x, double y,double size,int indice){
	(*ptcr)->set_line_width(2.0);
	set_color(indice);
	(*ptcr)->move_to((x*delta)+(delta/2.),(y*delta)+(delta/2.));
	(*ptcr)->line_to((x*delta)+(size*delta)-(delta/2.),(y*delta)+(delta/2.));
	(*ptcr)->line_to((x*delta)+(size*delta)-(delta/2.),
					(y*delta)+(size*delta)-(delta/2.));
	(*ptcr)->line_to((x*delta)+(delta/2.),(y*delta)+(size*delta)-(delta/2.));
	(*ptcr)->line_to((x*delta)+(delta/2.),(y*delta)+((delta-2.)/2.));
	(*ptcr)->stroke();	
}	
	
void thick_line(double x, double y, double size, int indice){
	(*ptcr)->set_line_width(delta);
	set_color(indice);
	(*ptcr)->move_to((x*delta),(y*delta)+(delta/2));
	(*ptcr)->line_to((x*delta)+(size*delta),(y*delta)+(delta/2));
	(*ptcr)->stroke();		
}	

void thick_line_light(double x, double y, double size, int indice){
	(*ptcr)->set_line_width(delta);
	set_color_light(indice);
	(*ptcr)->move_to((x*delta),(y*delta)+(delta/2));
	(*ptcr)->line_to((x*delta)+(size*delta),(y*delta)+(delta/2));
	(*ptcr)->stroke();		
}
