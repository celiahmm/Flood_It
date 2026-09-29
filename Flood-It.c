#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Grille.h"
#include "API_Gene_instance.h"
#include "Liste_case.h"
#include "S_Zsg.h"
#include "Strategies.h"
 

int main(int argc,char**argv){

	int dim, nbcl, nivdif, graine, exo, aff;
	Grille *G;
	int i,j;
	int **M;

	int nb_coups; 

	clock_t
		temps_initial, /* Temps initial (ticks CPU) */
		temps_final;   /* Temps final (ticks CPU) */
	double
		temps_cpu;     /* Temps total en secondes */ 
	
	if(argc!=7){
        printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-7> <aff 0/1>\n",argv[0]);
        return 1;
    }

	dim=atoi(argv[1]);
	nbcl=atoi(argv[2]);
	nivdif=atoi(argv[3]);
	graine=atoi(argv[4]);
	exo=atoi(argv[5]);
	aff=atoi(argv[6]);

	/* Allocation puis generation de l'instance */

	M = (int **) malloc(sizeof(int*)*dim);
	for (i = 0; i<dim; i++) {
		M[i] = (int*) malloc(sizeof(int)*dim);
		if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
	}

	Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);

    /* Affichage de la grille */
	if (aff==1) { 
		Grille_init(dim, nbcl, 500, &G);

	Grille_ouvre_fenetre(G);

	for (i=0; i<dim; i++)
	  for (j=0; j<dim; j++){
			Grille_attribue_couleur_case(G, i, j, M[i][j]);
	  }

	Grille_redessine_Grille(G);
	Grille_attente_touche();
  	}

	// initialiser le générateur aléatoire une seule fois (gain de temps)
	//srand(time(NULL));
	srand(graine);
	  
  	temps_initial = clock();
	
	//if (exo==0) {
		/* A VOUS DE JOUER    */
	//}
	if (exo == 1) {
		nb_coups = sequence_aleatoire_rec(M, G, dim, nbcl, aff);
		printf("Nombre de coups : %d\n", nb_coups);
	} 
	if (exo == 2) {
		nb_coups = sequence_aleatoire_rapide(M, G, dim, nbcl, aff);
		printf("Nombre de coups : %d\n", nb_coups);
	}
	if (exo == 3) { 
		nb_coups = sequence_max_bordure(M, G, dim, nbcl, aff);
		printf("Nombre de coups : %d\n", nb_coups);
	}
	if (exo == 4) { 
		nb_coups = sequence_max_bordure_zone(M, G, dim, nbcl, aff); 
		printf("Nombre de coups : %d\n", nb_coups); 
	}
	if (exo == 5) {
		nb_coups = sequence_anticipation(M, G, dim, nbcl, aff);
		printf("Nombre de coups : %d\n", nb_coups);
	}
	if (exo == 6) { 
		nb_coups = sequence_mixte(M, G, dim, nbcl, aff); 
		printf("Nombre de coups : %d\n", nb_coups); 
	}

  
	// calcul du temps mis pour gagner le jeu (en fonction de la stratégie utilisee)
	temps_final = clock();
    temps_cpu = (double)(temps_final - temps_initial) / (double)CLOCKS_PER_SEC;
    printf("Temps : %fs\n", temps_cpu);

	/* Desallocation de la matrice */
	for(i = 0; i< dim; i++) {
		if (M[i]) free(M[i]);
	}
	if (M) free(M);

	/* Fermeture et désallocation de la grille */
	if (aff==1) { 
		Grille_ferme_fenetre();
		Grille_free(&G);
	}

	return 0;
}