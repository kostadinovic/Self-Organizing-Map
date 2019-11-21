#include <stdio.h>
#include <stdlib.h>

/*              Structure pour mes données issues du fichier        */
typedef struct vect_data{
  double *valeur; //de taille 4 contenant les données double du fichier iris
  double norme;
  char *nom; //5 eme colonnes du fichier
}vect_data;

typedef struct liste_data{ //faire un tableau car plus simple et on va pas changer la taille donc liste inutile
  struct vect_data *data;
  int nb_lignes;
  int taille_vec;
}liste_data;


/*            Structure de mon réseaux de neurones            */

typedef struct unitN{
  double *valeur; //valeur du vecteur dim4 a init avec vect_moyen+écart random
  char *nom;
  double dist;
}unitN;

typedef struct map{ //la map où placer les unitN
  struct unitN **Grille; // longueur*largeur
  int longueur;
  int largeur;
}map;

/* premier rang de voisin 8 puis 16/24.. <30
rayon = 1 puis 2 puis 3



 */

//structure pour placer la position du meilleure unit
typedef struct bmu{ // initialiser a 0,0 car le premier et le bmu a l'init
  int bmu_ligne;
  int bmu_colonne;
  double norme;
  char *nom;
  struct bmu *suiv; //si d'autres bmu existe
}bmu;





// définitions des fonctions
vect_data *alloc_vecteur_data(int taille_vec); //OK
liste_data *alloc_liste_data(int nb_lignes_file, int taille_vec_file); //OK
liste_data *init_liste_data(char *fichier); //OK
liste_data *liste_from_vecteur(vect_data *vect); //OK
void ajouter_fin_liste(vect_data *vect, liste_data *liste); //OK
void afficher_vecteur(vect_data *vect); //OK
void afficher_liste(liste_data *liste); //OK
int nombreLigne(char *fichier); //OK

double normalise_vect(double *vect, int taille_vec); //OK
void normaliser_liste(liste_data *liste); //OK

vect_data *vecteur_moyen(liste_data *liste); //OK

unitN *alloc_unitN(int taille_vec);
map *alloc_map(int taille_vec);
void afficher_unitN(unitN *unit);
void afficher_network_map(map *map_network);
map *init_map(vect_data *vecteur_moyen, int taille_vecteur);
double aleatoire(double min, double max);
