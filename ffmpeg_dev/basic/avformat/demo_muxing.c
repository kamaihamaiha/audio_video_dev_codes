#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>


int main(int argc, char **argv){

    /**
     * 0. API 注册，新版本不需要了
     * 1. 申请 AVFormatContext
     * 2. 申请 AVStream
     * 3. write header
     * 4. write frame
     * 5. write all
     */

    const char* file_name = "./demo.mp4";
    // 申请 AVFormatContext
   const AVOutputFormat *fmt;
    AVFormatContext *oc;

    avformat_alloc_output_context2(&oc, NULL, "flv", file_name);
    if (!oc){
        printf("Cannot alloc flv format!\n");
        return -1;
    }

    fmt = oc->oformat;

    // 申请 AVStream
    AVStream *st;
    AVCodecContext *avcc;
    st = avformat_new_stream(oc, NULL);



    return 0;
}