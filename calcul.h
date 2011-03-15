/********************************************************************************
 * 	Auteur : Jonathan Druart						*
 * 	calcul.h								*
 * 		$ Nouvelles coordonnées						*
 * 			# Calcul de coordonnées dans l'espace écran à partir	*
 * 				de coordonnées dans l'espace euclidien		*
 *		$ Rotation							*
 *			# Calcul des nouvelles coordonnées de la fin 		*
 *				de la barre grâce au nouvel angle (theta)	*
 *			# Tests de débordement					*
 *		$ Retourne la fenetre						*
 *			# On retourne la fenetre pour l'avoir dans l'écran	*
 *		$ Calcul de l'angle						*
 *			# Calcul du nouvel angle grâce à l'accélération 	*
 *				angulaire puis de la vitesse angulaire		*
 *			# Réaction en fonction des chocs non élastique		*
 *		$ Mise à jour de l'accélération					*
 *			# Tests de débordement					*
 *			# Calcul de la nouvelle vitesse instantannée		*
 *			# En dérivant on trouve l'accélération gamma		*
 *			# Mise à jour des nouvelles coordonnées de la 		*
 *				position de la barre				*
 *		$ Calcul de l'angle en degré					*
 *			# retourne la valeur en degré d'un angle en radian	*
 *		$ Gestion des scores						*
 *			# Gère l'état des scores				*
 ********************************************************************************/

#ifndef _CALCUL
#define _CALCUL


/********************************************************************************
 *	Calcul des coordonnées dans l'espace écran d'un point se situant 	*
 *	dans l'espace euclidien	(i.e. d'un point en pixel)			*
 ********************************************************************************/
Pixel NouvCoord(Point p){
	Pixel NewP;
	NewP.x = (unsigned short) (p.x * (viewport.max.x-viewport.min.x)/(window.max.x-window.min.x) - window.min.x * (viewport.max.x-viewport.min.x)/(window.max.x-window.min.x) + viewport.min.x);
	NewP.y = (unsigned short) (- p.y * (viewport.max.y - viewport.min.y)/(window.max.y-window.min.y) + HECRAN + window.min.y * (viewport.max.y - viewport.min.y)/(window.max.y-window.min.y) - viewport.min.y);

	#ifdef _DEBUG
	printf("x=%hu\n",NewP.x);
	printf("y=%hu\n",NewP.y);
	#endif

	return NewP;
}


/********************************************************************************
 * 	Calcul des nouvelles coordonnées de la position de l'extrémité 		*
 * 	de la barre par rapport à l'angle courant et à la position de la main.	*
 *	Si on dépasse de la viewport (à cause des erreurs de calcul 		*
 *	qui subsistent), on décale légèrement l'extrémité de la barre		*
 ********************************************************************************/
Pixel Rotation(){

	// Calcul de la position de l'extrémité de la barre
	barre.position_fin.x = barre.position_debut.x + barre.longueur * cos(barre.theta - M_PI_2);
	barre.position_fin.y = barre.position_debut.y + barre.longueur * sin(barre.theta - M_PI_2);

	// Test pour éviter que l'extrémité de la barre ne sorte de la viewport
	if (barre.position_fin.x >= viewport.max.x){
		barre.position_fin.x = viewport.max.x;
	} else if (barre.position_fin.x <= viewport.min.x){
		barre.position_fin.x = viewport.min.x;
	}
	if (barre.position_fin.y <= viewport.max.y){
		barre.position_fin.y = viewport.max.y;
	} else if (barre.position_fin.y >= viewport.min.y){
		barre.position_fin.y = viewport.min.y;
	}
}

/********************************************************************************
 * 	On retourne la fenetre pour l'avoir dans l'écran			*
 ********************************************************************************/
void RetournerFenetre(){
	viewport.min.y = -viewport.min.y + HECRAN;
	viewport.max.y = -viewport.max.y + HECRAN;
}


/********************************************************************************
 * 	On calcul la nouvelle valeur de l'angle.				*
 * 	-> Calcul de l'accélération angulaire					*
 * 	-> On en déduit la vitesse angulaire					*
 * 	-> On trouve l'angle							*
 ********************************************************************************/
