#pragma

#include <stdint.h>
extern "C" {
    #include "libavcodec/avcodec.h"
}

int32_t open_input_output_files(const char* input_name, const char* output_name);

void close_input_output_files();