#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Strategies.h"
#include "Liste_case.h"
#include "S_Zsg.h"


//EXERCICE 1 : Stratégie aléatoire recursive
int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff) {
    int nb_coups = 0;
    int taille_zsg;
    ListeCase L;
    int couleur_zsg, nouvelle_couleur;
    Elnt_liste *cour;

    while (1) {
        init_liste(&L);
        taille_zsg = 0;
        couleur_zsg = M[0][0];

        trouve_zone_rec(M, dim, 0, 0, &taille_zsg, &L);

        if (taille_zsg == dim * dim) {
            cour = L;
            while (cour != NULL) {
                M[cour->i][cour->j] = couleur_zsg;
                cour = cour->suiv;
            }
            detruit_liste(&L);
            break;
        }

        do {
            nouvelle_couleur = rand() % nbcl;
        } while (nouvelle_couleur == couleur_zsg);

        cour = L;
        while (cour != NULL) {
            M[cour->i][cour->j] = nouvelle_couleur;
            if (aff == 1) Grille_attribue_couleur_case(G, cour->i, cour->j, nouvelle_couleur);
            cour = cour->suiv;
        }

        if (aff == 1) Grille_redessine_Grille(G);
        detruit_liste(&L);  
        nb_coups++;
    }

    return nb_coups;
}

//EXERCICE 2 : Stratégie aléatoire rapide
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff) {
    int nb_coups = 0;
    int nouvelle_couleur;
    S_Zsg Z;
    Elnt_liste *cour;
    
    init_Zsg(&Z, dim, nbcl);
    int couleur_initiale = M[0][0];
    agrandit_Zsg(M, &Z, couleur_initiale, 0, 0);
    
    while (1) {
        int couleur_zsg = M[0][0];
        int taille_zsg = 0;
        cour = Z.Lzsg;
        while (cour != NULL) {
            taille_zsg++;
            cour = cour->suiv;
        }
        
        if (taille_zsg == dim * dim) break;
        
        do {
            nouvelle_couleur = rand() % nbcl;
        } while (nouvelle_couleur == couleur_zsg);
        
        cour = Z.Lzsg;
        while (cour != NULL) {
            M[cour->i][cour->j] = nouvelle_couleur;
            if (aff == 1) Grille_attribue_couleur_case(G, cour->i, cour->j, nouvelle_couleur);
            cour = cour->suiv;
        }
        
        while (!test_liste_vide(&(Z.B[nouvelle_couleur]))) {
            int i, j;
            enleve_en_tete(&(Z.B[nouvelle_couleur]), &i, &j);
            agrandit_Zsg(M, &Z, nouvelle_couleur, i, j);
        }
        
        if (aff == 1) Grille_redessine_Grille(G);
        nb_coups++;
    }
    free_Zsg(&Z);  
    return nb_coups;
}

//EXERCICE 3 : Stratégie max-bordure
int sequence_max_bordure(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int cl_max;
    int max_count;
    int i, j, c;
    
    int *tab_compteurs = (int *)calloc(nbcl, sizeof(int));
    if (tab_compteurs == NULL) return -1;    
    
    init_Zsg(&Z, dim, nbcl);
    int couleur_zsg = M[0][0];
    agrandit_Zsg(M, &Z, couleur_zsg, 0, 0);
    
    if (aff == 1) {
        Elnt_liste *cour_aff = Z.Lzsg;
        while (cour_aff != NULL) {
            Grille_attribue_couleur_case(G, cour_aff->i, cour_aff->j, couleur_zsg);
            cour_aff = cour_aff->suiv;
        }
        Grille_redessine_Grille(G);
    }
    
    int zsg_complete = 0;
    while (!zsg_complete) {
        couleur_zsg = M[0][0];
        
        compte_couleurs_bordure(&Z, tab_compteurs);
        
        cl_max = -1;
        max_count = -1;
        zsg_complete = 1;
        
        for (c = 0; c < nbcl; c++) {
            if (tab_compteurs[c] > 0) {
                zsg_complete = 0;
                
                if (c != couleur_zsg && tab_compteurs[c] > max_count) {
                    max_count = tab_compteurs[c];
                    cl_max = c;
                }
            }
        }
        
        if (zsg_complete) break;
        
        if (cl_max == -1) {
            free(tab_compteurs);
            free_Zsg(&Z);
            return -1;
        }
        
        Elnt_liste *cour = Z.Lzsg;
        while (cour != NULL) {
            M[cour->i][cour->j] = cl_max;
            if (aff == 1) {
                Grille_attribue_couleur_case(G, cour->i, cour->j, cl_max);
            }
            cour = cour->suiv;
        }
        nb_coups++;
        
        while (!test_liste_vide(&(Z.B[cl_max]))) {
            enleve_en_tete(&(Z.B[cl_max]), &i, &j);
            agrandit_Zsg(M, &Z, cl_max, i, j);
        }
        
        if (aff == 1) Grille_redessine_Grille(G);
    }
    free(tab_compteurs);
    free_Zsg(&Z);
    return nb_coups;
}

