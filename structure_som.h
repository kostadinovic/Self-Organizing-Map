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
  char *nom;
  struct bmu *suiv; //si d'autres bmu existe
}bmu;





// définitions des fonctions

//PARTIE INITIALISATIONS
vect_data *alloc_vecteur_data(int taille_vec); //OK
liste_data *alloc_liste_data(int nb_lignes_file, int taille_vec_file); //OK
liste_data *init_liste_data(char *fichier); //OK
liste_data *liste_from_vecteur(vect_data *vect); //OK
void ajouter_fin_liste(vect_data *vect, liste_data *liste); //OK
void afficher_vecteur(vect_data *vect,int taille); //OK
void afficher_liste(liste_data *liste); //OK
int nombreLigne(char *fichier); //OK
int tailleVect(char *fichier); //OK

double calculer_norme(double *vect, int taille_vec); //OK
double normalise_vect(double *vect, int taille_vec); //OK
void normaliser_liste(liste_data *liste, int taille); //OK

vect_data *vecteur_moyen(liste_data *liste); //OK

unitN *alloc_unitN(int taille_vec);
map *alloc_map(int taille_vec);
void afficher_unitN(unitN *unit,int taille);
void afficher_network_map_full(map *map_network, int taille);
void afficher_network_map(map *map_network);
map *init_map(vect_data *vecteur_moyen, int taille_vecteur);
double aleatoire(double min, double max);
void afficher_map_debug(map *map_network);
void update_name(map *map_network);

//PARTIE APPRENTISSAGE
double dist_euclid(double *vect_data, double *vect_neurone, int taille_vect); //OK
bmu *alloc_bmu(); //OK
bmu *new_bmu(int ligne, int colonne, char *nom); //OK
void ajouter_bmu_fin(bmu *liste_de_bmu, int ligne, int colonne, char *nom); //OK
bmu *trouverBMU(map *network, vect_data *vecteur, int taille_vect, int i, int j); //OK
int compter_nb_bmu(bmu *liste_bmu); //OK
int alea(int max); //OK
void afficher_bmu_full(bmu *liste_bmu); //OK
bmu *choisir_le_best(bmu *liste_bmu); //OK

int indice_aleatoire(int min, int max); //OK
void shuffle(liste_data *liste); //OK
int calculer_rayon(map *network); //OK


void voisinage(bmu *best, map *network, int rayon, double alpha, vect_data *v, int taille_vec); //OK
void apprentissage(liste_data *donnees, map *network); //FONCTION FINALE Ok partiel
