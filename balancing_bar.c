/********************************************************************************************************************************
 *                                                                                                                              *
 *     Auteur : Jonathan Druart                                                                                                 *
 *                                                                                                                              *
 *     balancing_bar.c : Fichier principal                                                                                      *
 *                                                                                                                              *
 *     L'objectif de ce projet est de développer une petite                                                                     *
 *     application de simulation 2D d'une barre homogène en équilibre instable                                                  *
 *                                                                                                                              *
 *    Ce projet utilise :                                                                                                       *
 *        # La bibliothèque cng pour la gestion du temps, des fonts et des couleurs et la création de la fenêtre.               *
 *        # La bliblothèque SDL pour la gestion des évènements et le raffraichissement de l'affichage                           *
 *                                                                                                                              *
 *    Les objectifs atteints :                                                                                                  *
 *        # Saisie des informations sur l'entrée standard                                                                       *
 *        # Tracé des segments de droite à l'aide de l'algorithme de Bresenham                                                  *
 *        # Gestion des évènements (clic, déplacement de la souris, clavier)                                                    *
 *        # Simulation effectuée dans l'espace écran après transformation                                                       *
 *        # Implantation de la loi des mouvements (Calcul de la vitesse angulaire, prise en compte d'un amortissement           *
 *            et de l'accélération instantanée du point de départ de la barre en fonction des mouvements de la souris           *
 *        # Tests de débordement ! La main et la barre ne doivent pas sortir de la viewport et réagir en conséquence.           *
 *            Différents tests sont effectués à différentes étapes.                                                             *
 *        # Gestion des chocs !                                                                                                 *
 *            $ Élastique : Quand l'extrémité de la barre touche un bord, la barre rebondit dessus                              *
 *            $ Inélastique : Quand la barre touche un bord, la barre perd sa vitesse                                           *
 *        # Gestion des scores (lecture/enregistrement dans un fichier du score max)                                            *
 *        # Affichage optionnel des scores, de l'angle ainsi que du nombre de frames par secondes (sur sortie standard)         *
 *        # Pour gagner en performance, on n'efface pas toute la fenetre à chaque cycle mais on réaffiche la barre en noir      *
 *            pour avoir un effet de déplacement                                                                                *
 *        # Les différents fichiers sont commentés pour une facilité de lecture                                                 *
 *                                                                                                                              *
 *    Utilisation :                                                                                                             *
 *        utiliser l'option d'aide : $cat param | ./balancing_bar -h                                                            *
 *                                                                                                                              *
 ********************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "/usr/include/SDL/SDL.h"
#include "cng.h"

#include "balancing_bar.h"

#define FALSE 0
#define TRUE 1

/* Définition de la taille de la "fenetre" */
#define HECRAN 800
#define LECRAN 1000

// Taille de la "main"
#define TAILLE_MAIN 5

/* Constantes */
#define GRAVITE 9.81
#define AMORTISSEMENT 0.05

/* Booleen à vrai si les bords sont élastiques sinon à faux */
#define NON_ELASTIQUE 0
#define ELASTIQUE 1


/* Variables Globales : */

// Instanciation de la viewport, la window et de la barre
Viewport viewport;
Window window;
Barre barre;

// Flag pour éviter d'afficher le score quand il est égal à 0
unsigned char scoreNul = 1;

// Flag qui gère les rebords
unsigned char bord = 0; // bord = 1 si on a touché un bord

// Flag qui gère les chocs, par défaut les chocs ne sont pas élastiques
unsigned char chocs = NON_ELASTIQUE; // gère l'élasticité des chocs

// Variables pour l'implantation de la loi des mouvements 
double acc_ang = 0; // accélération angulaire
double vit_ang = 0; // vitesse angulaire
double vitesseX = 0, vitesseY = 0; // Vitesse en x et y du point A (début de la barre)
double accAx = 0, accAy = 0; // accélération en x et y (dérivé de la vitesse)

// Coefficient modifiable avec les touches 'up' et 'down' pour permettre une adaptation du programme sur différents ordinateurs
float coef = 0.15;

#include "bresenham.h"
#include "calcul.h"
#include "affichage.h"
#include "initParam.h"
#include "test.h"


/****************************************************************************************
 *     Fonction appelée lors de la fermeture du programme                               *
 *     Enregistre le score dans un fichier ("scores.txt")si le record à été battu       *
 ****************************************************************************************/    
void Quitter(Scores* lesScores){
    printf("\n\nLe meilleur score pour la session : %d\n", lesScores->Max);
    // Si le score max est supérieux à celui enregistré
    if (lesScores->Max > lesScores->MaxTot){
        FILE* ficScores;
        ficScores = fopen("scores.txt","w+");
        if (ficScores != NULL){
            fprintf(ficScores, "%d", lesScores->Max);
            fclose(ficScores);
            printf("\nLe score à été enregistré\n");
        }else{
            printf("\nImpossible d'enregistrer le score, vérifiez les droits\n");
        }
        printf("\nRecord battu ! ! !\n");
    }
    printf("\n\nAu revoir ... :)\n\n");
}


