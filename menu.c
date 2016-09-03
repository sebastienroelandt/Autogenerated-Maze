# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include "labyrinthe.h"

typedef struct
{
    SDL_Rect _position;
    SDL_Rect _positiontexte;
    SDL_Surface *_texte;
    SDL_Surface *_texture;
    int _clic;
} BOUTON;

BOUTON AjoutBouton(int positionX, int positionY, char *texte)
{
    BOUTON bouton;
    bouton._position.x=positionX;
    bouton._position.y=positionY;

    bouton._texture=IMG_Load("Bouton_jouer1.png");

    TTF_Font *police = TTF_OpenFont("CaslonRoman.ttf", 35);
    SDL_Color couleurNoire = {0, 0, 0};

    bouton._texte=TTF_RenderText_Blended(police, texte, couleurNoire);
    bouton._positiontexte.x=bouton._position.x+bouton._texture->w/2-bouton._texte->w/2;
    bouton._positiontexte.y=bouton._position.y+bouton._texture->h/2-bouton._texte->h/2;

    bouton._clic=0;

    return bouton;
}

BOUTON modificationBouton(BOUTON bouton, int sourisX, int sourisY, SDL_Event event)
{
    if(sourisX>bouton._position.x&&sourisX<(bouton._position.x+bouton._texture->w)
        &&sourisY>bouton._position.y&&sourisY<(bouton._position.y+bouton._texture->h))
    {
        bouton._texture=IMG_Load("Bouton_jouer2.png");
        if (event.button.button == SDL_BUTTON_LEFT) bouton._clic=1;
    }
    else  bouton._texture=IMG_Load("Bouton_jouer1.png");

    return bouton;
}

int menu (SDL_Surface *ecran)
{
    TTF_Init();
    SDL_Event event;

    BOUTON jouer = AjoutBouton(ecran->w/2-150,ecran->h/3-25,"Jouer");
    BOUTON quitter = AjoutBouton(ecran->w/2-150,ecran->h*2/3-25,"Quitter");

    int continuer=1;
    while(continuer)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        jouer = modificationBouton(jouer, event.button.x, event.button.y, event);
        quitter = modificationBouton(quitter, event.button.x, event.button.y, event);

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format,255,255,255));

        SDL_BlitSurface(jouer._texture, NULL, ecran, &jouer._position);
        SDL_BlitSurface(jouer._texte, NULL, ecran, &jouer._positiontexte);
        SDL_BlitSurface(quitter._texture, NULL, ecran, &quitter._position);
        SDL_BlitSurface(quitter._texte, NULL, ecran, &quitter._positiontexte);

        if(jouer._clic) return 1;
        if(quitter._clic) return 0;
        SDL_Flip(ecran);
        SDL_Delay(20);
    }

    SDL_Quit();
    TTF_Quit();
}
