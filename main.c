#include <stdio.h>
#include "structure_som.h"


int main(){

  liste_data *liste_de_vecteur = init_liste_data();
  //afficher_liste(liste_de_vecteur);
  vect_data *vm = vecteur_moyen(liste_de_vecteur);
  afficher_vecteur(vm);

  map *network = init_map(vm);
  afficher_network_map(network);

  return 0;
}
