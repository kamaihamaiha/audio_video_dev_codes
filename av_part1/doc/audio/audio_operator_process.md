## 音频处理流程

- 基于：直播客户端的处理流程
- 音频数据的流转

#### 直播客户端的处理流程
<font color="red">共享端</font> --> <font color="green">观看端</font>   
<font color="red">音视频采集 => 音视频编码</font> --> 传输 --> <font color="green">音视频解码 => 音视频渲染</font>

#### 音频数据流
PCM: 原始的数据   
<font color="red">PCM</font> => <font color="green">aac/mp3</font> => <font color="blue">mp4/flv</font>    
<font color="red">PCM</font> <= <font color="green">aac/mp3</font> <= <font color="blue">mp4/flv</font> 