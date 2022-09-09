#include <stdio.h>
#include <SDL.h>

int main(int argc, char *argv[]) {

  SDL_Event event;
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

  // 处理事件. 参考：https://www.jianshu.com/p/f9437ae573e4
  int quit = 0;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      } else {
        SDL_Log("event type is %d", event.type);
      }
    }
  }
  /*do { // 这个是课程方法，mac 上不行
    SDL_WaitEvent(&event);
    switch (event.type) {
      case SDL_QUIT:
        quit = 0;
        break;
      default:
        SDL_Log("event type is %d", event.type);
    }
  } while (!quit);*/

  __DWINDOW:
  SDL_DestroyWindow(window);

  __EXIT:
  SDL_Quit();

  return 0;
}