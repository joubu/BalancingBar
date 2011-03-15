/********************************************************************************************************
 * 	Auteur : Jonathan Druart									*
 *	bresenham.h											*
 *	Regroupe les fonctions utiles à l'utilisation du tracé de bresenham dans tous les octants	*
 *	Le fonction Bresenham calcul l'octant dans lequel se trouve le segment à tracer			*
 *	En fontion de cet octant, on appelle la fonction BresenhamCalcul qui calcule			*
 *		de manière incrémentale (avec des entiers) les pixels à afficher et appelle 		*
 *		la fonction d'affichage correspondante							*
 ********************************************************************************************************/



#ifndef _BRESENHAM
#define _BRESENHAM

/********************************************************************************
 *	Détermine l'octant du segment de droite qui va du pixel a au pixel b	*
 ********************************************************************************/
unsigned short DetermineOctant (int aX, int aY, int bX, int bY){
	int dx, dy;
	unsigned short o;
	dx = bX - aX;
	dy = bY - aY;

	if(aX <= bX){
		if(aY <= bY){
			if(dx <= dy)
				o = 2;
			else
				o = 1;
		}else{
			if(dx <= -dy)
				o = 7;
			else
				o = 8;
		}
	}else{
		if(aY <= bY){
			if(-dx <= dy)
				o = 3;
			else
				o = 4;
		}else{
			if(dx <= dy)
				o = 5;
         		else
				o = 6;
		}
	}
	#ifdef _DEBUG
	printf("octant : %d\n",o);
	#endif
	return o;
}

/********************************************************************************
 *	fonctions d'affichage en fonction de l'octant				*
 ********************************************************************************/
int Affiche1(int x, int y){
	cng_put_pixel(x, y);
}
int Affiche2(int x, int y){
	cng_put_pixel(y, x);
}
int Affiche3(int x, int y){
	cng_put_pixel(-y, x);
}
int Affiche4(int x, int y){
	cng_put_pixel(-x, y);
}
int Affiche5(int x, int y){
	cng_put_pixel(-x, -y);
}
int Affiche6(int x, int y){
	cng_put_pixel(-y, -x);
}
int Affiche7(int x, int y){
	cng_put_pixel(y, -x);
}
int Affiche8(int x, int y){
	cng_put_pixel(x, -y);
}


/********************************************************************************
 * 	Fonction principale de Bresenham					*
 ********************************************************************************/
void BresenhamCalcul(int aX, int aY, int bX, int bY, int (*Affiche)()){
	int dx = (bX - aX) * 2;
	int dy = (bY - aY) * 2;

	//e = erreur
	int e =  bX - aX;

	while(aX < bX){
		//Appel de la fonction d'affichage
		Affiche(aX, aY);
		aX++;
		e -= dy;
		if(e <= 0){
			aY++;
			e += dx;
		}
	}
}


/********************************************************************************
 * 	En fonction de l'octant dans lequel se situe le pixel,			*
 * 	on appelle la fonction BresenhamCalcul avec la bonne fonction d'affichage *
 ********************************************************************************/
void Bresenham(int aX, int aY, int bX, int bY){
	switch(DetermineOctant(aX, aY, bX, bY)){
		case 1:
			BresenhamCalcul( aX, aY, bX, bY, Affiche1);
			break;
		case 2:
			BresenhamCalcul( aY, aX, bY, bX, Affiche2);
			break;
		case 3:
			BresenhamCalcul( aY,-aX, bY,-bX, Affiche3);
			break;
		case 4:
			BresenhamCalcul(-aX, aY,-bX, bY, Affiche4);
			break;
		case 5:
			BresenhamCalcul(-aX,-aY,-bX,-bY, Affiche5);
			break;
		case 6:
			BresenhamCalcul(-aY,-aX,-bY,-bX, Affiche6);
			break;
		case 7:
			BresenhamCalcul(-aY, aX,-bY, bX, Affiche7);
			break;
		case 8:
			BresenhamCalcul( aX,-aY, bX,-bY, Affiche8);
			break;
	}
}



#endif

