/*!
  \file   constantes.h
  \author Charly Guardia et Gauthier de Mercey
  \date   mars 2023
  \version 1
*/

#ifndef CONSTANTES_H
#define CONSTANTES_H

#include "shape.h"

using namespace std;

constexpr short unsigned maxF(25);
constexpr double dmax(128.);
constexpr double delta_t(0.125); // seconde
constexpr double r_spatial(16.);
constexpr double r_reparateur(2.);
constexpr double r_neutraliseur(4.);
constexpr double vtran_max(4.); // par seconde
constexpr double vrot_max(0.125); // rd/s env. 7°/s
constexpr double epsil_alignement(0.01); // rd env. 0.6°
constexpr double desintegration_rate(0.4);
constexpr double risk_factor(3.);
constexpr double d_particule_min(8*epsil_zero);
constexpr unsigned max_update(600);
constexpr unsigned modulo_update(100);
constexpr unsigned taille_dessin(500);

#endif