//EXERCICE 4 : Stratégie max-bordure-zone
int sequence_max_bordure_zone(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int cl_max;
    int max_count;
    int c;
    
    int *tab_compteurs = (int *)calloc(nbcl, sizeof(int));
    if (tab_compteurs == NULL)  return -1;
    
    init_Zsg(&Z, dim, nbcl);
    
    int couleur_zsg = M[0][0];
    Z.App[0][0] = couleur_zsg;
    ajoute_en_tete(&(Z.B[couleur_zsg]), 0, 0);
    
    //agrandir avec la version zone (explore les zones complètes)
    agrandit_Zsg_zone(M, &Z, couleur_zsg);
    
    if (aff==1) {
        Elnt_liste *cour_aff = Z.Lzsg;
        while (cour_aff != NULL) {
            Grille_attribue_couleur_case(G, cour_aff->i, cour_aff->j, couleur_zsg);
            cour_aff = cour_aff->suiv;
        }
        Grille_redessine_Grille(G);
    }
    
    int zsg_complete = 0;
    while (!zsg_complete) {
        couleur_zsg = M[0][0];
        
        // cmpter les cases dans la bordure-zone
        // cette fois B[c] contient toutes les cases des zones de couleur c adjacentes
        compte_couleurs_bordure(&Z, tab_compteurs);
        
        cl_max = -1;
        max_count = -1;
        zsg_complete = 1;
        
        for (c = 0; c < nbcl; c++) {
            if (tab_compteurs[c] > 0) {
                zsg_complete = 0;
                
                if (c != couleur_zsg && tab_compteurs[c] > max_count) {
                    max_count = tab_compteurs[c];
                    cl_max = c;
                }
            }
        }
        
        if (zsg_complete) break;
        if (cl_max == -1) {
            free(tab_compteurs);
            free_Zsg(&Z);
            return -1;
        }
        
        // colorier Zsg
        Elnt_liste *cour = Z.Lzsg;
        while (cour != NULL) {
            M[cour->i][cour->j] = cl_max;
            if (aff == 1) {
                Grille_attribue_couleur_case(G, cour->i, cour->j, cl_max);
            }
            cour = cour->suiv;
        }
        nb_coups++;
        agrandit_Zsg_zone(M, &Z, cl_max);

        if (aff == 1) Grille_redessine_Grille(G);
    }
    free(tab_compteurs);
    free_Zsg(&Z);
    return nb_coups;
}


// EXERCICE 5 : Stratégies bonus

// quelques fonctions auxiliaures utiles
/* Copie toute la structure Z dans une nouvelle structure Z_copie */
static void copie_structure_zsg(S_Zsg *Z, S_Zsg *Z_copie) {
    int i, j, c;
    Elnt_liste *cour;
    
    Z_copie->dim = Z->dim;
    Z_copie->nbcl = Z->nbcl;
    
    // copier la liste Lzsg
    init_liste(&(Z_copie->Lzsg));
    cour = Z->Lzsg;
    while (cour != NULL) {
        ajoute_en_tete(&(Z_copie->Lzsg), cour->i, cour->j);
        cour = cour->suiv;
    }
    
    // copier les bordures B[c] pour chaque couleur
    Z_copie->B = (ListeCase *)malloc(Z_copie->nbcl * sizeof(ListeCase));
    for (c = 0; c < Z_copie->nbcl; c++) {
        init_liste(&(Z_copie->B[c]));
        cour = Z->B[c];
        while (cour != NULL) {
            ajoute_en_tete(&(Z_copie->B[c]), cour->i, cour->j);
            cour = cour->suiv;
        }
    }
    
    // copier le tableau App
    Z_copie->App = (int **) malloc(Z_copie->dim * sizeof(int *));
    for (i = 0; i < Z_copie->dim; i++) {
        Z_copie->App[i] = (int *) malloc(Z_copie->dim * sizeof(int));
        for (j = 0; j < Z_copie->dim; j++) {
            Z_copie->App[i][j] = Z->App[i][j];
        }
    }
}

