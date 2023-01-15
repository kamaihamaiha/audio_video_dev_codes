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

int showError(int errorCode, const char* msg);