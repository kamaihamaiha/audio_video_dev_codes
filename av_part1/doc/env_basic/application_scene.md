## 音视频的应用

- 互动直播系统
- 娱乐直播系统: 斗鱼，和互动直播系统融合
- 音视频特效：配音、美颜、瘦脸、变声
- 音视频编辑：
- 监控系统

本课程以<font color="red">娱乐直播系统</font>为背景讲解。

### 最简单的直播系统：

常用工具：ffmpeg、ffplay、vlc

步骤：
1. 推流： 用 ffmpeg 把媒体文件推送到流媒体服务器：``ffmpeg -i input.mp4 -f flv rtmp://localhost/live/test``
   
2. 拉流： 用 ffplay 拉流: ``ffplay rtmp://localhost/live/test``

#### 架构模型

流媒体服务器 + 推流工具 + 拉流工具

拉流工具：ffplay 或者 VLC