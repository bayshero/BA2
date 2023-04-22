//SIMULATION.CC, BENARAFA MANON: 90% / GLASSEY ADELINE: 10%, V2
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "constantes.h"
#include "nourriture.h"
#include "fourmiliere.h"
#include "simulation.h"
#include "squarecell.h"

using namespace std;


void lecture(string nom_fichier){
    ifstream ifs;
	ifs.open(nom_fichier, ios::in); 
    if(!ifs.fail()){
		string line;
		size_t nbN(0);
		size_t nbF(0);
		
		while(getline(ifs>>ws, line)){
			if(line[0]=='#'){
				continue;
			}
			istringstream data(line);
			data >> nbN ;
			break;
		}
		for(size_t i(0) ; i< nbN;){	
			getline(ifs>>ws, line);
			if(line[0]!='#'){
				if (lecture_N(line)==false){
					clear_simulation();
					return;
				}
				++i;
			}
		}		
		while(getline(ifs>>ws,line)){
			if(line[0]=='#'){
				continue;
			}
			istringstream data(line);
			data >> nbF ;
			break;
		}
		for(size_t i(0) ; i< nbF ; ++i){
			if (lectureF(ifs,i)==false){
				clear_simulation();
				return;
			}
		}
		ifs.close();	
	}else{
		exit(EXIT_FAILURE);
	}
}

void draw_world(){
	draw_nourriture();
	draw_fourmilieres();
}

void clear_simulation(){
	squarecell_reset_memory();
	clear_fourmilieres();
	clear_n();
}

void save_simulation(string filename){
	ofstream myfile;
    myfile.open(filename);
    myfile<<save_N();
    myfile<<save_F();
    myfile.close();	
}

void maj(){
	
	generate_N();
	maj_fourmiliere();

	
}

void generate_N(){
	
	if(random_bool(food_rate)){
		Point p={random_unsigned(),random_unsigned()};
		Square s={1,p,1};
		for(unsigned int i(0); i<max_food_trial; ++i){
			if(square_memory(s)){
				if(test_superpose_F(s)==false){
					Nourriture n(s);
					return;
				}
				
			}
		}
	}
	
}

string get_nourriture_string(){
	unsigned int nbn(get_nbN());
	return to_string(nbn);
}

vector<string> get_fourmiliere_string(unsigned int i){
	vector<double> info_i(get_info_fourmiliere(i));
	vector<string>info_s;
	for(auto element : info_i){
		stringstream ss;
		ss<<fixed<<setprecision(1)<<element;
		info_s.push_back(ss.str());
	}
	return info_s;
}

unsigned int get_nbF(){
	return get_nb_fourmilieres();
}


