#include "structure_som.h"
#include <string.h>
#include <math.h>


/*      Fonction d'allocation et d'initialisation de ma structure à partir du fichier       */

//alouée mon vecteur de données
vect_data *alloc_vecteur_data(){
  vect_data *vect=(vect_data*)malloc(sizeof(vect_data));
  vect->valeur=(double*)malloc(4*sizeof(double));
  vect->norme=0;
  vect->nom=(char*)malloc(sizeof(char)*50);
  return vect;
}

liste_data *alloc_liste_data(){
  liste_data *liste=(liste_data*)(malloc(sizeof(liste_data)));
  liste->data=NULL;
  liste->nb_lignes=150;
  liste->taille_vec=4;
  liste->suiv=NULL;
  //printf("débug\n");
  //printf("%d\n",liste->nb_lignes);
  //printf("%d\n",liste->taille_vec);
  return liste;
}

liste_data *liste_from_vecteur(vect_data *vect){
  liste_data *liste=alloc_liste_data();
  liste->data=vect;
  return liste;
}

void ajouter_fin_liste(vect_data *vect, liste_data *liste){
  if(liste->data){
    liste_data *tmp = liste;
    while(tmp->suiv){
      tmp=tmp->suiv;
    }
    tmp->suiv=liste_from_vecteur(vect);
  } else {
    liste->data=vect;
  }
}

//grande fonction pour initialiser+normaliser la liste chainée des données
liste_data *init_liste_data(){
  FILE *fichier = fopen("iris_data.txt","r");
  char ligne[600];
  char *token;
  int j=0;

  liste_data *liste = alloc_liste_data();

  for(int i=0;i<liste->nb_lignes;i++){
    fgets(ligne, sizeof(ligne), fichier);
    //printf("ligne = %s",ligne);
    token=strtok(ligne,",");
    //printf("token= %s\n",token);
    vect_data *vecteur = alloc_vecteur_data();
    while(j < liste->taille_vec){
      vecteur->valeur[j] = atof(token);
      token = strtok(NULL,",");
      //printf("token= %s\n",token);
      j++;
    }
    strcpy(vecteur->nom,token);
    token=NULL;
    ajouter_fin_liste(vecteur,liste);
    j=0;
  }
  normaliser_liste(liste);
  return liste;
}

double carre(double x){
  return x*x;
}

double normalise_vect(double *vect, int taille_vec){
  double norme_vect=0;
  for(int i=0;i<taille_vec;i++){
    norme_vect += carre(vect[i]);
  }
  norme_vect=sqrt(norme_vect);
  return norme_vect;
}

void normaliser_liste(liste_data *liste){
  liste_data *tmp=liste;
  for(int i=0;i<tmp->nb_lignes;i++){
    tmp->data->norme=normalise_vect(tmp->data->valeur,tmp->taille_vec);
    if(tmp->suiv){
      tmp=tmp->suiv;
    }
  }
}

vect_data *vecteur_moyen(liste_data *liste){
  vect_data *vecteur_moyen = alloc_vecteur_data();
  double w,x,y,z;
  //int cpt=1;

  for(int i=0;i<liste->nb_lignes;i++){
    if(liste->suiv){
      w += liste->data->valeur[0];
      x += liste->data->valeur[1];
      y += liste->data->valeur[2];
      z += liste->data->valeur[3];
      //cpt++;
      liste=liste->suiv;
    }

  }
  w = w/liste->nb_lignes;
  x = x/liste->nb_lignes;
  y = y/liste->nb_lignes;
  z = z/liste->nb_lignes;
  //printf("w=%f x=%f y=%f z=%f et cpt=%d\n",w,x,y,z,cpt);
  vecteur_moyen->nom="vecteur_moyen";
  vecteur_moyen->valeur[0]=w;
  vecteur_moyen->valeur[1]=x;
  vecteur_moyen->valeur[2]=y;
  vecteur_moyen->valeur[3]=z;
  vecteur_moyen->norme=normalise_vect(vecteur_moyen->valeur,liste->taille_vec);
  return vecteur_moyen;
}

unitN *alloc_unitN(){
  unitN *unit=(unitN*)malloc(sizeof(unitN));
  unit->valeur=(double*)malloc(4*sizeof(double));
  unit->norme=0;
  unit->nom=(char*)malloc(sizeof(char)*50);
  return unit;
}

map *alloc_map(){
  map *map_network=(map*)malloc(sizeof(map));
  map_network->longueur=10;
  map_network->largeur=6;
  map_network->Grille=(unitN**)malloc(10*6*(sizeof(struct unitN)));

  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      map_network->Grille[i]=(unitN*)malloc(map_network->longueur*map_network->largeur*sizeof(unitN*));
    }
  }
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      map_network->Grille[i][j]=*alloc_unitN();
    }
  }
  return map_network;
}

map *init_map(vect_data *vecteur_moyen){
  map *map_network=alloc_map();
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      for(int k=0;k<4;k++){
        map_network->Grille[i][j].valeur[k]=aleatoire(vecteur_moyen->valeur[k] -0.02, vecteur_moyen->valeur[k] +0.05);
      }
    }
  }
  return map_network;
}

/*        Fonctions de débug  ou très simples       */

void afficher_vecteur(vect_data *vect){
  for(int i=0;i<4;i++){
    printf("%f;",vect->valeur[i]);
  }
  printf("\n");
  printf("%f\n",vect->norme);
  printf("%s\n",vect->nom);
}

void afficher_liste(liste_data *liste){
  while(liste){
    afficher_vecteur(liste->data);
    liste=liste->suiv;
  }
}

void afficher_unitN(unitN *unit){
  for(int i=0;i<4;i++){
    printf("%f;",unit->valeur[i]);
  }
  printf("\n");
  printf("%f\n",unit->norme);
  printf("%s\n",unit->nom);
}

void afficher_network_map(map *map_network){
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      //printf("i=%d et j=%d\n",i,j);
      afficher_unitN(&map_network->Grille[i][j]);
    }
  }
}

double aleatoire(double min, double max){
  return min+((max-min)*(rand()/(double)RAND_MAX));
}
