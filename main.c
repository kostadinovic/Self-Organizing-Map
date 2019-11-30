#include <stdio.h>
#include "structure_som.h"


int main(){

  int taille_des_vect = tailleVect("iris_data.txt");
  liste_data *liste_de_vecteur = init_liste_data("iris_data.txt");
  vect_data *vm = vecteur_moyen(liste_de_vecteur);
  normaliser_liste(liste_de_vecteur,taille_des_vect);

  map *network = init_map(vm,taille_des_vect);

  bmu *liste_bmu = new_bmu(2, 3, "nem");
  ajouter_bmu_fin(liste_bmu, 5, 6, "kos");
  ajouter_bmu_fin(liste_bmu, 8, 9, "KOSKA");
  afficher_bmu_full(liste_bmu);
  bmu *best = choisir_le_best(liste_bmu);
  printf("################\n");
  afficher_bmu_full(best);
  printf("################\n");



  return 0;
}
