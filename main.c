#include <stdio.h>
#include "structure_som.h"


int main(){

  //Initialiser mes données et les normaliser
  int taille_des_vect = tailleVect("iris_data.txt");
  liste_data *donnees = init_liste_data("iris_data.txt");
  vect_data *vm = vecteur_moyen(donnees);
  normaliser_liste(donnees,taille_des_vect);
  //afficher_liste(donnees);


  //Crée mon réseau de neurones
  map *network = init_map(vm,taille_des_vect);
  //printf("La map avant la phase d'apprentissage : \n");
  //afficher_map_debug(network);
/*
  bmu *bmu_liste;
  for(int j=0;j<1000;j++){
    for(int i=0;i<donnees->nb_lignes;i++){
      bmu_liste = trouverBMU(network,&donnees->data[i],taille_des_vect);
      //afficher_bmu_full(bmu_liste);
      //printf("\n");
    }
  }
  printf("BMU FINALE:\n");
  afficher_bmu_full(bmu_liste);
*/


  //Partie apprentissage

  apprentissage(donnees,network);
  printf("La map après la phase d'apprentissage : \n");
  afficher_network_map(network);


  return 0;
}