/****************************************************************************************
 *     Fonction d'attente des évènements                                                *
 *    Traite les différents évènements jusqu'à l'évènement SDL_QUIT                     *
 ****************************************************************************************/
void MainLoop(SDL_Event* event, Scores* lesScores, char afficherInfos){
    while (event->type != SDL_QUIT) {
        // Démarrage du chrono
        cng_start_clock();

        // On prend l'évènement en tête de file
        SDL_PollEvent(event);

        // On efface la barre de sa position précédente
        EffacerBarre();

        // On test si on a touché un des bord
        TestBord();

        // On calcule la valeur du nouvel angle
        CalculAngle();

        // On affiche la barre
        AfficherBarre();

        // On actualise l'affichage si on a pas affiché les infos (car il y a apparemment un raffraichissement de l'affichage dans cng_draw_string)

        // Gestion des scores
        GestionScore(lesScores);

        // Affichage des "étiquettes"
        if (afficherInfos){
            AfficherInfos(lesScores, 1);
        }else{
            // On actualise l'affichage si on a pas affiché les infos (car il y a apparemment un raffraichissement de l'affichage dans cng_draw_string)
            SDL_Flip(SDL_GetVideoSurface());

            // Affichage des fps
            printf("FPS : %d\n", (int)(1000 / cng_get_time()));
            printf("\033[1A");
        }

        // Si on a touché un bord, on réaffiche la viewport
        if (bord == 1){
            AfficherFenetre();
        }

        // On réagit en fonction de l'évènement
        switch (event->type){
            case SDL_MOUSEBUTTONDOWN :
                if(!estPressee){
                    SourisPressee(event->button.x, event->button.y);
                }
                break;

            case SDL_MOUSEBUTTONUP : 
                SourisRelachee();
                break;

            case SDL_MOUSEMOTION :
                if (estPressee){
                    EffacerBarre();
                    MajAcc(event->button.x, event->button.y);
                }
                break;

            case SDL_QUIT :
                Quitter(lesScores);
                break;

            case SDL_KEYDOWN :
                switch (event->key.keysym.sym){
                    // Appuis sur 'up' augmente le coefficient du delta t
                    case SDLK_UP :
                        if (coef < 5){
                            coef = coef + 0.01;
                            AfficherCoef();
                        }
                        break;

                    // Appuis sur 'down' diminue le coefficient du delta t
                    case SDLK_DOWN :
                        if (coef > 0.01){
                            coef = coef - 0.01;
                            AfficherCoef();
                        }
                        break;

                    // 'a' affiche les informations (scores & angle)
                    case SDLK_a :
                        if (!afficherInfos){
                            AfficherInfos(lesScores, 0);
                            afficherInfos = TRUE;
                        }
                        break;

                    // 'e' rend les chocs élastiques
                    case SDLK_e :
                        chocs = ELASTIQUE;
                        break;

                    // 'i' rend les chocs inélastiques
                    case SDLK_i :
                        chocs = NON_ELASTIQUE;
                        break;

                    // 'right' augmente la longueur de la barre
                    case SDLK_RIGHT : 
                        barre.longueur = barre.longueur + 1;
                        if (barre.longueur > (viewport.min.y - viewport.max.y) / 2){
                            barre.longueur = barre.longueur - 1;
                        }
                        break;

                    // 'left' réduit la longueur de la barre
                    case SDLK_LEFT :
                        if (barre.longueur > 1){
                            barre.longueur = barre.longueur - 1;
                        }
                        break;

                    default :
                        break;
                }
                break;

            default :
                break;
        }

        // On saute les évènements qui se sont produit pendant la séquence d'instructions précédente
        while(SDL_PollEvent(event)){
            if (event->type == SDL_QUIT || event->type == SDL_MOUSEBUTTONUP){
                if(event->type==SDL_QUIT){
                    Quitter(lesScores);
                }
                break ;
            }
        }
    }
}


/****************************************************************************************
 *             Fonction d'affichage de l'aide                                           *
 ****************************************************************************************/