/* Simule un coup sans modifier l'etat reel */
static int simuler_coup(int **M, S_Zsg *Z, int couleur_testee) {
    int couleur_actuelle = M[0][0];
    
    if (couleur_testee == couleur_actuelle) return 0;

    ListeCase anciennes_cases_zsg;
    init_liste(&anciennes_cases_zsg);
    for (Elnt_liste *p = Z->Lzsg; p != NULL; p = p->suiv) {
        ajoute_en_tete(&anciennes_cases_zsg, p->i, p->j);
    }

    S_Zsg Z_test;
    copie_structure_zsg(Z, &Z_test);

    for (Elnt_liste *p = anciennes_cases_zsg; p != NULL; p = p->suiv) {
        M[p->i][p->j] = couleur_testee;
    }

    int gain = agrandit_Zsg_zone(M, &Z_test, couleur_testee);

    for (Elnt_liste *p = anciennes_cases_zsg; p != NULL; p = p->suiv) {
        M[p->i][p->j] = couleur_actuelle;
    }

    detruit_liste(&anciennes_cases_zsg);
    free_Zsg(&Z_test);
    return gain;
}

/* Evalue un coup en regardant 2 coups à l avance */
static int evalue_deux_coups(int **M, S_Zsg *Z, int couleur_candidate) {
    int couleur_actuelle = M[0][0];
    if (couleur_candidate == couleur_actuelle) return -1;

    ListeCase anciennes_cases;
    init_liste(&anciennes_cases);
    for (Elnt_liste *p = Z->Lzsg; p != NULL; p = p->suiv) {
        ajoute_en_tete(&anciennes_cases, p->i, p->j);
    }

    S_Zsg Z_apres_coup1;
    copie_structure_zsg(Z, &Z_apres_coup1);

    for (Elnt_liste *p = anciennes_cases; p != NULL; p = p->suiv) {
        M[p->i][p->j] = couleur_candidate;
    }

    int gain_coup1 = agrandit_Zsg_zone(M, &Z_apres_coup1, couleur_candidate);

    int meilleur_gain_coup2 = 0;
    for (int c2 = 0; c2 < Z_apres_coup1.nbcl; c2++) {
        if (c2 == couleur_candidate) continue;
        int gain2 = simuler_coup(M, &Z_apres_coup1, c2);
        if (gain2 > meilleur_gain_coup2) {
            meilleur_gain_coup2 = gain2;
        }
    }

    for (Elnt_liste *p = anciennes_cases; p != NULL; p = p->suiv) {
        M[p->i][p->j] = couleur_actuelle;
    }

    detruit_liste(&anciennes_cases);
    free_Zsg(&Z_apres_coup1);

    return gain_coup1 + meilleur_gain_coup2;
}


/* Stratégie avec anticipation à 2 coups */
int sequence_anticipation(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int c;
    int couleur_zsg;
    int meilleur_score;
    int couleur_choisie;
    Elnt_liste *cour;

    int *compteurs = (int *)calloc(nbcl, sizeof(int));
    if (compteurs == NULL) return -1;   

    // initialisation (pareil que exo4)
    init_Zsg(&Z, dim, nbcl);
    couleur_zsg = M[0][0];
    Z.App[0][0] = couleur_zsg;
    ajoute_en_tete(&(Z.B[couleur_zsg]), 0, 0);
    agrandit_Zsg_zone(M, &Z, couleur_zsg);

    if (aff == 1) {
        cour = Z.Lzsg;
        while (cour != NULL) {
            Grille_attribue_couleur_case(G, cour->i, cour->j, couleur_zsg);
            cour = cour->suiv;
        }
        Grille_redessine_Grille(G);
    }

    int fini = 0;
    while (!fini) {
        couleur_zsg = M[0][0];

        // compter ce qu'il reste à prendre
        compte_couleurs_bordure(&Z, compteurs);

        // verifier si c'est fini
        fini = 1;
        for (c = 0; c < nbcl; c++) {
            if (compteurs[c] > 0) { 
                fini = 0; 
                break; 
            }
        }  if (fini) break;

        // on va chercher la meilleure couleur en simulant 2 coups
        meilleur_score = -1;
        couleur_choisie = -1;

        for (c = 0; c < nbcl; c++) {
            if (c == couleur_zsg || compteurs[c] == 0) continue;

            int score = evalue_deux_coups(M, &Z, c);
            if (score > meilleur_score) {
                meilleur_score = score;
                couleur_choisie = c;
            }
        }

        if (couleur_choisie == -1) {
            free(compteurs);
            free_Zsg(&Z);
            return -1;
        }

        // jouer vraiment le coup
        cour = Z.Lzsg;
        while (cour != NULL) {
            M[cour->i][cour->j] = couleur_choisie;
            if (aff == 1) {
                Grille_attribue_couleur_case(G, cour->i, cour->j, couleur_choisie);
            }
            cour = cour->suiv;
        }

        nb_coups++;
        agrandit_Zsg_zone(M, &Z, couleur_choisie);
        if (aff == 1) Grille_redessine_Grille(G);
    }
    free(compteurs);
    free_Zsg(&Z);
    return nb_coups;
}

