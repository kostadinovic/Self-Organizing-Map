#include "structure_som.h"
#include <string.h>
#include <math.h>
#include <time.h>


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
  liste->nb_lignes=nb_lignes_file; //enfin dynamique
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
  //printf("nb ligne = %d\n",nb_lignes_file); //debug
  int taille_vec_file = tailleVect(nom_fichier);
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
      vecteur->valeur[j] = strtod(token,&end);
      token = strtok(NULL,",");
      //printf("token= %s\n",token);
      j++;
    }
    strcpy(vecteur->nom,token);
    token=NULL;
    liste->data[i]=*vecteur; //ajout du vect ds tableau
    j=0;
  }
  //normaliser_liste(liste, taille_vec_file);
  return liste;
}

double calculer_norme(double *vect, int taille_vec){
  double norme_vect=0;
  double tmp =0;
  for(int i=0;i<taille_vec;i++){
    tmp += pow(vect[i],2);
  }
  norme_vect=sqrt(tmp);
  return norme_vect;
}

void normaliser_liste(liste_data *liste, int taille){
  //int taille = liste->taille_vec;
  for(int i=0;i<liste->nb_lignes;i++){
    liste->data[i].norme=calculer_norme(liste->data[i].valeur,taille);
    for(int j=0;j<liste->taille_vec;j++){
      liste->data[i].valeur[j] = (liste->data[i].valeur[j])/(liste->data[i].norme);
    }
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
  vecteur_moyen->norme=calculer_norme(vecteur_moyen->valeur,taille_vecteur);
  //printf("LA NORME == %f",vecteur_moyen->norme);
  for(int i=0;i<taille_vecteur;i++){
    double n = vecteur_moyen->valeur[i]/(vecteur_moyen->norme);
    vecteur_moyen->valeur[i] = n;
  }
  return vecteur_moyen;
}

unitN *alloc_unitN(int taille_vecteur){
  unitN *unit=(unitN*)malloc(sizeof(unitN));
  unit->valeur=(double*)malloc(taille_vecteur*sizeof(double));
  unit->nom=(char*)malloc(sizeof(char)*50);
  return unit;
}

//vérifier initalisation de la taille de map avec le prof
map *alloc_map(int taille_vecteur){
  map *map_network=(map*)malloc(sizeof(map));
  map_network->longueur=6; //lignes
  map_network->largeur=10; //colonnes
  map_network->Grille=(unitN**)malloc(map_network->largeur*(sizeof(unitN*))); //ALLOCATION OK MTN

  for(int i=0;i<map_network->longueur;i++){
    map_network->Grille[i]=(unitN*)malloc(map_network->largeur*sizeof(unitN));
  }

  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      map_network->Grille[i][j]=*alloc_unitN(taille_vecteur);
    }
  }
  return map_network;
}

map *init_map(vect_data *vecteur_moyen, int taille_vecteur){
  //int cpt=0;
  map *map_network=alloc_map(taille_vecteur);
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      for(int k=0;k<taille_vecteur;k++){
        map_network->Grille[i][j].valeur[k]=aleatoire(vecteur_moyen->valeur[k] -0.02, vecteur_moyen->valeur[k] +0.05);
        strcpy(map_network->Grille[i][j].nom,"*");
      }
      //cpt++;
    }
  }
  //printf("cpt = %d\n",cpt);
  return map_network;
}

/*        Fonctions de débug  ou très simples       */

void afficher_vecteur(vect_data *vect,int taille){
  for(int i=0;i<taille;i++){
    printf("%f;",vect->valeur[i]);
  }
  printf("\n");
  printf("%f\n",vect->norme);
  printf("%s\n",vect->nom);
}

void afficher_liste(liste_data *liste){
  int i;
  for(i=0;i<liste->nb_lignes;i++){
    afficher_vecteur(&liste->data[i], liste->taille_vec);
  }
  //printf("nombre total de vecteur = %d\n",i);
}

void afficher_unitN(unitN *unit,int taille){
  for(int i=0;i<taille;i++){
    printf("%f;",unit->valeur[i]);
  }
  printf("\n");
  printf("%s\n",unit->nom);
}

void afficher_network_map_full(map *map_network, int taille){
  //int cpt=0;
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      printf("i=%d et j=%d\n",i,j);
      afficher_unitN(&map_network->Grille[i][j],taille);
      printf("\n");
      //cpt++;
    }
  }
  //printf("%d\n",cpt);
}

void afficher_map_debug(map *map_network){ //place DEBUG
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      afficher_unitN(&map_network->Grille[i][j],4);
    }
    printf("\n");
  }

}
void afficher_network_map(map *map_network){
  for(int i=0;i<map_network->longueur;i++){
    for(int j=0;j<map_network->largeur;j++){
      printf("%s", map_network->Grille[i][j].nom);
    }
    printf("\n");
  }
}

double aleatoire(double min, double max){
  srand(time(NULL)+rand());
  return min+((max-min)*(rand()/(double)RAND_MAX));
}

int nombreLigne(char *fichier){
  FILE *f = fopen(fichier,"r");
  int lignes=0;
  size_t taille =0;
  char *buffer = malloc(taille);
  while(getline(&buffer,&taille,f) != -1){
    lignes++;
  }
  if(buffer) free(buffer);
  fclose(f);
  return lignes;
}

int tailleVect(char *fichier){
  FILE *f = fopen(fichier,"r");
  int nbVec=0;
  size_t taille =0;
  char *buffer = malloc(taille);
  char *tmp = malloc(taille);

  if(getline(&buffer,&taille,f) != -1){
    while(1){ //comment éviter cette boucle je sais PAS
      if(nbVec==0){
        tmp = strtok(buffer, ",;:");
      }
      else{
        tmp = strtok(NULL,",:;");
      }
      if(tmp==NULL){
        break;
      }
      else{
        nbVec++;
      }
    }
  }
  return nbVec-1; //je lui enlève le nom faire autrement?
}
