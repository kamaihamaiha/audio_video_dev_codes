#include <cstdlib>
#include <string>
#include <iostream>

#include "io_data.h"

using namespace std;

// 如果输入的不对的话，提示用户输入
static void usage(const char *program_name) {
    cout << "usage: " << string(program_name) << "input_yuv output_file codec_name" << endl;
}


int main(int argc, char **argv){

    // 小于4个参数，返回
    if(argc < 4) {
        usage(argv[0]);
        return 1;
    }

    // 保存输入参数
    char* input_file_name = argv[1];
    char* output_file_name = argv[2];
    char* codec_name = argv[3];

    cout << "Input file: " << string(input_file_name) << endl;
    cout << "Output file: " << string(output_file_name) << endl;
    cout << "codec: " << string(codec_name) << endl;
    
    // open file
    int32_t ret = open_input_output_files(input_file_name, output_file_name);
    if (ret < 0)
    {
        return ret;
    }

    // ...

    close_input_output_files();
    
}