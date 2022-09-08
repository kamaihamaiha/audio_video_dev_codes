#include <stdio.h>
#include <SDL.h>

int main(int argc, char *argv[]) {

  SDL_Window *window = NULL;
  SDL_Renderer *render = NULL;
  SDL_Init(SDL_INIT_VIDEO);

  window = SDL_CreateWindow("my sdl window", 200, 200, 640, 480, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("create sdl window failed!\n");
    goto __EXIT;
  }

  // create Render
  render = SDL_CreateRenderer(window, -1, 0);
  if (!render) {
    printf("create sdl renderer failed!\n");
    goto __DWINDOW;
  }
  SDL_RenderClear(render);
  SDL_SetRenderDrawColor(render, 200, 40, 120, 255);
  SDL_RenderPresent(render);

  SDL_Delay(10000);

  __DWINDOW:
  SDL_DestroyWindow(window);

  __EXIT:
  SDL_Quit();

  return 0;
}