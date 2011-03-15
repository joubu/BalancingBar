#include "graphe.h"

#define taille 650

double zoom=1.0;
double zoomx=1.0;
double zoomy=1.0;
double centrex=0.0;
double centrey=0.0;
double positionx=0.0;
double positiony=0.0;
// double 3Dposx=0.0;
// double 3Dposy=0.0;
// double 3Dposz=0.0;




/***************************************************************/

void fenetre(double x, double y)
{
 cng_init_window("GRAPHIQUE", taille, taille,0,0); 
 cng_current_color(255,255,255);
 cng_clear_screen();
 cng_current_color(10,200,10);
  //  cng_line(2,2,taille-2,2);
  //  cng_line(taille-2,2,taille-2,taille-2);
  //   cng_line(taille-2,taille-2,2,taille-2);
  //   cng_line(2,taille-2,2,2);
  // cng_current_color_index(5);
  //   cng_line(taille/2,2,taille/2,taille-2);
  //   cng_line(1,taille/2,taille-2,taille/2);
  // cng_current_color_index(14);
  centrex=x;
  centrey=y;
}

void affiche(void)
{
  //cng_refresh_window();
  //cng_current_color(255,255,255);   
  //cng_clear_screen();
  cng_current_color(10,200,10);
  cng_line(2,2,taille-2,2);
  cng_line(taille-2,2,taille-2,taille-2);
  cng_line(taille-2,taille-2,2,taille-2);
  cng_line(2,taille-2,2,2);
  cng_current_color(10,10,200);
  cng_line(taille/2,2,taille/2,taille-2);
  cng_line(1,taille/2,taille-2,taille/2);
  cng_current_color(200,10,10);
  cng_refresh_window();
}

void afficheplus(void)
{
   cng_refresh_window();
   cng_current_color(10,200,10);
   cng_line(2,2,taille-2,2);
   cng_line(taille-2,2,taille-2,taille-2);
   cng_line(taille-2,taille-2,2,taille-2);
   cng_line(2,taille-2,2,2);
   cng_current_color(10,10,200);
   cng_line(taille/2,2,taille/2,taille-2);
   cng_line(1,taille/2,taille-2,taille/2);
   cng_current_color(200,10,10);
}

void quitte(void)
{
  //cng_delay(100000);
  //cng_clear_screen();
  cng_main_loop();
  cng_destroy_window();
}

void bouge(double x, double y)
{
  positionx=x;
  positiony=y;
}

void trace(double x, double y)
{
  double px;
  double py;
  px=(taille-4)/2*zoom*(positionx-centrex)+taille/2;
  py=-(taille-4)/2*zoom*(positiony-centrey)+taille/2;
  positionx=x;
  positiony=y;
  x=(taille-4)/2*zoom*(x-centrex)+taille/2;
  y=-(taille-4)/2*zoom*(y-centrey)+taille/2;
  cng_line(px,py,x,y);
}
