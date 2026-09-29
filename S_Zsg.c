#include <stdlib.h>
#include <stdio.h>

#include "S_Zsg.h"
#include "Liste_case.h"

/* initialise la structure S_Zsg */
void init_Zsg(S_Zsg *Z, int dim, int nbcl) {
    
    Z->dim = dim;
    Z->nbcl = nbcl;
    
    init_liste(&(Z->Lzsg));
    
    Z->B = (ListeCase *) malloc(nbcl * sizeof(ListeCase));
    for (int c = 0; c < nbcl; c++) {
        init_liste(&(Z->B[c]));
    }
    
    Z->App = (int **) malloc(dim * sizeof(int *));
    for (int i = 0; i < dim; i++) {
        Z->App[i] = (int *) malloc(dim * sizeof(int));
        for (int j = 0; j < dim; j++) {
            Z->App[i][j] = -2; // -2 = n'appartient ni à Zsg ni à une bordure
        }
    }
}

/*ajoute une case à la Zsg en O(1) */
void ajoute_Zsg(S_Zsg *Z, int i, int j) {
    ajoute_en_tete(&(Z->Lzsg), i, j);
    Z->App[i][j] = -1; // -1 = appartient à Zsg
}

/* ajoute une case à la bordure d'une couleur en O(1) */
void ajoute_Bordure(S_Zsg *Z, int i, int j, int cl) {
    ajoute_en_tete(&(Z->B[cl]), i, j);
    Z->App[i][j] = cl; // cl = appartient à la bordure de couleur cl
}

/* teste si une case appartient à Zsg en O(1) */
int appartient_Zsg(S_Zsg *Z, int i, int j) {
    return Z->App[i][j] == -1;
}

/* teste si une case appartient à la bordure d'une couleur en O(1) */
int appartient_Bordure(S_Zsg *Z, int i, int j, int cl) {
    return Z->App[i][j] == cl;
}

/* libere la memoire de la structure */
void free_Zsg(S_Zsg *Z) {
    detruit_liste(&(Z->Lzsg));
    
    for (int i = 0; i < Z->nbcl; i++) {
        detruit_liste(&(Z->B[i]));
    }
    free(Z->B);
    
    for (int i = 0; i < Z->dim; i++) {
        free(Z->App[i]);
    }
    free(Z->App);
}


/* compte le nombre de cases de chaque couleur dans la bordure */
void compte_couleurs_bordure(S_Zsg *Z, int *compteurs){
    for (int c = 0; c < Z->nbcl; c++) compteurs[c] = 0;

    for (int c = 0; c < Z->nbcl; c++) {
        for (Elnt_liste *p = Z->B[c]; p != NULL; p = p->suiv) {

            if (Z->App[p->i][p->j] == c) {
                compteurs[c]++;
            }
        }
    }
}


/* Met à jour la structure quand une case de bordure bascule dans Zsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l) {
    int nb_cases_ajoutees = 0;
    ListeCase file;  // on utilise une file pour parcourir en largeur
    int i, j, voisin_i, voisin_j, couleur_voisin;
    
    // initialiser la file avec la case de départ
    init_liste(&file);
    ajoute_en_tete(&file, k, l);
    Z->App[k][l] = -1; // on marque comme appartenant à Zsg
    
    // parcours de la zone de couleur cl
    while (!test_liste_vide(&file)) {
        // on retire une case de la file
        enleve_en_tete(&file, &i, &j);
        
        // on l'ajoute  à Lzsg
        ajoute_en_tete(&(Z->Lzsg), i, j);
        nb_cases_ajoutees++;
        
        // on explore les 4 voisins
        int dir_i[] = {-1, 1, 0, 0};  // haut-bas-gauche-droite
        int dir_j[] = {0, 0, -1, 1};
        
        for (int d = 0; d < 4; d++) {
            voisin_i = i + dir_i[d];
            voisin_j = j + dir_j[d];
            
            // on vérifie d'abord si le voisin est dans la grille
            if (voisin_i < 0 || voisin_i >= Z->dim || 
                voisin_j < 0 || voisin_j >= Z->dim) {
                continue;
            }
            
            couleur_voisin = M[voisin_i][voisin_j];
            
            // cas n1 : le voisin est de couleur cl et pas encore visité
            if (couleur_voisin == cl && Z->App[voisin_i][voisin_j] == -2) {
                // alors on l'ajoute à la file pour exploration
                ajoute_en_tete(&file, voisin_i, voisin_j);
                Z->App[voisin_i][voisin_j] = -1; 
            }

            // cas n2 : le voisin est d'une autre couleur et pas encore dans la bordure
            else if (couleur_voisin != cl && Z->App[voisin_i][voisin_j] == -2) {
                // alors l'ajouter à la bordure correspondante
                ajoute_Bordure(Z, voisin_i, voisin_j, couleur_voisin);
            }
        }
    }
    detruit_liste(&file); 
    return nb_cases_ajoutees; //retourne le nombre de cases ajoutees à Zsg
} 



/* fonction aux. de agrandit_Zsg-zone: explore une zone complète et l'ajoute à B[couleur] */
static void explore_zone_complete(int **M, S_Zsg *Z, int i_depart, int j_depart, int couleur) {
    ListeCase file;
    int i, j, voisin_i, voisin_j;
    int dir_i[] = {-1, 1, 0, 0};
    int dir_j[] = {0, 0, -1, 1};
    
    init_liste(&file);
    ajoute_en_tete(&file, i_depart, j_depart);
    Z->App[i_depart][j_depart] = couleur;  
    
    // explorer toute la zone
    while (!test_liste_vide(&file)) {
        enleve_en_tete(&file, &i, &j);
        
        //ajouter à B[couleur]
        ajoute_en_tete(&(Z->B[couleur]), i, j);
        
        //explorer les voisins de même couleur
        for (int d = 0; d < 4; d++) {
            voisin_i = i + dir_i[d];
            voisin_j = j + dir_j[d];
            
            if (voisin_i < 0 || voisin_i >= Z->dim || voisin_j < 0 || voisin_j >= Z->dim) continue;
            
            //si même couleur et pas encore visité
            if (M[voisin_i][voisin_j] == couleur && Z->App[voisin_i][voisin_j] == -2) {
                ajoute_en_tete(&file, voisin_i, voisin_j);
                Z->App[voisin_i][voisin_j] = couleur;  
            }
        }
    }
    detruit_liste(&file);
}


