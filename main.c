#include <stdio.h>
#include "structure_som.h"


int main(){

  //Initialiser mes données et les normaliser
  int taille_des_vect = tailleVect("iris_data.txt");
  liste_data *donnees = init_liste_data("iris_data.txt");
  vect_data *vm = vecteur_moyen(donnees);
  normaliser_liste(donnees,taille_des_vect);


  //Crée mon réseau de neurones
  map *network = init_map(vm,taille_des_vect);
  printf("La map avant la phase d'apprentissage : \n");
  afficher_network_map(network);


  //Partie apprentissage

  apprentissage(donnees,network);
  printf("La map après la phase d'apprentissage : \n");
  afficher_network_map(network);
  


  return 0;
}
