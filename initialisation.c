#include "structure_som.h"
#include <string.h>
#include <math.h>


/*      Fonction d'allocation et d'initialisation de ma structure à partir du fichier       */

//alouée mon vecteur de données
vect_data *alloc_vecteur_data(int taille_vec){
  vect_data *vect=(vect_data*)malloc(sizeof(vect_data));
  vect->valeur=(double*)malloc(taille_vec*sizeof(double));
  vect->norme=0;
  vect->nom=(char*)malloc(sizeof(char)*50);
  return vect;
}

liste_data *alloc_liste_data(int nb_lignes_file, int taille_vec_file){
  liste_data *liste=(liste_data*)(malloc(sizeof(liste_data)));
  liste->data=(vect_data*)(malloc(sizeof(vect_data)*nb_lignes_file));
  liste->nb_lignes=nb_lignes_file; //pas dynamique il faut pas mettre de constante
  liste->taille_vec=taille_vec_file; //idem que en haut
  //printf("débug\n");
  //printf("%d\n",liste->nb_lignes);
  //printf("%d\n",liste->taille_vec);
  return liste;
}

/* PLUS BESOIN CAR PAS LISTE CHAINEE MAIS TABLEAU
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

*/

//grande fonction pour initialiser+normaliser la liste chainée des données
liste_data *init_liste_data(char *nom_fichier){
  int nb_lignes_file = nombreLigne(nom_fichier);
  FILE *fichier = fopen(nom_fichier,"r");
  printf("nb ligne = %d",nb_lignes_file);
  int taille_vec_file = 4; //a calcler fonction i dont know comment
  if(fichier==NULL || nb_lignes_file==0 ){
    printf("Erreur ouverture de fichier");
  }
  char ligne[600];
  char *token;
  char *end;
  int j=0;

  liste_data *liste = alloc_liste_data(nb_lignes_file, taille_vec_file);

  for(int i=0;i<liste->nb_lignes;i++){
    fgets(ligne, sizeof(ligne), fichier);
    //printf("ligne = %s",ligne);
    token=strtok(ligne,",");
    //printf("token= %s\n",token);
    vect_data *vecteur = alloc_vecteur_data(taille_vec_file);
    while(j < liste->taille_vec){
      vecteur->valeur[j] = strtod(token,&end); //strtod
      token = strtok(NULL,",");
      //printf("token= %s\n",token);
      j++;
    }
    strcpy(vecteur->nom,token);
    token=NULL;
    liste->data[i]=*vecteur; //ajout du vect ds tableau
    j=0;
  }
  normaliser_liste(liste);
  return liste;
}

double normalise_vect(double *vect, int taille_vec){
  double norme_vect=0;
  for(int i=0;i<taille_vec;i++){
    norme_vect += pow(vect[i],2);
  }
  norme_vect=sqrt(norme_vect);
  return norme_vect;
}

void normaliser_liste(liste_data *liste){
  for(int i=0;i<liste->nb_lignes;i++){
    liste->data[i].norme=normalise_vect(liste->data[i].valeur,liste->taille_vec);
  }
}

vect_data *vecteur_moyen(liste_data *liste){
  int taille_vecteur=liste->taille_vec;
  int nb_lignes=liste->nb_lignes;
  vect_data *vecteur_moyen = alloc_vecteur_data(taille_vecteur);
  double tab[taille_vecteur];
  //int cpt=0;

  for(int i=0;i<liste->nb_lignes;i++){
    for(int j=0;j<taille_vecteur;j++){
      tab[j] += liste->data[i].valeur[j];
    }
    //cpt++;
  }
  for(int i=0;i<taille_vecteur;i++){
    tab[i] = tab[i]/nb_lignes;
  }
  //printf("w=%f x=%f y=%f z=%f et cpt=%d\n",w,x,y,z,cpt);
  vecteur_moyen->nom="vecteur_moyen";
  for(int i=0;i<taille_vecteur;i++){
    vecteur_moyen->valeur[i] = tab[i];
  }
  vecteur_moyen->norme=normalise_vect(vecteur_moyen->valeur,taille_vecteur);
  return vecteur_moyen;
}

unitN *alloc_unitN(int taille_vecteur){
  unitN *unit=(unitN*)malloc(sizeof(unitN));
  unit->valeur=(double*)malloc(taille_vecteur*sizeof(double));
  unit->nom=(char*)malloc(sizeof(char)*50);
  unit->dist=0;
  return unit;
}

map *alloc_map(int taille_vecteur){
  map *map_network=(map*)malloc(sizeof(map));
  map_network->longueur=10;
  map_network->largeur=6;
  map_network->Grille=(unitN**)malloc(10*6*(sizeof(struct unitN))); //faux

  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      map_network->Grille[i]=(unitN*)malloc(map_network->longueur*map_network->largeur*sizeof(unitN*));
    }
  }
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      map_network->Grille[i][j]=*alloc_unitN(taille_vecteur);
    }
  }
  return map_network;
}

map *init_map(vect_data *vecteur_moyen, int taille_vecteur){
  map *map_network=alloc_map(taille_vecteur);
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
  int i;
  for(i=0;i<liste->nb_lignes;i++){
    afficher_vecteur(&liste->data[i]);
  }
  //printf("nombre total de vecteur = %d\n",i);
}

void afficher_unitN(unitN *unit){
  for(int i=0;i<4;i++){
    printf("%f;",unit->valeur[i]);
  }
  printf("\n");
  printf("%s\n",unit->nom);
  printf("%f\n",unit->dist);
}

void afficher_network_map(map *map_network){
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      //printf("i=%d et j=%d\n",i,j);
      afficher_unitN(&map_network->Grille[i][j]);
      printf("\n");
    }
  }
}

double aleatoire(double min, double max){
  return min+((max-min)*(rand()/(double)RAND_MAX));
}

int nombreLigne(char *fichier){
  FILE *f = fopen(fichier,"r");
  int lignes=0;
  size_t taille =0;
  char *tmp = malloc(taille);
  while(getline(&tmp,&taille,f) != -1){
    lignes++;
  }
  if(tmp) free(tmp);
  fclose(f);
  return lignes;
}
