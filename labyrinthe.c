# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include <time.h>

const int nombreCase=41;
const int tailleGrosBloc=25;
const int taillePetitBloc=10;
int fin=0;

enum {Vide, MurHorizontal, MurVertical, Coin, Fin};

int tempsAnimationBalle;

typedef struct
{
    int ID;
    int typeCase;
    SDL_Surface *texture;
}CASE;

typedef struct
{
    SDL_Rect _position;
    SDL_Surface *_texture;
}BALLE;

BALLE AjoutBalle()
{
    BALLE balle;
    balle._texture=IMG_Load("BalleFer1.png");
    balle._position.x=12;
    balle._position.y=12;
    return balle;
}

BALLE verificationCollisionBalle(BALLE balle, CASE terrain[nombreCase][nombreCase], int UP, int DOWN, int LEFT, int RIGHT)
{
    int i,j;
    if( UP ) balle._position.y--;
    if( DOWN ) balle._position.y++;
    if( LEFT ) balle._position.x--;
    if( RIGHT ) balle._position.x++;

    SDL_Rect positionCurseur;
    positionCurseur.x=0;
    positionCurseur.y=0;

    for(i=0;i<nombreCase;i++)
        {
            for(j=0;j<nombreCase;j++)
            {
                if(terrain[i][j].typeCase!=Vide&&terrain[i][j].typeCase!=Fin)
                {
                    if(balle._position.x<positionCurseur.x+terrain[i][j].texture->w
                       &&balle._position.x+balle._texture->w>positionCurseur.x
                       &&balle._position.y<positionCurseur.y+terrain[i][j].texture->h
                       &&balle._position.y+balle._texture->h>positionCurseur.y)
                       {
                            if( UP ) balle._position.y++;
                            if( DOWN ) balle._position.y--;

                        if(balle._position.x<positionCurseur.x+terrain[i][j].texture->w
                           &&balle._position.x+balle._texture->w>positionCurseur.x
                           &&balle._position.y<positionCurseur.y+terrain[i][j].texture->h
                           &&balle._position.y+balle._texture->h>positionCurseur.y)
                       {
                            if( UP ) balle._position.y--;
                            if( DOWN ) balle._position.y++;
                            if( LEFT ) balle._position.x++;
                            if( RIGHT ) balle._position.x--;

                            if(balle._position.x<positionCurseur.x+terrain[i][j].texture->w
                               &&balle._position.x+balle._texture->w>positionCurseur.x
                               &&balle._position.y<positionCurseur.y+terrain[i][j].texture->h
                               &&balle._position.y+balle._texture->h>positionCurseur.y)
                           {
                               if( UP ) balle._position.y++;
                                if( DOWN ) balle._position.y--;
                           }
                       }
                    }
                }
                if(j%2==0) positionCurseur.y+=taillePetitBloc;
                else positionCurseur.y+=tailleGrosBloc;
            }
            if(i%2==0) positionCurseur.x+=taillePetitBloc;
            else positionCurseur.x+=tailleGrosBloc;
            positionCurseur.y=0;

            if(balle._position.x>675
                &&balle._position.y>675)
               {
                    fin=1;
               }
        }
        return balle;
}

