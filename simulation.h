#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <string>

std::string get_nourriture_string();
std::vector<std::string> get_fourmiliere_string(unsigned int i);
unsigned int get_nbF();
bool get_erreur_simul();

void lecture(std::string nom_fichier);
void draw_world();
void clear_simulation();
void save_simulation(std::string filename);
void maj();
void generate_N();

#endif // SIMULATION_H_INCLUDED
