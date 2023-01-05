## SDL 使用

- [基本使用步骤](#基本使用)
- [SDL 窗口渲染](#窗口渲染)
- [SDL 事件基本原理](#事件基本原理)
- [SDL 事件处理](#事件处理)

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

### 窗口渲染
我在 mac 上无法看到窗口，值看到 Dock view 栏有个终端的图标   
- `SDL_CreateRender()/SDL_DestoryRender()`
- `SDL_RenderClear()`
- `SDL_RenderPresent()`

### 事件基本原理

[代码](../../SDL/sdl_event.c)

- SDL 将所有事件都存放在一个队列中
- 所有对事件的操作，其实就是对队列的操作

#### 事件种类

- SDL_WindowEvent: 
- SDL_KeyboardEvent: 
- SDL_MouseMotionEvent: 
- 自定义事件: 

#### 事件处理

- SDL_PollEvent: 轮询处理
- SDL_WaitEvent: 有事件时，激活线程处理

#### toto 要熟悉常见事件类型