void labyrinthe(SDL_Surface *ecran)
{
    int i,j;
    fin=0;
    int masque=0;

    SDL_Rect positionCurseur;
    SDL_Rect positionMasque;
    SDL_Surface *textureMasque=IMG_Load("masque.png");


    BALLE balle = AjoutBalle();

    CASE terrain[nombreCase][nombreCase];

    // Initialisation des cases
    for(i=0;i<nombreCase;i++)
    {
        for(j=0;j<nombreCase;j++)
        {
            terrain[i][j].ID=i+j*nombreCase;
            if(((i-1)%2==0)&&((j-1)%2==0)) terrain[i][j].typeCase=Vide;
            else if((i%2==0)&&(j%2==0)) terrain[i][j].typeCase=Coin;
            else if (j%2==0)terrain[i][j].typeCase=MurHorizontal;
            else if (i%2==0)terrain[i][j].typeCase=MurVertical;
        }
    }

    srand(time(NULL));

    // Generation du labyrinthe
    int nombreCaseRelie=0;
    int valeurAleatoire1;
    int valeurAleatoire2;
    int direction;
    int Largeur2;
    int Hauteur2;

    while (nombreCaseRelie!=(nombreCase/2)*(nombreCase/2)-1)
    {
        valeurAleatoire1=(rand()%(nombreCase/2)+1)*2-1;
        Largeur2=valeurAleatoire1;
        valeurAleatoire2=(rand()%(nombreCase/2)+1)*2-1;
        Hauteur2=valeurAleatoire2;

        direction=rand()%4;
        switch(direction)
        {
            case 0:
                Hauteur2=Hauteur2-2;
                break;
            case 1:
                Largeur2=Largeur2-2;
                break;
            case 2:
                Hauteur2=Hauteur2+2;
                break;
            case 3:
                Largeur2=Largeur2+2;
                break;
            default:
                break;
        }
        if ((Hauteur2>=1)&&(Hauteur2<=nombreCase-1)
            &&(Largeur2>=1)&&(Largeur2<=nombreCase-1))
        {
        if(terrain[Largeur2][Hauteur2].ID!=terrain[valeurAleatoire1][valeurAleatoire2].ID)
            {
                int IDTemp=terrain[Largeur2][Hauteur2].ID;
                terrain[Largeur2][Hauteur2].ID=terrain[valeurAleatoire1][valeurAleatoire2].ID;
                terrain[(valeurAleatoire1+Largeur2)/2][(valeurAleatoire2+Hauteur2)/2].ID
                = terrain[valeurAleatoire1][valeurAleatoire2].ID;
                terrain[(valeurAleatoire1+Largeur2)/2][(valeurAleatoire2+Hauteur2)/2].typeCase
                =Vide;
                for(i=0;i<nombreCase;i++)
                {
                    for(j=0;j<nombreCase;j++)
                    {
                        if(terrain[i][j].ID==IDTemp) terrain[i][j].ID=terrain[valeurAleatoire1][valeurAleatoire2].ID;
                    }
                }
                nombreCaseRelie++;
            }
        }
    }

    for(i=0;i<nombreCase;i++)
    {
        for(j=0;j<nombreCase;j++)
        {
            if(terrain[i][j].typeCase==MurVertical) terrain[i][j].texture=IMG_Load("MurVertical.png");
            if(terrain[i][j].typeCase==MurHorizontal) terrain[i][j].texture=IMG_Load("MurHorizontal.png");
            if(terrain[i][j].typeCase==Coin)
            {
                if (terrain[i][j-1].typeCase==Vide)
                {
                    if(terrain[i-1][j].typeCase==Vide&&terrain[i+1][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Coin4.png");
                    else if (terrain[i-1][j].typeCase==Vide&&terrain[i][j+1].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin2.png");
                    else if(terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin5.png");
                    else if(terrain[i+1][j].typeCase==Vide&&terrain[i][j+1].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin3.png");
                    else if(terrain[i+1][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Coin6.png");
                    else terrain[i][j].texture=IMG_Load("Coin.png");
                }
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i+1][j].typeCase==Vide&&terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin1.png");
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i+1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin7.png");
                else if(terrain[i][j+1].typeCase==Vide&&terrain[i-1][j].typeCase==Vide)terrain[i][j].texture=IMG_Load("Coin8.png");
                else terrain[i][j].texture=IMG_Load("Coin.png");
            }
            if(terrain[i][j].typeCase==Vide) terrain[i][j].texture=IMG_Load("Vide.png");

        }
    }
    terrain[39][39].typeCase=Fin;
    terrain[39][39].texture=IMG_Load("Fin.png");

    SDL_Event event;
    Uint8 *keystates = SDL_GetKeyState( NULL );

    int continuer=1;
    while(continuer)
    {
        SDL_Delay(1);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        continuer=0;
                        break;
                        default:
                        break;
                    }
                break;
            }
        }
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,182,220,228));

        positionCurseur.x=0;
        positionCurseur.y=0;

        for(i=0;i<nombreCase;i++)
        {
            for(j=0;j<nombreCase;j++)
            {
                SDL_BlitSurface(terrain[i][j].texture, NULL, ecran, &positionCurseur);

                if(j%2==0)positionCurseur.y+=taillePetitBloc;
                else positionCurseur.y+=tailleGrosBloc;
            }
            if(i%2==0) positionCurseur.x+=taillePetitBloc;
            else positionCurseur.x+=tailleGrosBloc;
            positionCurseur.y=0;
        }

        balle=verificationCollisionBalle(balle, terrain, keystates[SDLK_UP], keystates[SDLK_DOWN],keystates[SDLK_LEFT],keystates[SDLK_RIGHT]);

        positionMasque.x=balle._position.x-700+6;
        positionMasque.y=balle._position.y-700+6;

        if(fin==1) return;
        SDL_BlitSurface(balle._texture, NULL, ecran, &balle._position);
        if(masque==1)SDL_BlitSurface(textureMasque, NULL, ecran, &positionMasque);
        if(keystates[SDLK_p])
        {
            if(masque==0) masque=1;
            else masque=0;
            SDL_Delay(100);
        }
        SDL_Flip(ecran);
    }
}