/* Nouvelle version de agrandit_Zsg_zone pour l exercice 4 : explore les zones complètes adjacentes
 * différence avec agrandit_Zsg : quand on découvre un voisin d'une autre couleur on explore toute sa zone au lieu d'ajouter juste cette case */
int agrandit_Zsg_zone(int **M, S_Zsg *Z, int cl) {
    int nb_cases_ajoutees = 0;
    ListeCase zones_a_traiter;
    int i, j, voisin_i, voisin_j;
    int dir_i[] = {-1, 1, 0, 0};
    int dir_j[] = {0, 0, -1, 1};
    
    // on copie toutes les cases de B[cl] dans une liste temporaire
    init_liste(&zones_a_traiter);
    Elnt_liste *cour = Z->B[cl];
    while (cour != NULL) {
        ajoute_en_tete(&zones_a_traiter, cour->i, cour->j);
        cour = cour->suiv;
    }
    
    // on vide B[cl] czr ses cases vont être intégrées à Zsg
    detruit_liste(&(Z->B[cl]));
    init_liste(&(Z->B[cl]));
    
    // traiter chaque case de l'ancienne bordure
    while (!test_liste_vide(&zones_a_traiter)) {
        enleve_en_tete(&zones_a_traiter, &i, &j);
        
        // si pas déjà dans Zsg
        if (Z->App[i][j] != -1) {
            // intégrer toute la zone de couleur cl à Zsg
            ListeCase file;
            init_liste(&file);
            ajoute_en_tete(&file, i, j);
            Z->App[i][j] = -1;
            
            while (!test_liste_vide(&file)) {
                int ci, cj;
                enleve_en_tete(&file, &ci, &cj);
                ajoute_en_tete(&(Z->Lzsg), ci, cj);
                nb_cases_ajoutees++;
                
                // explorer les voisins
                for (int d = 0; d < 4; d++) {
                    voisin_i = ci + dir_i[d];
                    voisin_j = cj + dir_j[d];
                    
                    if (voisin_i < 0 || voisin_i >= Z->dim || voisin_j < 0 || voisin_j >= Z->dim) continue;
                    int couleur_voisin = M[voisin_i][voisin_j];
                    
                    // cas n1 : voisin de même couleur cl : l'ajouter à Zsg
                    if (couleur_voisin == cl && Z->App[voisin_i][voisin_j] != -1) {
                        ajoute_en_tete(&file, voisin_i, voisin_j);
                        Z->App[voisin_i][voisin_j] = -1;
                    }
                    // cas n2 : voisin d'autre couleur: explorer toyte sa zone
                    else if (couleur_voisin != cl && Z->App[voisin_i][voisin_j] == -2) {
                        explore_zone_complete(M, Z, voisin_i, voisin_j, couleur_voisin);
                    }
                }
            }
            detruit_liste(&file);
        }
    }
    detruit_liste(&zones_a_traiter);
    return nb_cases_ajoutees;
}

