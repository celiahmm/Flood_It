#include<stdlib.h>
#include "Liste_case.h"

 /* Initialise une liste vide */
void init_liste(ListeCase *L){
  *(L)=NULL;
}

/* Ajoute un element en tete de liste */
int ajoute_en_tete(ListeCase *L, int i, int j){
  Elnt_liste *elnt;
  elnt=(Elnt_liste*) malloc(sizeof(Elnt_liste)); // correction: sizeof(Elnt_liste) et pas sizeof(Elnt_liste *) 
  if (elnt==NULL) 
    return 0;
  elnt->suiv=*L;
  elnt->i=i;
  elnt->j=j;
  (*L)=elnt;
  return 1;
}

/* teste si une liste est vide */
int test_liste_vide(ListeCase *L){
  return (*L)==NULL;
}

/* Supprime l element de tete et retourne les valeurs en tete */
/* Attention: il faut que la liste soit non vide */
void enleve_en_tete(ListeCase *L, int *i, int *j){
  Elnt_liste *temp;
  *i=(*L)->i;
  *j=(*L)->j;
  temp=*L;
  *L=(*L)->suiv;
  free(temp);
}

/* Detruit tous les elements de la liste */
void detruit_liste(ListeCase *L){
 Elnt_liste *cour,*temp;
  cour=(*L);
  while (cour!=NULL){
    temp=cour;
    cour=cour->suiv;
    free(temp);
  }

  *L=NULL;
}


/* Trouve la zone connectée de même couleur à partir de la case (i, j) et remplit la liste L avec les coordonnées des cases de cette zone
  Met à jour la taille de la zone dans la variable pointée par taille */
void trouve_zone_rec(int **M, int dim, int i, int j, int *taille, ListeCase *L) {
    // verifier les limites de la grille
    if (i < 0 || i >= dim || j < 0 || j >= dim)  return;

    
    //recuperer la couleur de la case actuelle
    int couleur = M[i][j];
    
    //si la case est déjà visitée (marquée -1) alors on s'arrête
    if (couleur < 0) return;
    
    // marquer la case comme visitée
    M[i][j] = -1;
    
    // ajouter la case à la liste
    ajoute_en_tete(L, i, j);
    (*taille)++; //met à jour la taille
    
    // explorer les 4 voisins s'ils ont la même couleur
    if (i > 0 && M[i-1][j] == couleur) {
        trouve_zone_rec(M, dim, i-1, j, taille, L);
    }
    if (i < dim-1 && M[i+1][j] == couleur) {
        trouve_zone_rec(M, dim, i+1, j, taille, L);
    }
    if (j > 0 && M[i][j-1] == couleur) {
        trouve_zone_rec(M, dim, i, j-1, taille, L);
    }
    if (j < dim-1 && M[i][j+1] == couleur) {
        trouve_zone_rec(M, dim, i, j+1, taille, L);
    }
}