void Aide(){
    printf("\nUtilisation : ./balancing_bar [-option] [difficulté]\n");
    printf("\tOption : \n");
    printf("\t\t[-e]\tchocs élastiques\n\t\t[-i]\tchocs inélastiques\n\t\t[-a]\taffichage des infos (scores, angles, etc...)\n\t\t[-h]\tAide\n");
    printf("\t\tPar défaut les chocs ne sont pas élastiques\n\n");
    printf("\tDifficulté : doit être comprise entre 0 et 20 (avec 20 la difficulté le plus grande)\n");
    printf("\t\tLe niveau de difficulté influe sur l'angle qui rapport des points par rapport à la verticale\n");
    printf("\t\tPar défaut la difficulté est de 10\n");
    printf("\nÉvènements clavier : \n");
    printf("\t* 'up' et 'down' permettent de modifier la valeur du coefficient qui influe sur différentes valeurs\n");
    printf("\t(Permet une meilleure maniabilité de la barre en fonction de l'ordinateur sur lequel est exécuté le programme)\n");
    printf("\t* 'left' et 'right' diminue ou augmente la taille de la barre (à utiliser avec précaution !)\n");
    printf("\t* 'a' Affiche les informations (scores et angle\n");
    printf("\t* 'e' et 'i' rendent les chocs élastiques ou inélastiques\n");
    printf("\t* [ESC] pour quitter\n");
    printf("\nUtilisation à partir du fichier de paramètres : $cat param | ./balancing_bar [-option] [difficulté]\n");
    printf("\nExemple : $cat param | ./balancing_bar -ea 10 pour avoir des chocs élastiques, l'affichage des scores et une difficulté moyenne\n\n");
    printf("ATTENTION : Aucun test n'est effectué sur les valeurs entrées\n\n");
}


/****************************************************************************************
 *             Fonction principale                                                      *
 ****************************************************************************************/
int main(int argc, char* argv[]){
    if (argc <= 3){ // Vérification du nombre d'arguments
        int difficulte = 10;
        char afficherInfos = 0;
        int i = 0;
        // Vérification des arguments passés en paramètres
        switch(argc){
            case 1 : 
                break;

            case 3 : // Calcul de la difficulté
                ; // Incompréhensible, sans le ';' le compilo fait la tronche sur le "int i = 0;"
                char* chaine;
                chaine = (char*)malloc(sizeof(char)*3);
                difficulte = atoi (argv[2]);
                for (i = 0 ; i <= 20 ; i++){
                    if (difficulte == i){
                        difficulte = 20 - i;
                        break;
                    }
                }
                if ((difficulte < 0) || (difficulte > 20)){
                    printf("\nL'argument correspondant à la difficulté doit être compris entre 0 et 20 !\n");
                    Aide();
                    exit(0);
                }

            case 2 : // Vérification de l'option
                switch(argv[1][0]){
                    case '-' : 
                        for (i = 1 ; i < strlen(argv[1]) ; i++){
                            switch(argv[1][i]){
                                case 'e' :
                                    chocs = ELASTIQUE;
                                    break;
                                case 'i' :
                                    chocs = NON_ELASTIQUE;
                                    break;
                                case 'a' : 
                                    afficherInfos = 1;
                                    break;
                                case 'h' : 
                                    printf("\nAide : ");
                                    Aide();
                                    exit(0);
                                    break;
                                default : 
                                    printf("\nL'option n'est pas valide\n");
                                    exit(0);
                                    break;
                            }
                        }
                            break;
                    default : 
                        printf("\nLes paramètres doivent commencés par l'option\n");
                        Aide();
                        exit(0);
                        break;
                 }
                break;

            default : 
                printf("\nPas le bon nombre d'arguments, Veuillez consulter l'aide\n");
                Aide();
                exit(0);
                break;
        }

        // On initialise les données
        InitViewport();
        InitWindow();
        InitBarre();
        printf("\n\n");
        // Création de la fenetre
        int la = LECRAN;
        int ha = HECRAN;
        char *nom = "Simulation d'une barre en équilibre";
        bool lut = FALSE; //look up table
        bool auto_swap = FALSE;
        cng_init_window(nom, la, ha, lut, auto_swap);

        // Initialisation de la couleur courante (blanc)
        cng_current_color(255,255,255);

        // Ouverture du fichier des fonts utilisé pour l'affichage des scores
        cng_open_font("./CNG/font2");

        // Initialisation des scores
        Scores lesScores;
        lesScores.Courant = 0;
        lesScores.Max = 0;
        lesScores.difficulte = difficulte;

        // Lecture du fichier de scores
        FILE* fic;
        if ((fic = fopen("scores.txt", "r")) != NULL){
            fscanf(fic, "%d", &lesScores.MaxTot);
            fclose(fic);
        }else{
            lesScores.MaxTot = 0;
        }


        // Affichage de la viewport
        RetournerFenetre();
        AfficherFenetre();

        // Affichage des "étiquettes"
        if (afficherInfos){
            AfficherInfos(&lesScores, 0);
        }else{
        // On actualise l'affichage si on a pas affiché les infos (car il y a apparemment un raffraichissement de l'affichage dans cng_draw_string)
        // SDL_Flip() swap les buffers écrans et SDL_GetVideoSurface() retourne un poiteur vers la zone d'affichage courante.
            SDL_Flip(SDL_GetVideoSurface());
        }

        // Création d'un évènement de la bibliothèque SDL
        SDL_Event event;

        // Boucle d'évènement perso à l'aide de SDL
        MainLoop(&event, &lesScores, afficherInfos);

        // On libère la ressource utilisée pour les fonts
        cng_close_font();

    }else{
        printf("\nErreur : Pas le bon nombre d'arguments !\n");
        Aide();
        return 1;
    }
    return 0;
}

