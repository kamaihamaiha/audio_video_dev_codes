#include <stdio.h>
#include <SDL.h>

int main(int argc, char *argv[]) {

  SDL_Event event;
  SDL_Window *window = NULL;
  SDL_Renderer *render = NULL;
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Rect rect;
  rect.w = 30;
  rect.h = 30;

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
  /*SDL_RenderClear(render);
  SDL_SetRenderDrawColor(render, 200, 40, 120, 255);
  SDL_RenderPresent(render);*/

  // 处理事件. 参考：https://www.jianshu.com/p/f9437ae573e4
  int quit = 0;

  // 纹理
  SDL_Texture *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 640,480);
  if (!texture) {
    SDL_Log("Failed to create sdl texture!");
    goto __DRENDER;
  }
  while (!quit) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT){
        quit = 1;
      } else {
        SDL_Log("event type is %d", event.type);
      }
    }

    // 创建小方块
    rect.x = rand() % 600;
    rect.y = rand() % 400;

    // 纹理颜色
    SDL_SetRenderTarget(render, texture);
    SDL_SetRenderDrawColor(render, 0, 0, 0, 200);
    SDL_RenderClear(render);
    // 方块颜色
    SDL_RenderDrawRect(render, &rect);
    SDL_SetRenderDrawColor(render, 255, 0, 0, 0);
    SDL_RenderFillRect(render, &rect);

    // 把纹理输出到窗口
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderCopy(render, texture, NULL, NULL); // 纹理交给显卡处理

    SDL_RenderPresent(render); // 输出到屏幕
  }
  SDL_DestroyTexture(texture);

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

  __DRENDER:
  SDL_DestroyRenderer(render);

  __DWINDOW:
  SDL_DestroyWindow(window);

  __EXIT:
  SDL_Quit();

  return 0;
}