// Stratégie mixte
/* Combine max-bordure-zone et anticipation: 
 * -> utilise max-bordure-zone quand le choix est evident (+ rapide)
 * -> utilise anticipation quand plusieurs choix sont possibles (+ lent mais efficace) */
int sequence_mixte(int **M, Grille *G, int dim, int nbcl, int aff) {
    S_Zsg Z;
    int nb_coups = 0;
    int c;
    int couleur_zsg;
    int couleur_choisie;
    Elnt_liste *cour;

    int *compteurs = (int *)calloc(nbcl, sizeof(int));
    if (compteurs == NULL) return -1;

    init_Zsg(&Z, dim, nbcl);
    couleur_zsg = M[0][0];
    Z.App[0][0] = couleur_zsg;
    ajoute_en_tete(&(Z.B[couleur_zsg]), 0, 0);
    agrandit_Zsg_zone(M, &Z, couleur_zsg);

    if (aff == 1) {
        cour = Z.Lzsg;
        while (cour != NULL) {
            Grille_attribue_couleur_case(G, cour->i, cour->j, couleur_zsg);
            cour = cour->suiv;
        }
        Grille_redessine_Grille(G);
    }

    int fini = 0;
    while (!fini) {
        couleur_zsg = M[0][0];
        compte_couleurs_bordure(&Z, compteurs);

        fini = 1;
        for (c = 0; c < nbcl; c++) {
            if (compteurs[c] > 0) { 
                fini = 0; 
                break; 
            }
        }
        if (fini) break;

        // trouver les 2 meilleures couleurs
        int max1 = -1, max2 = -1;
        int couleur_max1 = -1, couleur_max2 = -1;

        for (c = 0; c < nbcl; c++) {
            if (c == couleur_zsg || compteurs[c] == 0) continue;
            if (compteurs[c] > max1) {
                max2 = max1; couleur_max2 = couleur_max1;
                max1 = compteurs[c]; couleur_max1 = c;
            } else if (compteurs[c] > max2) {
                max2 = compteurs[c]; couleur_max2 = c;
            }
        }

        if (couleur_max1 == -1) {
            free(compteurs);
            free_Zsg(&Z);
            return -1;
        }

        // on décide 
        if (couleur_max2 == -1 || max1 > max2 * 1.5) {
            // choix evident :on utilise max-bordure-zone 
            couleur_choisie = couleur_max1;
        } else {
            // utiliser l anticipation 
            int s1 = evalue_deux_coups(M, &Z, couleur_max1);
            int s2 = evalue_deux_coups(M, &Z, couleur_max2);
            couleur_choisie = (s2 > s1) ? couleur_max2 : couleur_max1;
        }

        //appliquer le coup
        cour = Z.Lzsg;
        while (cour != NULL) {
            M[cour->i][cour->j] = couleur_choisie;
            if (aff == 1) {
                Grille_attribue_couleur_case(G, cour->i, cour->j, couleur_choisie);
            }
            cour = cour->suiv;
        }

        nb_coups++;
        agrandit_Zsg_zone(M, &Z, couleur_choisie);
        if (aff == 1) Grille_redessine_Grille(G);
    }
    free(compteurs);
    free_Zsg(&Z);
    return nb_coups;
}
