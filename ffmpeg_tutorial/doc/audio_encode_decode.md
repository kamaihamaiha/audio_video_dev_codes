## 音频编解码

使用 libavcodec 库中的相关 API

### 音频编码

- 把 PCM 格式的原始音频采样数据编码为 MP3 或 AAC 格式的音频文件

#### 从手机录制的 pcm 数据

- 录制后 copy 到电脑
- 先用 ffmpeg 播放下
    ```shell
    ffplay -ac 1 -ar 44100 -f s16le foo.pcm
    ```


### 音频解码

- 把 MP3 或 AAC 格式的音频文件解码为 PCM 格式的音频采样数据