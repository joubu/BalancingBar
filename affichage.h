/****************************************************************************************	
 * 	Auteur : Jonathan Druart							*
 * 	affichage.h									*
 * 		$ Affichage de la fenetre						*
 *			# Affiche la viewport au commencement				*
 *			# Et à chaque fois qu'un bord à été touché par la barre		*
 * 		$ Affichage de la barre							*
 * 			# Efface la barre de sa position précédente			*
 * 			# Affiche la barre à sa position courante			*
 *		$ Remplir rectangle							*
 *			# Rempli un rectangle avec la couleur courante			*
 *		$ Afficher les scores et l'angle					*
 *			# Affichage des différents scores à leur position		*
 *			# Affichage de l'angle courant					*
 *			# Affichage de nombre de fps (sur la sortie standard)		*
 *		$ Afficher le coef							*
 *			# Affichage du coefficient multiplicateur en bas à gauche	*
 ****************************************************************************************/




#ifndef _AFFICHAGE
#define _AFFICHAGE


/****************************************************************************************
 * 	Affichage en blanc de la fenetre (viewport) grace au tracé de Bresenham		*
 ****************************************************************************************/
void AfficherFenetre(){
	cng_current_color(255,255,255);
	Bresenham(viewport.min.x , viewport.min.y , viewport.min.x , viewport.max.y);
	Bresenham(viewport.min.x , viewport.min.y , viewport.max.x , viewport.min.y);
	Bresenham(viewport.max.x , viewport.min.y , viewport.max.x , viewport.max.y);
	Bresenham(viewport.max.x , viewport.max.y , viewport.min.x , viewport.max.y);
}


/****************************************************************************************
 * 	On efface la barre de sa position précédente (affichage en noir)		*
 * 	(Avant recalcule de sa nouvelle position)					*
 ****************************************************************************************/
void EffacerBarre(){
	cng_current_color(0,0,0);
	Bresenham(barre.position_debut.x, barre.position_debut.y-TAILLE_MAIN, barre.position_debut.x-TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN);
	Bresenham(barre.position_debut.x-TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN, barre.position_debut.x+TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN);
	Bresenham(barre.position_debut.x+TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN, barre.position_debut.x, barre.position_debut.y-TAILLE_MAIN);

	Bresenham(barre.position_debut.x, barre.position_debut.y, barre.position_fin.x, barre.position_fin.y);

}


/****************************************************************************************
 * 	On affiche la barre à sa position courante (affichage en blanc)			*
 * 	(Après recalcule de sa nouvelle position)					*
 ****************************************************************************************/
void AfficherBarre(){
	cng_current_color(255,0,0);
	Bresenham(barre.position_debut.x, barre.position_debut.y-TAILLE_MAIN, barre.position_debut.x-TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN);
	Bresenham(barre.position_debut.x-TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN, barre.position_debut.x+TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN);
	Bresenham(barre.position_debut.x+TAILLE_MAIN, barre.position_debut.y+TAILLE_MAIN, barre.position_debut.x, barre.position_debut.y-TAILLE_MAIN);

	Bresenham(barre.position_debut.x, barre.position_debut.y, barre.position_fin.x, barre.position_fin.y);
}


/****************************************************************************************
 *	Trace un rectangle de la couleur courante avec a et b 				*
 *	les deux points de la diagonale.						*
 ****************************************************************************************/
void RemplirRectangle(unsigned int aX, unsigned int aY, unsigned int bX, unsigned int bY){
	int i;
	for (i = aY ; i <= bY ; i++){
		Bresenham(aX, i, bX, i);
	}
}


/****************************************************************************************
 * 	Affichage des scores et de la valeur de l'angle courant et des fps		*
 * 	Au premier appel, affichage des étiquettes (Courant, max, total, angle, fps)	*
 * 	Après, affiche les scores en fonction de l'angle de la barre			*
 ****************************************************************************************/
void AfficherInfos(Scores* lesScores, unsigned char booleen){

	char* chaine;
	chaine = (char*)malloc(sizeof(char)*53);

	// Si c'est le premier appel, on affiche le score max qui était contenu dans le fichier des scores
	if (!booleen){
		sprintf(chaine, "%s %d", "MaxTot : ", lesScores->MaxTot);
		cng_draw_string(chaine, viewport.max.x - cng_string_width(chaine), 5);
	}

	sprintf(chaine, "%s %d %s %d %s %d", "Score courant : ", lesScores->Courant, ", Score Max : ", lesScores->Max, " | Angle : ", Degre(barre.theta));
	cng_draw_string(chaine, viewport.min.x, 5);
	RemplirRectangle(viewport.min.x, 5, viewport.min.x + cng_string_width(chaine), 20);

	// Affichage des fps
	printf("FPS : %d\n", (int)(1000 / cng_get_time()));
	printf("\033[1A");
}


/****************************************************************************************
 * 	Affichage du coefficient multiplicateur en bas à gauche du viewport		*
 ****************************************************************************************/
AfficherCoef(){
	char* chaine;
	chaine = (char*)malloc(sizeof(char)*10);
	sprintf(chaine, "%s" "%.2f", "coef : ", coef);
	RemplirRectangle(viewport.min.x, viewport.min.y + 5, viewport.min.x + cng_string_width(chaine) + 5 , viewport.min.y + 20);
	cng_draw_string(chaine, viewport.min.x, viewport.min.y + 5);
}

#endif
