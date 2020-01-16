#include <stdio.h>
#include <math.h>
#include "structure_som.h"
#include <time.h>
#include <string.h>

double dist_euclid(double *vect_data, double *vect_neurone, int taille_vect){
  double d = 0.0;
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

bmu *trouverBMU(map *network, vect_data *vecteur, int taille_vect,int ii, int jj){
  bmu *bmu_t = NULL;
  //bmu_t = new_bmu(0,0,network->Grille[0][0].nom);
  double min = 100;
  double distance_tmp = 0;
  for(int i=0;i<network->longueur;i++){
    for(int j=0;j<network->largeur;j++){
      if(vecteur->valeur == NULL){
        printf("BREAK : i=%d   j=%d\n",ii,jj);
        printf("vect data null\n");
        break;
      }
      distance_tmp = dist_euclid(network->Grille[i][j].valeur, vecteur->valeur,taille_vect);
      //printf("MIN = %f, TMP = %f\n",min,distance_tmp);
      if(distance_tmp == min){ //pas de cas dans mes essais
        //printf("ON ADD LISTE\n");
        ajouter_bmu_fin(bmu_t,i,j,network->Grille[i][j].nom);
      }
      if(distance_tmp < min){
        //printf("ON FREE LA LISTE\n");
        min = distance_tmp;
        free(bmu_t);
        bmu_t = NULL;
        bmu_t=new_bmu(i,j,network->Grille[i][j].nom);
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

int compter_nb_bmu(bmu *liste_bmu){
  int i=1;
  while(liste_bmu->suiv){
    i++;
    liste_bmu = liste_bmu->suiv;
  }
  return i;
}

int alea(int max){
  srand(time(NULL));
  int r = rand()%max+1;
  //printf("%d",r);
  return r;
}

bmu *choisir_le_best(bmu *liste_bmu){
  int nb_bmu = compter_nb_bmu(liste_bmu);
  if(nb_bmu>1){
    int r = alea(nb_bmu);
    //printf("\nnb = %d\nR =%d\n",nb_bmu,r);
    int j=1;
    while(j!=r && liste_bmu->suiv){
      liste_bmu = liste_bmu->suiv;
      j++;
    }
    return new_bmu(liste_bmu->bmu_ligne,liste_bmu->bmu_colonne,liste_bmu->nom);
  }
  return liste_bmu;
}

void afficher_bmu_full(bmu *liste_bmu){
  int i=1;
  while(liste_bmu->suiv){
    printf("BMU%d ; ligne=%d ; colonne=%d ; nom=%s\n",i,liste_bmu->bmu_ligne,liste_bmu->bmu_colonne,liste_bmu->nom);
    liste_bmu = liste_bmu->suiv;
    i++;
  }
  printf("BMU%d ; ligne=%d ; colonne=%d ; nom=%s\n",i,liste_bmu->bmu_ligne,liste_bmu->bmu_colonne,liste_bmu->nom);
}

int indice_aleatoire(int min, int max){
  return rand()%(max-min)+min;
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
  return (network->longueur*network->largeur)*0.5;
}

void voisinage(bmu *best, map *network, int rayon, double alpha, vect_data *v, int taille_vec){
  double *vecteur = v->valeur;
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
        strcpy(network->Grille[ligne_inf][colonne_inf].nom, v->nom);
      }
    }
  }
}


void apprentissage(liste_data *donnees, map *network){

  bmu *bmu_liste;
  bmu *best;
  int taille_du_vecteur=donnees->taille_vec;
  int nb_vect=donnees->nb_lignes;
  int nb_it_total=(500*taille_du_vecteur);
  int phase2=3*nb_it_total/4;
  double alpha_init=aleatoire(0.7,0.9);
  double alpha=0.0;
  double ca= 0.0;
  int rayon;
  //printf("NB VECT : %d",nb_vect);
  for(int i=0;i< nb_it_total; i++){
    shuffle(donnees);

    if(i <=phase2){
      ca = (double)(i/nb_it_total); //pour mettre à jour le coeff d'APPRENTISSAGE
      alpha = alpha_init*(1-ca);
      rayon = calculer_rayon(network);
      for(int j=0;j<nb_vect-1;j++){
        //printf("apprentissage avant bmu:  i = %d et j = %d\n", i,j);
        bmu_liste = trouverBMU(network, &donnees->data[j], taille_du_vecteur,i,j);
        //printf("apprentissage après bmu :  i = %d et j = %d\n", i,j);
        if(bmu_liste == NULL) printf("ERREUR LISTE DE BMU VIDE : i=%d , j=%d",i,j);
        best = choisir_le_best(bmu_liste);
        voisinage(best,network,rayon,alpha,&donnees->data[j],taille_du_vecteur);
      }
    }
    else{
      ca = (double)(i/nb_it_total); //pour mettre à jour le coeff d'APPRENTISSAGE
      alpha = alpha_init*(1-ca)*0.1; //juste en phase2
      rayon = 1;
      for(int j=0;j<nb_vect;j++){
        bmu_liste = trouverBMU(network, &donnees->data[j], taille_du_vecteur,i,j);
        best = choisir_le_best(bmu_liste);
        voisinage(best,network,rayon,alpha,&donnees->data[j],taille_du_vecteur);
      }
    }
    update_name(network);
    afficher_network_map(network);
  }
}
