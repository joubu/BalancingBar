#include <stdio.h>
#include <time.h>
#include "cng.h"

#define WSIZE 640
#define HSIZE 480


void dragon(int n, int x, int y, int z, int t)
{
  if (n == 1)
    cng_line(x,y,z,t);
  else {
    int u = (x - y + z + t)/2;
    int v = (x + y - z + t)/2;
    //cng_current_color(255/16*n, 255/16*n, 255/16*n);
    cng_current_color(255,0,0);
    dragon(n-1, x, y, u, v);
    cng_current_color(0,255,0);
    dragon(n-1, z, t, u, v);
  }
}

int main(int argc, char** argv)
{
  // creation de la fenetre graphique de taille WSIZE x HSIZE
  // TRUE indique que l'on souhaite utiliser une palette de 256 couleurs
  cng_init_window("Minigraph Test", WSIZE, HSIZE, FALSE, FALSE);

  cng_current_color(255, 255, 255);
  cng_clear_screen();
  cng_current_color(0, 0, 0);
  dragon(16, WSIZE/2-150, HSIZE/2, WSIZE/2+150, HSIZE/2);

  //cng_line(0, HSIZE/2, WSIZE, HSIZE/2);
  cng_refresh_window();

  /* Main loop */
  cng_main_loop();

  // liberation des ressources
  cng_destroy_window();

  return 0;
}
