#include <stdio.h>
#include <SDL.h>

int main(int argc, char *argv[]) {

    SDL_Window *window = NULL;
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("my sdl window", 200, 200, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("create sdl window failed!\n");
        goto __EXIT;
    }

    SDL_DestroyWindow(window);
    __EXIT:
    SDL_Quit();

    return 0;
}