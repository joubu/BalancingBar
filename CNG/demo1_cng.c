#include <stdio.h>
#include <time.h>
#include "cng.h"

#define WSIZE 640
#define HSIZE 480

void menu()
{
  /* on efface l'ecran (en noir) */
  cng_current_color_index(255);
  cng_clear_screen();

  /* couleur blanche */
  cng_current_color_index(1);

  /* choix de la police 1 (32 pixels) */
  cng_open_font("font1");
  cng_draw_string("MENU", 20, 10);
  cng_close_font();

  /* choix de la police 2 (16 pixels) */
  cng_open_font("font2");
  cng_draw_string("[a] association d'une touche", 20, 50);
  cng_draw_string("[b] palette du feu", 20, 65);
  cng_draw_string("[c] animation tres simple", 20, 80);
  cng_draw_string("Evenements pris en compte : bouton 0 et deplacement de la souris", 20, 110);
  cng_draw_string("[ESC] Quitter", 20, 140);
  cng_close_font();
}

int very_simple_animation()
{
  int i;
  int cx = WSIZE/2, cy = HSIZE/2; // position initiale du cercle
  int vx = 5, vy = 5; // vecteur direction
  int cra = 50; // rayon du cercle

  // ecran noir
  cng_current_color_index(255);
  cng_clear_screen();
  // limite superieure
  cng_current_color_index(3);
  cng_line(0, 0, WSIZE, 0);
  // rebond d'un cercle
  cng_start_clock();
  for (i = 0 ; i < 500 ; i++) {
    // methode plus rapide (?) : effacement du cercle precedent
    //cng_current_color_index(255);
    //cng_circle(cx,cy,cra);
    //cng_update_region(cx-cra, cy-cra, cx+cra, cy+cra);
    // a droite ou a gauche
    if ((cx+vx+cra > WSIZE) || (cx-abs(vx)-cra < 0)) vx = -vx;
    // en bas ou en haut
    if ((cy+vy+cra > HSIZE) || (cy-abs(vy)-cra < 0)) vy = -vy;
    // mise a jour de la position du cercle
    cx += vx; cy += vy;
    // affichage du cercle
    cng_current_color_index(3);
    cng_circle(cx,cy,cra);
    // effacement de l'ecran
    cng_current_color_index(255);
    cng_clear_screen();
    //cng_update_region(cx-cra, cy-cra, cx+cra, cy+cra);
  }
  printf("Temps : %f\n", cng_get_time());

  cng_delay(500);
  menu();
}

int palette_feu()
{
  unsigned char feu[256][3];
  int i, r, v, b;

  cng_clear_screen();

  // initialisation
  for (i = 0 ; i < 256 ; i++) {
    feu[i][0] = 0; feu[i][1] = 0; feu[i][2] = 0;
  }

  // du blanc au jaune
  for (i = 0 ; i < 64 ; i++) {
    feu[i][0] = 255; feu[i][1] = 255; feu[i][2] = 255-(i*4);
  }

  // du jaune a l'orange
  for (i = 0 ; i < 64 ; i++) {
    feu[i+64][0] = 255; feu[i+64][1] = 255-(i*2); feu[i+64][2] = 0;
  }

  // de l'orange au rouge
  for (i = 0 ; i < 64 ; i++) {
    feu[i+128][0] = 255; feu[i+128][1] = 128-(i*2); feu[i+128][2] = 0;
  }

  // du rouge au noir
  for (i = 0 ; i < 64 ; i++) {
    feu[i+192][0] = 255-(i*4); feu[i+192][1] = 0; feu[i+192][2] = 0;
  }

  cng_palette(feu);

  cng_current_color_index(0);
  cng_clear_screen();
  // affichage de la palette
  for (i = 0 ; i < 256 ; i++) {
    cng_current_color_index(i);
    cng_line(0, i, WSIZE, i);
  }

  cng_delay(2000);
  menu();
}

int touche_a()
{
  int i;

  cng_current_color_index(255);
  cng_clear_screen();
  cng_current_color(255, 255, 0);
  cng_line(0, (int)(HSIZE/10.0), WSIZE, (int)(HSIZE/10.0)); // HLine
  cng_line((int)(WSIZE/10.0), 0, (int)(WSIZE/10.0), HSIZE); // VLine
  //cng_line(0, 0, WSIZE, HSIZE);
  cng_current_color_index(3);
  cng_circle((int)(WSIZE/10.0) + 50, (int)(HSIZE/10.0) + 50, 50); // cercle
  for(i = 0 ; i < 20 ; i++) {
    cng_put_pixel(32*i, 24*i);  // point
  }
  fprintf(stderr, "touche [a] pressee\n");
  cng_delay(2000);
  menu();
}

int bouton_0()
{
  int x, y;
  unsigned char lr, lv, lb;
  char coord[256];

  fprintf(stderr, "Bouton 1 presse\n");
  x = cng_get_mouse_x();
  y = cng_get_mouse_y();
  cng_open_font("font2");
  sprintf(coord, "(%d,%d)", x, y);
  cng_current_color_index(5);
  cng_draw_string(coord, x, y);
  cng_close_font();
  cng_current_color(0, 0, 0);
  //cng_get_pixel(x, y, &lr, &lv, &lb);
}

int mon_depla()
{
  cng_current_color_index(1);
  cng_put_pixel(cng_get_mouse_x(), cng_get_mouse_y());
}

void speed_test()
{
  int j;
  time_t deb, fin;

  deb = time( NULL );

  for (j = 0 ; j < 256 ; j++) {
    cng_current_color_index(0);
    cng_clear_screen();
    cng_current_color_index(j);
    cng_line(0, j, WSIZE, j);
  }

  fin = time( NULL );
  printf("\nTemps %5.2f sec.\n", difftime(fin, deb));
}

int main(int argc, char** argv)
{
  char rep = ' ';
  unsigned char palette0[256][3];
  int i;

  // creation de la fenetre graphique de taille WSIZE x HSIZE
  // TRUE indique que l'on souhaite utiliser une palette de 256 couleurs
  cng_init_window("Minigraph Test", WSIZE, HSIZE, TRUE, TRUE);

  //cng_video_info();

  // initialisation de la table des couleurs par defaut
  for (i = 0 ; i < 256 ; i++) {
    palette0[i][0] = 0; palette0[i][1] = 0; palette0[i][2] = 0;
  }
  palette0[1][0] = 255; palette0[1][1] = 255; palette0[1][2] = 255;
  palette0[2][0] = 255; palette0[2][1] = 0; palette0[2][2] = 0; // rouge
  palette0[3][0] = 0; palette0[3][1] = 255; palette0[3][2] = 0; // vert
  palette0[4][0] = 0; palette0[4][1] = 0; palette0[4][2] = 255; // bleu
  palette0[5][0] = 255; palette0[5][1] = 255; palette0[5][2] = 0; // jaune
  cng_palette(palette0);

  // association des touches a, b et c
  cng_assoc_key(a, touche_a);
  cng_assoc_key(b, palette_feu);
  cng_assoc_key(c, very_simple_animation);
  // association du bouton 0 (i.e bouton gauche)
  cng_assoc_button(0, bouton_0);
  // association deplacement de la souris, bouton 0 appuye
  cng_assoc_motion(mon_depla);

  menu();

  /* Main loop */
  cng_main_loop();

  // liberation des ressources
  cng_destroy_window();

  return 0;
}
