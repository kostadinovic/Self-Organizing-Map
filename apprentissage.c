#include <stdio.h>
#include <math.h>
#include "structure_som.h"

double dist_euclid(double *vect_data, double *vect_neurone, int taille_vect){
  double d = 0;
  for(int i=0; i<taille_vect;i++){
    d += pow(fabs(vect_data[i]-vect_neurone[i]),2);
  }
  return sqrt(d);
}

bmu *alloc_bmu(){
  bmu *bmu_t=(bmu*)(malloc(sizeof(bmu)));
  bmu_t->bmu_ligne=0;
  bmu_t->bmu_colonne=0;
  bmu_t->nom=(char*)malloc(sizeof(char)*50);
  bmu_t->suiv=NULL;
  return bmu_t;
}

bmu *new_bmu(int ligne, int colonne, char *nom){
  bmu *bmu = alloc_bmu();
  bmu->bmu_ligne=ligne;
  bmu->bmu_colonne=colonne;
  bmu->nom=nom;
  return bmu;
}

void ajouter_bmu_fin(bmu *liste_de_bmu, int ligne, int colonne, char *nom){
  while(liste_de_bmu->suiv){
      liste_de_bmu=liste_de_bmu->suiv;
    }
    liste_de_bmu->suiv=new_bmu(ligne,colonne,nom);
}

bmu *trouverBMU(map *network, vect_data *vecteur, int taille_vect){
  bmu *bmu_t;
  bmu_t = new_bmu(0,0,network->Grille[0][0].nom);
  double min = 100;
  double distance_tmp = 0;
  for(int i=0;i<network->longueur;i++){
    for(int j=0;j<network->largeur;j++){
      distance_tmp = dist_euclid(network->Grille[i][j].valeur, vecteur->valeur,taille_vect);
      if(distance_tmp < min){
        min = distance_tmp;
        free(bmu_t);
        bmu_t = NULL;
        bmu_t=new_bmu(i,j,network->Grille[i][j].nom);
      }
      if(distance_tmp == min){
        ajouter_bmu_fin(bmu_t,i,j,network->Grille[i][j].nom);
      }
    }
  }
  return bmu_t;
}

void afficher_bmu_full(bmu *liste_bmu){
  int i=1;
  while(liste_bmu->suiv){
    printf("BMU%d ; ligne=%d ; colonne=%d ; nom=%s\n",i,liste_bmu->bmu_ligne,liste_bmu->bmu_colonne,liste_bmu->nom);
    liste_bmu = liste_bmu->suiv;
    i++;
  }
}

int indice_aleatoire(int min, int max){
  return rand()%(max-min+1)+min;
}

void *shuffle(liste_data *liste){
  int max = liste->nb_lignes;
  for(int i=0;i<max;i++){
    vect_data tmp = liste->data[i];
    int new_i = indice_aleatoire(0,max);
    liste->data[i] = liste->data[new_i];
    liste->data[new_i] = tmp;
  }
}
