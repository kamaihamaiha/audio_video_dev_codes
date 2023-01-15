#pragma

#include <stdint.h>


// init video encoder
int32_t init_video_encoder(const char* codec_name);


// destroy video encoder
void destroy_video_encoder();

// 对 yuv 输入图像进行循环编码
int32_t encoding(int32_t frame_cnt);




