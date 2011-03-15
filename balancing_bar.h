#ifndef _PROJETH
#define _PROJETH

typedef struct Point {
	float x,y;
} Point;

typedef struct Pixel {
	unsigned short x,y;
} Pixel;

typedef struct Viewport {
	Pixel min, max;
} Viewport;

typedef struct Window {
	Point min, max;
} Window;

typedef struct Barre {
	Pixel position_debut;
	Pixel position_fin;
	int longueur;
	double theta;
} Barre;

typedef struct Scores {
	int Courant;
	int Max;
	int MaxTot;
	int difficulte;
} Scores;

#endif

