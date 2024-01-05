## FFmpeg 常用命令

### 简介

1. 不同音视频格式转换，如: mp4 -> flv
2. 音频和视频合并,
3. 录制
4. 裁剪
5. 分辨率转换

![处理流程](../imgs/av_process.png)

### 分类

- [基本信息查询命令](#基本信息查询命令)
- [录制命令](#录制命令)
- [分解与复用命令](#分解与复用命令)
- [处理原始数据命令](#处理原始数据命令)
- [裁剪与合并命令](#裁剪与合并命令)
- [图片与视频互转命令](#图片与视频互转命令)
- [直播相关命令](#直播相关命令)
- [滤镜命令](#滤镜命令)

---


### 基本信息查询命令

```shell
-version

# 显示可用的 demuxer
-demuxers

# 显示可用的 muxer
-muxers

# 显示可用的设备
-devices

# 显示所有编解码器
-codecs

# 显示可用的解码器
-decoders

# 显示可用的编码器
-encoders

# 显示比特流 filter
-bsfs

# 显示可用的格式
-formats

# 显示可用的协议
-protocols

# 显示可用的过滤器
-filters

# 显示可用的像素格式
-pix_fmts

# 显示可用的采样格式
-sample_fmts

# 显示channel名称
-layouts

# 显示识别的颜色名称
-colors

```

### 录制命令

- 录制视频
- 录制声音
- 同时录制视频和音频到一个文件: ``ffmpeg -f avfoundation -i "1:0" -r 30 output.mp4``

#### 录制视频

```shell
# mac 下
ffmpeg -f avfoundation -i 1 -r 30 out.yuv
```
- `-f` 指定用什么库采集数据
- `-i` 指定从哪里采集数据，后面的1是文件索引号
- `-r` 指定帧率

执行命令后，终端开始会输出:
```shell
ffmpeg -f avfoundation -i 1 -r 30 out.yuv
ffmpeg version N-95190-gf66458cfc7 Copyright (c) 2000-2019 the FFmpeg developers
  built with Apple clang version 12.0.0 (clang-1200.0.32.28)
  configuration:
  libavutil      56. 35.100 / 56. 35.100
  libavcodec     58. 59.101 / 58. 59.101
  libavformat    58. 33.100 / 58. 33.100
  libavdevice    58.  9.100 / 58.  9.100
  libavfilter     7. 61.100 /  7. 61.100
  libswscale      5.  6.100 /  5.  6.100
  libswresample   3.  6.100 /  3.  6.100
objc[57975]: class `NSKVONotifying_AVCaptureScreenInput' not linked into application
objc[57975]: class `NSKVONotifying_AVCaptureScreenInput' not linked into application
[AVFoundation indev @ 0x7fe103005080] Configuration of video device failed, falling back to default.
[avfoundation @ 0x7fe10380a000] Selected pixel format (yuv420p) is not supported by the input device.
[avfoundation @ 0x7fe10380a000] Supported pixel formats:
[avfoundation @ 0x7fe10380a000]   uyvy422
[avfoundation @ 0x7fe10380a000]   yuyv422
[avfoundation @ 0x7fe10380a000]   nv12
[avfoundation @ 0x7fe10380a000]   0rgb
[avfoundation @ 0x7fe10380a000]   bgr0
[avfoundation @ 0x7fe10380a000] Overriding selected pixel format to use uyvy422 instead.
[avfoundation @ 0x7fe10380a000] Stream #0: not enough frames to estimate rate; consider increasing probesize
Input #0, avfoundation, from '1':
  Duration: N/A, start: 833744.102167, bitrate: N/A
    Stream #0:0: Video: rawvideo (UYVY / 0x59565955), uyvy422, 2880x1800, 1000k tbr, 1000k tbn, 1000k tbc
Stream mapping:
  Stream #0:0 -> #0:0 (rawvideo (native) -> rawvideo (native))
Press [q] to stop, [?] for help
Output #0, rawvideo, to 'out.yuv':
```

上面有录制视频的分辨率: 2880x1800

```shell
# 播放录制的文件 out.yuv
ffplay -s 2880x1800 -pix_fmt uyvy422 out.yuv
```
注意：需要指定格式 uyvy422，因为 ffplay 播放默认格式为 yuv420p

##### 关于设备索引号
在 mac 中查看 avfoundation 库支持的设备

```shell
ffmpeg -f avfoundation -list_devices true -i ""
```

输出如下:
```shell
[AVFoundation indev @ 0x7f994ba041c0] AVFoundation video devices:
[AVFoundation indev @ 0x7f994ba041c0] [0] FaceTime高清摄像头（内建）
[AVFoundation indev @ 0x7f994ba041c0] [1] Capture screen 0
[AVFoundation indev @ 0x7f994ba041c0] AVFoundation audio devices:
[AVFoundation indev @ 0x7f994ba041c0] [0] MacBook Pro麦克风
```

#### 录制声音
```shell
ffmpeg -f avfoundation -i :0 out.wav
```

播放:
``ffplay out.wav``

### 分解与复用命令

![](../imgs/av_demuxer-and-muxer.png)

```shell
# mp4 -> flv
ffmpeg -i demo.mp4 -vcodec copy -acodec copy demo.flv
```
注意: 可能会产生这个错误: ``Codec mpeg4 is not supported in the official FLV specification``; [原因参见](https://coding.imooc.com/learn/questiondetail/Dg8m5YZL1dQ6Wa4B.html)

```shell
# 抽取视频数据
ffmpeg -i out.mp4 -an -vcodec copy out.h264
```
参数说明:
- `-an`: 不要音频 

同理抽取音频
```shell
# 抽取音频数据
ffmpeg -i out.mp4 -vn -acodec copy out.aac
```

### 处理原始数据命令

### 裁剪与合并命令

### 图片与视频互转命令

### 直播相关命令

### 滤镜命令
