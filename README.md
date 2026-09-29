# Flood-It

Projet académique réalisé en **L2 Informatique** dans le cadre du module
**Programmation Impérative 3 : introduction à l'algorithmique**. (Dec. 2025)

**Note obtenue : 20/20**

L'objectif du projet est d'implémenter le jeu **Flood-It** en langage C et de
comparer plusieurs stratégies de résolution. Le programme génère une grille
colorée, agrandit progressivement la zone contrôlée depuis la case supérieure
gauche et mesure le nombre de coups ainsi que le temps d'exécution.

## Présentation

À chaque coup, la zone contrôlée change de couleur et absorbe les zones
adjacentes de cette couleur. La partie est gagnée lorsque toute la grille
appartient à la zone contrôlée.

Le projet explore deux axes :

- **optimiser le temps d'exécution**, en évitant de recalculer toute la zone à
  chaque coup ;
- **réduire le nombre de coups**, en choisissant plus efficacement la couleur
  suivante.

## Stratégies implémentées

| Exercice | Fonction | Principe |
| --- | --- | --- |
| 1 | `sequence_aleatoire_rec` | Recalcule récursivement la zone contrôlée et choisit une couleur aléatoire. |
| 2 | `sequence_aleatoire_rapide` | Conserve la zone et ses bordures en mémoire pour limiter les recalculs. |
| 3 | `sequence_max_bordure` | Choisit la couleur qui possède le plus de cases en bordure. |
| 4 | `sequence_max_bordure_zone` | Compare les zones complètes adjacentes plutôt que les seules cases de bordure. |
| 5 | `sequence_anticipation` | Simule l'impact du coup courant et du coup suivant. |
| 6 | `sequence_mixte` | Combine `max-bordure-zone` et l'anticipation selon la situation. |

La stratégie mixte cherche un compromis entre la qualité de la solution et le
temps de calcul. Les structures `S_Zsg` et `ListeCase` permettent de mémoriser
la zone contrôlée et les bordures par couleur.

## Organisation du dépôt

```text
.
├── Flood-It.c              # Programme principal et analyse des arguments
├── Strategies.c/.h         # Stratégies de résolution
├── S_Zsg.c/.h              # Gestion de la zone contrôlée et des bordures
├── Liste_case.c/.h         # Liste chaînée de cases
├── API_Gene_instance.c/.h  # Génération des instances
├── API_Grille.c/.h         # Affichage graphique avec SDL2
└── Makefile                # Compilation et nettoyage
```

## Compilation

Prérequis : `gcc`, `make` et **SDL2** (`sudo apt install libsdl2-dev` sur
Debian/Ubuntu, `brew install sdl2` sur macOS).

```bash
make        # compile
make clean  # supprime les fichiers objets et l'exécutable
```

## Utilisation

La syntaxe générale est :

```bash
./Flood-It <dimension> <nb_de_couleurs> <niveau_de_difficulte> <graine> <strategie> <affichage>
```

| Paramètre | Description |
| --- | --- |
| `dimension` | Dimension de la grille carrée. |
| `nb_de_couleurs` | Nombre de couleurs utilisées. |
| `niveau_de_difficulte` | Niveau de difficulté de l'instance, entre `0` et `100`. |
| `graine` | Graine utilisée pour générer la grille. |
| `strategie` | Numéro de la stratégie, de `1` à `6`. |
| `affichage` | `0` pour désactiver SDL2, `1` pour afficher la grille. |

### Exemples

Exécuter la stratégie aléatoire rapide sans affichage :

```bash
./Flood-It 30 6 50 42 2 0
```

Exécuter la stratégie mixte avec affichage graphique :

```bash
./Flood-It 30 6 50 42 6 1
```

Le programme affiche le nombre de coups nécessaires et le temps CPU mesuré :

```text
Nombre de coups : 13
Temps : 0.001234s
```

Pour les tests de performance, il est recommandé d'utiliser `affichage=0`,
car le rendu graphique peut augmenter sensiblement le temps d'exécution.

## Résultats et observations

Les expérimentations du projet montrent notamment que :

- la stratégie aléatoire rapide est nettement plus efficace que la version
  récursive sur les grandes grilles : sur une grille 200×200, elle est environ
  **6 à 7 fois plus rapide** ;
- `max-bordure` réduit fortement le nombre de coups par rapport à une sélection
  aléatoire : par exemple **7 coups au lieu de 16** sur une grille 15×15 à
  4 couleurs, et **19 au lieu de 55** sur une grille 50×50 à 8 couleurs ;
- `max-bordure-zone` améliore généralement encore le résultat, au prix d'un
  calcul légèrement plus important ;
- la stratégie d'anticipation peut obtenir de très bons scores (par exemple
  13 coups au lieu de 16 pour `max-bordure-zone` sur une grille 30×30 à
  6 couleurs), mais elle est coûteuse car elle simule plusieurs possibilités ;
- la stratégie mixte offre un bon compromis : elle obtient un nombre de coups
  quasi identique à l'anticipation tout en étant environ **4 à 5 fois plus
  rapide**.

## Contexte pédagogique

Ce projet met en pratique :

- la récursivité et l'exploration de grille ;
- les listes chaînées ;
- la gestion dynamique de la mémoire en C ;
- l'analyse de la complexité et la comparaison de performances ;
- la conception de stratégies gloutonnes et d'heuristiques ;
- l'utilisation d'une bibliothèque graphique avec SDL2.
