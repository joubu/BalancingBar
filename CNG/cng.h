/**************************************************************
 *
 *    CNG : Campus Naked Graphic
 *
 *    Copyright (c) 2002-2007 Christian Nguyen
 *
 *    This program is free software; you can redistribute it 
 *    and/or modify it under the terms of the GNU General 
 *    Public License as published by the Free Software 
 *    Foundation (version 2 of the License).
 *
 *    This program is distributed in the hope that it will 
 *    be useful, but WITHOUT ANY WARRANTY; without even the 
 *    implied warranty of MERCHANTABILITY or FITNESS FOR A 
 *    PARTICULAR PURPOSE.  See the GNU General Public License 
 *    for more details.
 *
 *    For a copy of the GNU General Public License, write to the 
 *    Free Software Foundation, Inc., 675 Mass Ave, Cambridge, 
 *    MA 02139, USA.
 **************************************************************/

/**************************************************************
 *    Bibliotheque minimaliste pour la realisation des TP
 *    d'infographie de 2ieme cycle.
 **************************************************************/

#ifndef __CNG_H__
#define __CNG_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {FALSE, TRUE} bool;
typedef enum {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z} touche_alpha;

// Gestion de la fenetre graphique
// ========================================================

// constructeur : nom de la fenetre, dimension, palette de couleurs (O/N),
// activation ou non de la permutation automatique des mémoires images
// dans ce cas, l'utilisation de cng_refresh_window() est obligatoire
void cng_init_window(char *nom, int la, int ha, bool lut, bool auto_swap);

// gestion manuelle du double buffer image
// ATTENTION : cette fonction n'est utile que pour les fonctions
// de dessin suivante cng_put_pixel et cng_line (et cng_line_to)
void cng_refresh_window();

// affiche sur stderr des informations sur l'environnement graphique
void cng_video_info();

// boucle d'attente des evenements ; le parametre sert de "idle function"
void cng_main_loop(int (*g)());

// destructeur
void cng_destroy_window();

// efface l'ecran de travail
void cng_clear_screen();

// positionne la couleur dans laquelle on va dessiner
// code : r rouge, v vert et b bleu dans [0, 255]
void cng_current_color(unsigned char r, unsigned char v, unsigned char b);

// initialisation effective de la palette des couleurs (256 couleurs maxi)
void cng_palette(unsigned char rvb[256][3]);

// positionne la couleur dans laquelle on va dessiner
// icolor : index dans la palette de la couleur choisie
void cng_current_color_index(int icolor);

// Fonctions graphiques de base
// ========================================================

// dessine un point de la couleur courante en (x, y)
void cng_put_pixel(int x, int y);

// dessine une ligne de la couleur courante de (x1,y1) a (x2,y2)
void cng_line(int x1, int y1, int x2, int y2);

// dessine une ligne de la couleur courante de la position courante a (x,y)
void cng_line_to(int x, int y);

// initialise la position courante
void cng_move_to(int x, int y);

// dessine le contour d'un cercle
void cng_circle(int posx, int posy, int r);

// Fonctions de gestion du texte
// ========================================================

// choix du fichier contenant la police de caracteres
void cng_open_font(char* fontfile);

// affichage du texte a la position (x,y)
// attention : une police doit etre chargee prealablement
//             avec la fonction cng_open_font()
void cng_draw_string(const char* texte, int x, int y);

// largeur en pixels de la chaine de caractere a afficher
int cng_string_width(char* string);

// liberation de la ressource
void cng_close_font();

// Fonctions de gestion des E/S
// ========================================================

// association d'une touche avec une fonction de la forme "int f()"
void cng_assoc_key(touche_alpha k, int (*g)());

// association d'un bouton (0, 1, 2) avec une fonction de la forme "int f()"
void cng_assoc_button(int bid, int (*g)());

// association du deplacement souris avec une fonction de la forme "int f()"
// ce deplacement se fait bouton 0 presse pour des raisons d'efficacite
void cng_assoc_motion(int (*g)());

// retourne la position en X (resp. Y) du curseur de la souris
int cng_get_mouse_x();
int cng_get_mouse_y();

// Fonctions de traitement d'images
// ========================================================

// recupere la couleur du pixel
void cng_get_pixel(int x, int y, unsigned char* r, unsigned char* v, unsigned char* b);

// recuperation du nombre de plan de l'ecran de travail
int cng_get_bpp();

// Fonctions diverses
// ========================================================

// demarrage du chrono (mesure de temps)
void cng_start_clock();

// recuperation du temps en millisecondes (ms)
double cng_get_time();

// delais d'attente en millisecondes
void cng_delay(int ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CNG_H__ */
