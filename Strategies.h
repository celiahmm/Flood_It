#ifndef __STRATEGIES__
#define __STRATEGIES__

#include "API_Grille.h"
#include "S_Zsg.h"

int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);
int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff);
int sequence_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff);


// EXERCICE 5 
/*Stratégie avec anticipation à 2 coups
 * elle chaque coup + le meilleur coup suivant pour faire un choix optimal
 * plus lent mais meilleur nombre de coups*/
int sequence_anticipation(int **M, Grille *G, int dim, int nbcl, int aff);

// Stratégie mixte
/* Combine max-bordure-zone et anticipation: 
 * -> utilise max-bordure-zone quand le choix est evident
 * -> utilise anticipation quand plusieurs choix sont possibles */
int sequence_mixte(int **M, Grille *G, int dim, int nbcl, int aff);

#endif