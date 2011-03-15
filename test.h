/****************************************************************************************	
 * 	Auteur : Jonathan Druart							*
 * 	test.h										*
 * 		$ Si on est dans la main						*
 * 			# Renvoie vrai si on a bien cliqué dans la main			*
 * 		$ Test de la souris							*
 * 			# Si on l'a pressée						*
 * 			# Si on l'a relachée						*
 * 		$ Test de la barre							*
 * 			# Réagit en fonction des bords					*
 ****************************************************************************************/

#ifndef _TEST
#define _TEST


// Booleen d'état
unsigned char estDansLaMain = FALSE; // Si on a cliqué ou pas dans la main
int estPressee = FALSE; // Si la souris est pressée ou non


/****************************************************************************************
 *	Test si on a bien cliqué dans la main, retourne TRUE si oui sinon FALSE		*
 ****************************************************************************************/
int EstDansLaMain(unsigned int x, unsigned int y){
	if ((x >= barre.position_debut.x-TAILLE_MAIN) && (x <= barre.position_debut.x+TAILLE_MAIN) && (y <= barre.position_debut.y + TAILLE_MAIN) && (y >= barre.position_debut.y - TAILLE_MAIN)){
		return TRUE;
	}else{
		return FALSE;
	}
}


/****************************************************************************************
 * 	Si la souris est pressée, on test si on a bien cliqué dans la main		*
 ****************************************************************************************/
void SourisPressee(unsigned int x, unsigned int y){
	if (EstDansLaMain(x, y)){
		estDansLaMain = TRUE;
		estPressee = TRUE;
	}else{
		estDansLaMain = FALSE;
	}
}


/****************************************************************************************
 * 	On relache le bouton de la souris, estPressee = FALSE				*
 ****************************************************************************************/
void SourisRelachee(){
	estPressee = FALSE;
	estDansLaMain = FALSE;
}


/************************************************************************************************
 * 	Test de l'emplacement de la barre							*
 *	Si l'extrémité de la barre est en dehors de la viewport					*
 *	Alors on a touché un bord et on réagit en conséquence					*
 *	On modifie l'angle en calculant la valeur qu'il devrait avoir s'il n'était pas sorti	*
 *	On raccourci la barre d'un epsilon pour être certain que la barre ne restera pas	*
 *		collée à la viewport (problème rencontré)					* 
 ************************************************************************************************/
void TestBord(){
	float epsilon;
	bord = 0;
	if (chocs == NON_ELASTIQUE){
		epsilon = 0.01;
	}else{
		epsilon = 0;
	}
	// Si l'extrémité de la barre dépasse à droite
	if (barre.position_fin.x >= viewport.max.x){
		bord = 1;
		int dx = viewport.max.x - barre.position_debut.x;
		// Si la l'extrémité est 'en dessous' de la main
		if (barre.position_debut.y < barre.position_fin.y){
			// epsilon assure que la barre ne sera plus sur le bord au prochain passage
			barre.theta = M_PI_2 + acos((double)dx / (double)barre.longueur - epsilon);
 		} else {
			barre.theta = M_PI_2 - acos((double)dx / (double)barre.longueur);
		}

	// Sinon si l'extrémité de la barre dépasse à gauche
	} else if (barre.position_fin.x <= viewport.min.x){
		bord = 1;
		int dx = barre.position_debut.x - viewport.min.x;
		// Si l'extrémité est 'en dessous' de la main
		if (barre.position_debut.y < barre.position_fin.y){
			// epsilon assure que la barre ne sera plus sur le bord au prochain passage
			barre.theta = 3*M_PI_2 - acos((double)dx / (double)barre.longueur - epsilon);
		} else {
			barre.theta = 3*M_PI_2 + acos((double)dx / (double)barre.longueur);
		}

	// Sinon si l'extrémité de la barre dépasse en haut
	} else if (barre.position_fin.y <= viewport.max.y){
		bord = 1;
		int dy = barre.position_debut.y - viewport.max.y;
		// Si l'extrémité est 'à droite' de la main
		if (barre.position_debut.x < barre.position_fin.x){
			// epsilon assure que la barre ne sera plus sur le bord au prochain passage
			barre.theta = + acos((double)dy / (double)barre.longueur - epsilon);
		} else {
			// epsilon assure que la barre ne sera plus sur le bord au prochain passage
			barre.theta = - acos((double)dy / (double)barre.longueur - epsilon);
		}

	// Sinon si l'extrémité de la barre dépasse en bas
	} else if (barre.position_fin.y >= viewport.min.y){
		bord = 1;
		int dy = viewport.min.y - barre.position_debut.y;
		// Si l'extrémité est 'à gauche' de la main
		if (barre.position_debut.x < barre.position_fin.x){
			barre.theta = M_PI - acos((double)dy / (double)barre.longueur);
		} else {
			barre.theta = M_PI + acos((double)dy / (double)barre.longueur);		
		}
	}
	#ifdef _DEBUG
	printf("angle=%f=%d\n",barre.theta,Degre(barre.theta));
	#endif

	// Si le calcul à retourné un nan (généralement quand on force la barre dans un coin)
	if (isnan(barre.theta)) {
		barre.theta = M_PI;
	}
}

#endif

