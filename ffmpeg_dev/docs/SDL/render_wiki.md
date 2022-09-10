## 纹理渲染

[代码](../../SDL/sdl_texture.c)

纹理是把图形的信息描述出来，而不是每一像素都用 rgb 方式。

- 占用主内存空间小
- 计算在显卡的 CPU 上，也就是 GPU

### SDL 渲染基本原理

![](./imgs/img.png)

### SDL 纹理相关 API

- `SDL_CreateTexture()`
    - format: YUV, RGB
    - access: Texture 类型，Target, Stream
- `SDL_DestoryTexture()` 

### SDL 渲染相关 API

- `SDL_SetRenderTarget()`
- `SDL_RenderClear()`
- `SDL_RenderCopy()`
    - 把纹理拷贝到显卡
- `SDL_RenderPresent()`
