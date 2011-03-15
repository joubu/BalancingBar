/************************************************************************
 * 	Auteur : Jonathan Druart					*
 *	initParam.h							*
 *		$ Initialisation des paramètres : 			*
 *			# Taille et positionnement de la viewport 	*
 *			# Taille et positionnement de la window		*
 *			# Taille, positionnement et angle de la barre	*
 ************************************************************************/

#ifndef _INITPARAM
#define _INITPARAM


/************************************************************************
 *	Demande la taille de la viewport (longueur et largeur)		*
 *	ainsi que sa position centrale					*
 ************************************************************************/
void InitViewport(){
	unsigned short longueur, hauteur, x, y;
	printf("Taille de la Viewport ? (longueur hauteur) évitez de dépasser : 900 700 ");
	scanf("%hu %hu", &longueur, &hauteur);
	printf("Position centrale ? (x y) préférez : 500 400 pour un affichage centré ");
	scanf("%hu %hu", &x, &y);
	viewport.min.x = x - longueur / 2;
	viewport.min.y = y - hauteur / 2;
	viewport.max.x = x + longueur / 2;
	viewport.max.y = y + hauteur / 2;
}


/************************************************************************
 *	Demande la taille de la window (longueur et largeur)		*
 *	ainsi que sa position centrale					*
 ************************************************************************/
void InitWindow(){
	unsigned short longueur, hauteur, x, y;
	printf("Taille de la Window ? (longueur hauteur)");
	scanf("%hu %hu", &longueur, &hauteur);
	printf("Position centrale ? (x y) ");
	scanf("%hu %hu", &x, &y);
	window.min.x = x - longueur / 2;
	window.min.y = y - hauteur / 2;
	window.max.x = x + longueur / 2;
	window.max.y = y + hauteur / 2;
}


/************************************************************************
 *	Demande la position initiale de la barre			*
 *	ainsi que la longueur qu'elle aura et son angle de départ	*
 ************************************************************************/
void InitBarre(){
	Point p; // Servira pour la calcul des nouvelles coordonnées dans l'espace écran

	printf("position_debut de la barre ?");
	scanf("%f %f", &p.x, &p.y);
	Pixel pix = NouvCoord(p);
	barre.position_debut.x = pix.x;
	barre.position_debut.y = pix.y;

	printf("longueur et angle de la barre (en degré) ?");
	scanf("%d %lf", &(barre.longueur), &(barre.theta));
	barre.theta = (barre.theta*M_PI)/180; // On calcule l'angle en radians
	p.y = p.x + barre.longueur;
	pix = NouvCoord(p);
	barre.position_fin.x = pix.x;
	barre.position_fin.y = pix.y;

	// Calcul de la distance entre le début et la fin de la barre = longueur de la barre
	barre.longueur = (int)sqrt(pow(barre.position_debut.x - barre.position_fin.x,2) + pow(barre.position_debut.y - barre.position_fin.y,2)); 
}

#endif
