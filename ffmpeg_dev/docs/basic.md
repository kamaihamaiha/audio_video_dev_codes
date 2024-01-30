## 初级开发

### 涉及的内容

- FFmpeg日志的使用及目录操作
- FFmpeg基本概念和常用结构体
- 复用/解复用及流的操作

### FFmpeg 代码结构

- libavcodec: 提供了一系列编码器的实现
- libavformat: 流协议、多媒体格式、基本IO访问
- libavutil: hash器、解码器、各种工具函数
- libavfilter: 过滤器
- libavdevice: 访问捕获设备、回放设备接口
- libswresample: 混音和重采样
- libswscale: 色彩转换和缩放功能

### FFMpeg 多媒体文件处理
- [ffmpeg 日志系统](../basic/log_demo.c)
- [ffmpeg 文件删除与重命名](./5-3.md)
- [ffmpeg 操作目录及 list 的实现](./5-4.md)  
- [ffmpeg 处理流数据的基本概念](./5-6.md)  
- [实战 打印音/视频信息](./5-7.md)  
- [实战 抽取音频数据](./5-8.md)  
- [实战 抽取视频数据](./5-11.md)  

#### 多媒体文件转封装

- [实战 将 MP4 转成 Flv 格式](./5-14.md)  
- [实战 从 MP4 截取一段视频](./5-16.md)  

- [抽取视频数据](note/extro_video.md)

