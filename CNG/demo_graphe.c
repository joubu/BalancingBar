#include "graphe.h"  	

	      
main()			
{	
  double x;
  fenetre(0.,0.);
  cng_current_color(0,0,0);

  for (x=0.;x<=2*pi;x=x+0.002)
    {
      trace (x,sin(x));
    }

  affiche();
  printf("OK c est fait");
  quitte();
}

