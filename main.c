# include <stdio.h>
# include <stdlib.h>
# include <SDL.h>
# include <SDL_Image.h>
# include <SDL_ttf.h>
# include "menu.h"

int main(int argc, char **argv)
{
    SDL_Surface *ecran = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    ecran = SDL_SetVideoMode(41/2*25+(41/2+1)*10,41/2*25+(41/2+1)*10,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("It's A-MAZE-ING !",NULL);

    int choix=1;
    while(choix)
    {
        choix = menu (ecran);
        switch (choix)
        {
            case 1:
            labyrinthe(ecran);
            break;
        }
    }

    SDL_Quit();
    exit(EXIT_SUCCESS);
}
