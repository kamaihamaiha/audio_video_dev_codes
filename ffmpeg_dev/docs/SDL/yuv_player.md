## 实现 YUV 播放器

[代码](../../SDL/yuv_player.c)

- [创建线程](#创建线程)
- [更新纹理](#更新纹理)


### 创建线程

- `SDL_CreateThread(SDL_ThreadFunction fn, const char *name, void *data);`
    - fn: 线程执行函数
    - name: 线程名
    - data: 执行函数参数

### 更新纹理

- `SDL_UpdateTexture();`
- `SDL_UpdateYUVTexture();`
    - 效率更高
  
### 测试用 yuv 数据

```shell
# 没有 yuv 数据，要转换下。如下：
ffmpeg -i demo.mp4 demo.yuv
```

运行没有成功，如下：

```text
printf("not enough data: pos:%p, video_end:%p, blank_space_len:%d\n", video_pos, video_end, blank_space_len);
```

