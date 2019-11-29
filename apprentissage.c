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

void liberer_liste_bmu(bmu *mon_bmu){
  if(mon_bmu == NULL) return;
  while(mon_bmu){
    bmu *tmp= mon_bmu->suiv;
    //if(mon_bmu->nom == NULL) return;
    //free(mon_bmu->nom);
    //if(mon_bmu == NULL) return;
    free(mon_bmu);
    mon_bmu=tmp;
  }
  mon_bmu=NULL;
}

/*
bmu *trouverBMUFinal(map *network, liste_data *ma_liste){
  bmu *bmu_t;
  bmu_t = new_bmu(0,0,network->Grille[0][0].nom);
  double min = 100;
  double distance_tmp = 0;
  for(int i=0;i<network->longueur;i++){
    for(int j=0;j<network->largeur;j++){
      distance_tmp = dist_euclid(network->Grille[j][k].valeur, ma_liste->data[i].valeur,ma_liste->taille_vec);
        if(distance_tmp < min){
          printf("DEBUG1 %d,%d,%d\n",i,j,k);
          min = distance_tmp;
          liberer_liste_bmu(bmu_t);
          bmu_t=new_bmu(i,j,network->Grille[i][j].nom);
        }
        if(distance_tmp == min){
          if(bmu_t){
            ajouter_bmu_fin(bmu_t,i,j,network->Grille[i][j].nom);
          }else{
            //bmu_t=new_bmu(i,j,network->Grille[i][j].nom);
          }
        }
        afficher_bmu_full(bmu_t);
      }
    }
  }
  return bmu_t;
}
*/ //fonction inutile on veut pas faire ca à reutiliser dans le bloc appretentissage



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

void shuffle(liste_data *liste){
  int max = liste->nb_lignes;
  for(int i=0;i<max;i++){
    vect_data tmp = liste->data[i];
    int new_i = indice_aleatoire(0,max);
    liste->data[i] = liste->data[new_i];
    liste->data[new_i] = tmp;
  }
}

int calculer_rayon(map *network){
  int nb_unit = (network->longueur*network->largeur)*0.5;
  //printf("%d",nb_unit);
  int rayon=0;
  int nb_voisinage=0;
  while(rayon<nb_unit){
    rayon++;
    nb_voisinage = nb_voisinage + (rayon*8);
  }
  return rayon;
}

void voisinage(bmu *best, map *network, int rayon, double alpha, double *vecteur, int taille_vec){
  int ligne_inf = best->bmu_ligne - rayon;
  int colonne_inf = best->bmu_colonne - rayon;
  int ligne_sup = best->bmu_ligne + rayon;
  int colonne_sup = best->bmu_colonne + rayon;

  if(ligne_inf<0) ligne_inf=0;
  if(colonne_inf<0) colonne_inf=0;
  if(ligne_sup>network->longueur) ligne_sup= network->longueur-1;
  if(colonne_sup>network->largeur) colonne_sup= network->largeur-1;

  for(; ligne_inf<=ligne_sup; ligne_inf++ ){ //for bizarre mais pour éviter l'erreur -Wunused-value (merci stackoverflow)
    for(; colonne_inf<=colonne_sup;colonne_inf++){
      for(int k=0;k<taille_vec;k++){ //enfin j'applique la formule d'apprentissage du cours
        network->Grille[ligne_inf][colonne_inf].valeur[k] += alpha * (vecteur[k] - network->Grille[ligne_inf][colonne_inf].valeur[k]);
        //network->Grille[ligne_inf][colonne_inf].nom="DEBUG: JE CHANGE MES VOISINS ICI";
      }
    }
  }
}

void apprentissage(liste_data *donnees, int nb_vect, int taille_vect,map *network,){

  bmu *best;

  int nb_iterations = (500*taille_vect*0.25);
  double alpha_initial = aleatoire(0.7,0.9);

  for(int i=0; i<nb_iterations;i++){
    if(rayon >)
  }
}
