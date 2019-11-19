#include <stdio.h>
#include <stdlib.h>

/*              Structure pour mes données issues du fichier        */
typedef struct vect_data{
  double *valeur; //de taille 4 contenant les données double du fichier iris
  double norme;
  char *nom; //5 eme colonnes du fichier
}vect_data;

typedef struct liste_data{
  struct vect_data *data;
  int nb_lignes;
  int taille_vec;
  struct liste_data *suiv;
}liste_data;


/*            Structure de mon réseaux de neurones            */

typedef struct unitN{
  double *valeur; //valeur du vecteur dim4 a init avec vect_moyen+écart random
  double norme;
  char *nom;
}unitN;

typedef struct map{ //la map où placer les unitN
  struct unitN **Grille; // longueur*largeur
  int longueur;
  int largeur;
  //int *shuffle;
}map;

//structure pour placer la position du meilleure unit
typedef struct bmu{
  int bmu_ligne;
  int bmu_colonne;
  double norme;
  char *nom;
  struct bmu *suiv; //si d'autres bmu existe
}bmu;





// définitions des fonctions
vect_data *alloc_vecteur_data();
liste_data *alloc_liste_data();
liste_data *init_liste_data();
liste_data *liste_from_vecteur(vect_data *vect);
void ajouter_fin_liste(vect_data *vect, liste_data *liste);
void afficher_vecteur(vect_data *vect);
void afficher_liste(liste_data *liste);

double carre(double x);
double normalise_vect(double *vect, int taille_vec);
void normaliser_liste(liste_data *liste);

vect_data *vecteur_moyen(liste_data *liste);

unitN *alloc_unitN();
map *alloc_map();
void afficher_unitN(unitN *unit);
void afficher_network_map(map *map_network);
map *init_map(vect_data *vecteur_moyen);
double aleatoire(double min, double max);
