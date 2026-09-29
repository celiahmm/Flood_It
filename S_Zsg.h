#ifndef __S_ZSGZ__
#define __S_ZSGZ__

#include "Liste_case.h"

typedef struct {
    int dim;           // dimension de la grille
    int nbcl;          // nombre de couleurs
    ListeCase Lzsg;    // liste des cases de la zone Zsg
    ListeCase *B;      // tableau de listes de cases de la bordure 
                       // exo4 : cette fois B[c] contient toutes les cases des zones de couleur c adjacentes à Zsg
                            // exo3 : B[rouge] = cases rouges adjacentes 
                            // exo4 : B[rouge] = toutes les cases de toutes les zones rouges adjacentes 
    int **App;         // tableau d'appartenances à double entrée
} S_Zsg;

/* ====== qst 2.1 : ====== */
/* initialise la structure S_Zsg */
void init_Zsg(S_Zsg *Z, int dim, int nbcl);

/* ajoute une case dans la liste Lzsg en O(1) */
void ajoute_Zsg(S_Zsg *Z, int i, int j);

/* ajoute une case dans la bordure de couleur cl en O(1) */
void ajoute_Bordure(S_Zsg *Z, int i, int j, int cl);

/* renvoie vrai si une case appartient à Lzsg en O(1) */
int appartient_Zsg(S_Zsg *Z, int i, int j);

/* renvoie vrai si une case appartient à la bordure de couleur cl en O(1) */
int appartient_Bordure(S_Zsg *Z, int i, int j, int cl);

/* libere la memoire de la structure S_Zsg */
void free_Zsg(S_Zsg *Z);

/*  ====== qst 2.2 ====== */
/* Met a jour la structure quand une case de bordure bascule dans Zsg
 * version simple : elle traite que les cases directement adjacentes
 * @param M : matrice du jeu
 * @param Z : structure S_Zsg
 * @param cl : couleur a integrer
 * @param k, l : coordonnees d'une case de B[cl] a integrer
 * @return : le nombre de cases ajoutees a Zsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

/*  ====== qst 3.1 ====== */
/* compte le nombre de cases de chaque couleur dans la bordure */
void compte_couleurs_bordure(S_Zsg *Z, int *compteurs);

/*  ====== qst 4.1 ====== */
/* met à jour la Zsg en integrant la couleur 'cl' et en decouvrant les nouvelles zones adjacentes completes
 * cl : nouvelle couleur choisie pour l'agrandissement.
 * retourne le nombre de cases ajoutées à la Zsg. */
int agrandit_Zsg_zone(int **M, S_Zsg *Z, int cl);

#endif