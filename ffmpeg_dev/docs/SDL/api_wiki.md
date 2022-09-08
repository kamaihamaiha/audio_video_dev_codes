## SDL 使用

### 基本使用
[代码](../../SDL/first.c)   
  - 编译命令：` gcc -g -o demo first.c `pkg-config --cflags --libs sdl2``

- 引入头文件: `#include <SDL.h>`
- 初始化/退出: `SDL_Init()/SDL_Quit()`
- 创建/销毁窗口: `SDL_CreateWindow()/SDL_DestoryWindow()`
- 创建渲染：`SDL_CreateRender()`

#### 在项目中，把 SDL 库的 头文件/库文件 路径引进来

- 先查看下 SDL 库的头文件路径：`pkg-config --cflgs sdl2`
  - 或者用 `brew info sdl2`
- 然后在 CMakeLists.txt 中引入路径