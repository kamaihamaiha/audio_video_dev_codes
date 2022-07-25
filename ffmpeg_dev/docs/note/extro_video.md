### 抽取视频数据

---

### API
 
- Start Code（特征码）
- SPS/PPS
- codec->extradata
---

**Start Code** 

区分视频帧：
1. 读取帧长度，根据长度取数据
2. 在每一帧的前面加上「特征码」

**SPS/PPS**

解码视频参数：宽高、帧率等。实际上会有多个，当分辨率、帧率变化后，就需要用新的。
在每个关键帧都要加入这个。
 
**codec->extradata**

获取 SPS/PPS