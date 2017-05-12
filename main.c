#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>


//                          <----------|CONSTANTES|---------->

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 780


//                       <----------|TYPES STRUCTURES|---------->

//type permettant de créer la fenêtre d'affichage du jeu
typedef struct game
{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

//type permettant de lancer/arrêter le jeu
typedef struct gameState
{

    int g_bRunning;


}gameState;

//type permettant de situer des points dans la fenêtre
typedef struct coordonnees
{

    double x;
    double y;

}coordonnees;

//type permettant de situer la balle
typedef struct ball
{

    int x;
    int y;

}ball;


//                          <----------|PROTOTYPES|---------->

int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame);
void delay(unsigned int frameLimit);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2);
void drawAll(ball *Balle, int radius, game *myGame, coordonnees *dep,coordonnees *dep2 );
void deplaceBalle(ball *Balle);
void destroy(game *myGame);


//                       <----------|PROGRAMME PRINCIPAL|---------->

int main(int argc, char *argv[])
//But: Gérer l'intégralité du jeu.
//Entrée: Des éléments nécessaires à la SDL.
//Sortie: Le jeu.
{

    game myGame;
    gameState state;

    unsigned int frameLimit = SDL_GetTicks() + 16;

    //initialisation des éléments qui sont des structures
    coordonnees dep;
    coordonnees dep2;
    ball Balle;

    //initialisation des valeurs des structures crées juste avant
    dep.y=(SCREEN_HEIGHT/2)-50;
    dep2.y=(SCREEN_HEIGHT/2)-50;
    Balle.x=SCREEN_WIDTH/2;
    Balle.y=SCREEN_HEIGHT/2;

    int choix=0;

    state.g_bRunning=1;

    //Create texture for drawing in texture or load picture
    myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);

    while(state.g_bRunning)
    {

        if(choix==0)
        {

            printf("Bonjour ! Vous venez de lancer mon magnifique pong !\n");
            printf("Entrez sur 1 si vous voulez jouer.\n");
            printf("Entrez sur 2 si vous voulez quitter le programme.\n");

            scanf("%i",&choix);

        }
        switch (choix)
        {

            case 1:init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame);
                   while(state.g_bRunning)
                   {

                        handleEvents(&state,&dep,&dep2);
                        drawAll(&Balle, 10, &myGame,&dep,&dep2);
                        deplaceBalle(&Balle);

                   }
                   break;

            case 2:break;

            default:break;

        }

        system ("cls");

        // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
        delay(frameLimit);
        frameLimit = SDL_GetTicks() + 16;

    }

    destroy(&myGame);
    SDL_Quit();

    return 0;

}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame)
//But: Initialise la SDL et la fenêtre.
//Entrée: ?
//Sortie: Tout est bien initialisé.
{

    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;

    //initialiser SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {

        //si réussit à créer la fenêtre
        myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
        //si réussit à créer la fenêtre, crée le render
        if(myGame->g_pWindow!=NULL)
        {

            myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);

        }

    }
    else
    {

        return 0;

    }

    return 1;

}


void delay(unsigned int frameLimit)
//But: ?
//Entrée: La limite du nombre de frames.
//Sortie: ?
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {

        return;

    }

    if (frameLimit > ticks + 16)
    {

        SDL_Delay(16);

    }
    else
    {

        SDL_Delay(frameLimit - ticks);

    }
}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2)
//But: Récupérer les contrôles utilisés par le joueur.
//Entrée: L'état du jeu et les coordonnées des deux raquettes.
//Sortie: La raquette du joueur bouge (et celle de l'IA aussi, c'est à faire)
{

    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
            state->g_bRunning=0;break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {

                case SDLK_LEFT: break;
                case SDLK_RIGHT: break;
                case SDLK_UP: if (dep2->y>30){ dep2->y=dep2->y-15;  break; }else{ break; }
                case SDLK_DOWN: if (dep2->y<(SCREEN_HEIGHT-130)){ dep2->y=dep2->y+15;  break; }else{ break; }

            }
            break;

        case SDL_KEYUP:;break;

        default:break;

        }
    }


}


void drawAll(ball *Balle, int radius, game *myGame,coordonnees *dep,coordonnees *dep2)
//But: Dessiner la balle et les deux raquettes.
//Entrée: La balle, son radius, la fenêtre de jeu et les coordonnées des 2 raquettes.
//Sortie: La balle et les raquettes sont dessinées.
{

    int x = radius;
    int y = 0;
    int err = 0;

    //Définition du rectangle 1 a dessiner
    SDL_Rect rectangle;
    rectangle.x=30;//debut x
    rectangle.y=dep->y;//debut y
    rectangle.w=10; //Largeur
    rectangle.h=100; //Hauteur

    //Définition du rectangle 2 a dessiner
    SDL_Rect rectangle2;
    rectangle2.x=SCREEN_WIDTH-30;//debut x
    rectangle2.y=dep2->y;//debut y
    rectangle2.w=10; //Largeur
    rectangle2.h=100; //Hauteur

    //Dessine dans la texture
    SDL_SetRenderDrawColor(myGame->g_pRenderer,255,255,255,255);
    SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

    //On dessine le cercle point par point
    while (x >= y)
    {
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x + x, Balle->y + y);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x + y, Balle->y + x);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x - y, Balle->y + x);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x - x, Balle->y + y);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x - x, Balle->y - y);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x - y, Balle->y - x);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x + y, Balle->y - x);
        SDL_RenderDrawPoint(myGame->g_pRenderer, Balle->x + x, Balle->y - y);

        y += 1;
        if (err <= 0)
        {
            err += 2*y + 1;
        }
        if (err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }

    //Dessine les rectangles pleins
    SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
    SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);

    SDL_SetRenderTarget(myGame->g_pRenderer, NULL); // Dorénavent, on modifie à nouveau le renderer

    SDL_Rect position; //position de la texture dans le screen
    position.x = 0;
    position.y = 0;
    SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
    SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);

    SDL_RenderPresent(myGame->g_pRenderer);

    SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);

    //On détruit la texture
    SDL_DestroyTexture(myGame->g_ptexture);

    //On vide le render
    SDL_RenderClear(myGame->g_pRenderer);

}


void deplaceBalle(ball *Balle)
//But: Gérer les déplacements de la balle (et dans l'idéal les collisions).
//Entrée: La balle.
//Sortie: La balle bouge (et rebondit).
{

        //déplacement de base lors de l'engagement
        Balle->x=Balle->x+5;

}


void destroy(game *myGame)
//But: Détruit tous les éléments du jeu.
//Entrée: La fenêtre de jeu.
//Sortie: La fenêtre de jeu disparait.
{

    //Détruit le render
    if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

    //Détruit la texture
    if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Détruit la surface
    if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Détruit la fenêtre
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}



//Il faut encore programmer les collisions, les déplacements de la balle, l'IA, les scores, l'affichagedes scores et la ligne de milieu de terrain.
//Pas mal de temps de perdu au démarrage en essayant de commencer de zéro (pas de template SDL2, grosse erreur).
//Les recherches pour dessiner le cercle étaient fructueuses, mais surtout niveau bitmap, encore pas mal de temps de perdu.



