#include <stdio.h>
#include <time.h>
#include <math.h>
#include "cng.h"

#define NB_COULEURS_PALETTE 18
#define pi M_PI


// Variables globales que l'utilisateur peut initialiser a sa convenance

double zoom;   // taille de la fenetre 1 -> [-1,+1]x[-1,+1] par defaut
double zoomx;  // eventuellement non carree 
double zoomy;
double centrex;  //position du centre de la fenetre (0,0) par defaut
double centrey;
double positionx;  //position du curseur (0,0) par defaut
double positiony;
// double posx3D; //position du curseur 3D (0,0,0) par defaut
// double posy3D;
// double posz3D;


// Fonctions utiles 

void fenetre(double x, double y); // ouvre une fenetre de centre (x,y)
void affiche(void);               // efface le dessin et execute la liste des dernieres instructions graphiques
void afficheplus(void);           // idem sans effacer le dessin
void quitte(void);                // ferme la fenetre correctement
void bouge(double x, double y);   // place le curseur en (x,y)
void trace(double x, double y);   // trace un segment depuis la position courante du curseur jusqu a (x,y)
void couleur(int c);              // change la couleur du stylo en c


