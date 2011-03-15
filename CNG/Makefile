##############################################################################
# VARIABLES D'ENVIRONNEMENT

INCDIR=-I. -I/usr/include -I/usr/include/SDL -I/usr/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/lib/glib/include -I/usr/X11R6/include

LIBDIR=-L. -L/usr/lib -L/usr/lib -L/usr/X11R6/lib -L$(HOME)/lib

#MYLINK=-lgtk -lgdk -lgmodule -lglib -ldl -lXi -lXext -lX11 -lm -lSDL -lpthread
MYLINK=-lSDL -lpthread -lfreetype

MYDEF=-D_REENTRANT -D_DEBUG

##############################################################################
# BIBLIOTHEQUE CNG (Campus Naked Graphic)

libcng.a: cng.o
	ar cr libcng.a cng.o
	ranlib libcng.a
	mv libcng.a ~/lib
	cp cng.h ~/include

cng.o: cng.c
	gcc $(INCDIR) $(MYDEF) -c cng.c

# ET SES FICHIERS DE DEMOS

demo1_cng: demo1_cng.o
	gcc -O2 -o demo1_cng demo1_cng.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

demo1_cng.o: demo1_cng.c
	gcc $(INCDIR) $(MYDEF) -c demo1_cng.c

demo2_cng: demo2_cng.o
	gcc -O2 -o demo2_cng demo2_cng.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

demo2_cng.o: demo2_cng.c
	gcc $(INCDIR) $(MYDEF) -c demo2_cng.c

##############################################################################
# SIMULATION D'UNE FLAMME

flammes: flammes.o
	gcc -O2 -o flammes flammes.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

flammes.o: flammes.c
	gcc $(INCDIR) $(MYDEF) -c flammes.c

palette_feu: palette_feu.o
	gcc -O2 -o palette_feu palette_feu.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

palette_feu.o: palette_feu.c
	gcc $(INCDIR) $(MYDEF) -c palette_feu.c

##############################################################################
# ???

exemple01: exemple01.o
	gcc -O2 -o exemple01 exemple01.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib $(MYLINK)

exemple01.o: exemple01.c
	gcc $(INCDIR) $(MYDEF) -c exemple01.c

##############################################################################
# BRESENHAM

bresenham: bresenham.o
	gcc -O2 -o bresenham bresenham.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

bresenham.o: bresenham.c
	gcc $(INCDIR) $(MYDEF) -c bresenham.c

##############################################################################
# ENTRELAC CELTE

celte: celte.o
	gcc -O2 -o celte celte.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

celte.o: celte.c
	gcc $(INCDIR) $(MYDEF) -c celte.c

##############################################################################
# BEZIER

bezier: bezier.o
	gcc -O2 -o bezier bezier.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

bezier.o: bezier.c
	gcc -D_DEBUG $(INCDIR) $(MYDEF) -c bezier.c

##############################################################################
# SEPPECHER .PIG

demo_graphe: demo_graphe.o graphe.o
	gcc -O2 -o demo_graphe demo_graphe.o graphe.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

graphe.o: graphe.c
	gcc -D_DEBUG $(INCDIR) $(MYDEF) -c graphe.c

demo_graphe.o: demo_graphe.c
	gcc -D_DEBUG $(INCDIR) $(MYDEF) -c demo_graphe.c

##############################################################################
# DRAGON

dragon: dragon.o
	gcc -O2 -o dragon dragon.o $(LIBDIR) -rdynamic -Wl,-rpath,/usr/lib -lcng $(MYLINK)

dragon.o: dragon.c
	gcc -D_DEBUG $(INCDIR) $(MYDEF) -c dragon.c

##############################################################################
# NETTOYAGE

clean:
	rm -f *~ *.o core
