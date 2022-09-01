## FFmpeg 编解码实战

- [中级开发介绍](#中级开发介绍)
- [h264编码](#h264编码)
- [视频转图片详解](#视频转图片详解)
- [AAC编码](#AAC编码)

---

### 中级开发介绍

- [H264 解码](#H264解码)
- [H264 编码](#h264编码)
- AAC 解码
- AAC 编码

#### H264编码
[代码](../../middle/encode_video.c)

##### H264编码步骤

- 查找编码器(通过名字): `avcodec_find_encoder_by_name`
- 设置编码参数，并打开编码器：`avcodec_open2`
- 编码:`avcodec_encode_video2`

#### H264解码
[代码](../../middle/decode_video.c)
- 添加头文件：`libavcodec/avcodec.h`
- 常用数据结构体：
    - `ACVodec` 编码器结构体
    - `AVCodecContext` 编码器上下文
    - `AVFrame` 解码后的帧
- 结构体内存的分配与释放
    - `av_frame_alloc` / `av_frame_free()`
    - `avcodec_alloc_context3()` 
    - `avcodec_free_context()` 
- 解码步骤

##### H264解码步骤
- 查找解码器: `avcodec_find_decoder`
- 打开解码器: `avcodec_open2`  
- 解码: `avcodec_decode_video2`


### 视频转图片详解

- [代码](../../middle/decode_video.c)
  - 可以成功编译，运行

编译命令：
```shell
clang -g -o demo decode_video.c `pkg-config --libs libavformat libavcodec libswscale`
```


### AAC编码

[代码](../../middle/encode_audio.c)
  - 编译命令：
    ```shell
    gcc -g -o demo encode_audio.c `pkg-config --libs libavcodec`
    ```
  - 执行程序   
    `./demo demo.aac`
    

- 编码流程与视频相同
  - 1.添加头文件
  - 2.注册编解码器
  - 3.通过名字找到编解码器
  - 4.设置参数
  - 5.打开编解码器
  - 6.获取数据
  - 7.编码
- 编码函数：`avcodec_encodec_audio2`