void CalculAngle(){
	// Grâce à la loi des mouvements et à l'accélération de A, on calcule l'accélération angulaire
	acc_ang = (3. / (2 * barre.longueur)) * (GRAVITE * sin(barre.theta) + accAx * cos(barre.theta) + accAy * sin(barre.theta));
	// Si on a touché un bord précédemment
	if (bord == 1){
		// On inverse la vitesse angulaire pour que le pendule 'rebondisse'
		vit_ang = -vit_ang;
		// Si les chocs ne sont pas élastiques, on met à 0 la vitesse angulaire
		if (chocs == NON_ELASTIQUE){
			vit_ang = 0;
		}
	}

	// On affecte l'accélération angulaire d'un coefficient d'amortissement
	acc_ang = acc_ang - AMORTISSEMENT * vit_ang;

	// On dérive l'accélération angulaire, et on trouve la vitesse angulaire
	vit_ang = vit_ang + coef * acc_ang ;

	// On dérive la vitesse angulaire, et on trouve la valeur du nouvel angle
	barre.theta = barre.theta + coef * vit_ang;

	// On calcule la position de l'extrémité de la barre grâce au nouvel angle
	Rotation();

	// On met l'accélération et la vitesse instantannée à 0, pour le prochain déplacement
	accAx = 0;
	accAy = 0;
	vitesseX = 0;
	vitesseY = 0;

	#ifdef _DEBUG
	printf("theta=%f=%d\tacc_ang=%f\tvit_ang=%f\n",barre.theta,Degre(barre.theta),acc_ang,vit_ang);
	printf("fin=%d,%d\n",barre.position_fin.x, barre.position_fin.y);
	#endif
}


/********************************************************************************
 * 	Mise à jour de l'accélération instantannée au point A			*
 ********************************************************************************/
void MajAcc(unsigned int x, unsigned int y){
	// Calcule de débordement, la main ne doit jamais sortir de la viewport
	if (x < viewport.min.x){
		x = viewport.min.x;
	} else if (x > viewport.max.x){
		x = viewport.max.x;
	} 
	if (y > viewport.min.y){
		y = viewport.min.y;
	} else if (y < viewport.max.y){
		y = viewport.max.y;
	}

	// Calcule de la vitesse en x et y affecté d'un coefficient
	vitesseX = (float)(((float)barre.position_debut.x - (float)x)) / coef;
	vitesseY = (float)(((float)barre.position_debut.y - (float)y)) / coef;

	// Calcule de l'accélération en A grâce à la vitesse
	accAx = vitesseX / coef / 15;
	accAy = vitesseY / coef / 15;

	// On initialise la nouvelle position de la main grâce en fonction de la position de la souris
	barre.position_debut.x = x;
	barre.position_debut.y = y;

	// On calcule la position de l'extrémité de la barre avec la nouvelle position de la main
	Rotation();

}


/********************************************************************************
 * 	Retourne la valeur d'un angle en degré donné en radian			*
 ********************************************************************************/
int Degre(double angle){
	return (abs((int)(angle*180/M_PI))%360);
}


/********************************************************************************
 * 	Gère l'état des scores en fonction de l'angle de la barre		*
 ********************************************************************************/
void GestionScore(Scores* lesScores){
	int degre = Degre(barre.theta);
	// Si la barre est décalée de +- lesScores->angle ° par rapport à la verticale
	if ((degre >= 360 - lesScores->difficulte) || (degre <= lesScores->difficulte)){ 
		// Sans l'aide des bords
		if (bord != 1){
			//On incrémente le compteur du score courant
			lesScores->Courant ++;
		}else{
		// Si on a touché un bord, on enlève 10 au score
			lesScores->Courant = lesScores->Courant - 10;
		}
	}else{ // Si la barre n'est pas à "l'équilibre" le score est nul
		lesScores->Courant = 0;
	}
	// On vérifie que le score soit toujours positif
	if (lesScores->Courant < 0){
		lesScores->Courant = 0;
	}
	// Si on a battu le score de la session courante, on met à jour le score max
	if(lesScores->Courant > lesScores->Max){
		lesScores->Max = lesScores->Courant;
	}
}


#endif


