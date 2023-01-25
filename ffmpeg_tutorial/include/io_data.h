#pragma

#include <stdint.h>
extern "C" {
    #include "libavcodec/avcodec.h"
}

int32_t open_input_output_files(const char* input_name, const char* output_name);

void close_input_output_files();

// 读取 yuv 数据
int32_t read_yuv_to_frame(AVFrame *frame);

// 写出码流数据
void write_pkt_to_file(AVPacket *pkt);

// 输出解码图形数据
int32_t write_frame_to_yuv(AVFrame* frame);

int showError(int errorCode, const char* msg);

// 把从输入文件中读取的数据添加到缓存
int32_t read_data_to_buffer(uint8_t *buf, int32_t size, int32_t& out_size);

// 判断输入文件是否到达结尾
int32_t end_of_input_file();