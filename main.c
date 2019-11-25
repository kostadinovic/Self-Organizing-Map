#include <stdio.h>
#include "structure_som.h"


int main(){
  //int taille_des_vect = tailleVect("iris_data.txt");
  liste_data *liste_de_vecteur = init_liste_data("iris_data.txt");
  afficher_liste(liste_de_vecteur);
  /* vect_data *vm = vecteur_moyen(liste_de_vecteur);
  afficher_vecteur(vm,taille_des_vect);
  map *network = init_map(vm,taille_des_vect);
  afficher_network_map(network);
  vect_data *v= &liste_de_vecteur->data[0];
  bmu *liste_bmu = trouverBMU(network,v,taille_des_vect);
  afficher_bmu_full(liste_bmu);
  */
  printf("################\n");
  shuffle(liste_de_vecteur);
  afficher_liste(liste_de_vecteur);

  return 0;
}
