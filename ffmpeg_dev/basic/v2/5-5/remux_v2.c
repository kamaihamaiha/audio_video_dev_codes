#include <stdio.h>
#include "libavutil/log.h"
#include "libavformat/avformat.h"

int main(int argc, char** argv){

    /**
     * 1. 处理一些参数
     * 2. 打开多媒体文件
     * 3. 从多媒体文件中找到视频流
     * 4. 打开目标文件的上下文
     * 5. 为目标文件，创建一个新的视频流
     * 6. 设置输出视频参数
     * 7. 写多媒体文件到目标文件
     * 8. 从源多媒体文件中读到视频数据到目标文件中
     * 9. 写多媒体文件尾到文件中
     * 10. 将申请的资源释放掉
    */

    int ret = -1;
    int index = -1; // 视频流索引
    AVFormatContext *pFmtCtx = NULL;
    AVFormatContext *targetFmtCtx = NULL;
    AVOutputFormat *outFmt = NULL;
//    AVStream *outStream = NULL;
//    AVStream *inStream = NULL;
    AVPacket pkt;
    int stream_index = 0;

    // step1:
    char *src;
    char *dst;
    int *av_stream_map = NULL;

    av_log_set_level(AV_LOG_DEBUG);
    if (argc < 3) { // argv[1]: src ; argv[2]: dst
        av_log(NULL, AV_LOG_INFO, "Arguments must be more than 3!");
        exit(-1);
    }
    
    src = argv[1];
    dst = argv[2];

    // step2: open media file
    ret = avformat_open_input(&pFmtCtx, src, NULL, NULL);
    if(ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "%s\n", av_err2str(ret));
        exit(-1);
    }

    // step4: open the context of target file
    avformat_alloc_output_context2(&targetFmtCtx, NULL, NULL, dst);
    if(!targetFmtCtx) {
        av_log(pFmtCtx, AV_LOG_ERROR, "No memory: 无法给 avformat 分配空间了!");
        goto _ERROR;
    }

    // todo 处理每一路流
    // 申请空间
    av_stream_map = av_calloc(pFmtCtx->nb_streams, sizeof (int));
    if (!av_stream_map){
        av_log(pFmtCtx, AV_LOG_ERROR, "No memory: 无法给 av_stream_map 分配空间了!");
        goto _ERROR;
    }
    for( int i = 0; i < pFmtCtx->nb_streams; i++) {
        AVStream  *inStream = pFmtCtx->streams[i];
        AVStream *outStream = NULL;
        AVCodecParameters *inCodecParam =  pFmtCtx->streams[i];
        if (inCodecParam->codec_type != AVMEDIA_TYPE_AUDIO &&
            inCodecParam->codec_type != AVMEDIA_TYPE_VIDEO &&
            inCodecParam->codec_type != AVMEDIA_TYPE_SUBTITLE) { // 不是音频、视频、字母的情况处理
            av_stream_map[i] = -1;
            continue;
        }
        av_stream_map[i] = stream_index++;

        // step5: create audio stream
        outStream = avformat_new_stream(targetFmtCtx, NULL);
        if (!outStream){
            av_log(targetFmtCtx, AV_LOG_ERROR, "No memory: 无法给 avformat 分配空间了!");
            goto _ERROR;
        }

        // step6: config output video params
        ret = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
        outStream->codecpar->codec_tag = 0; // 根据多媒体文件自动设置编解码器，如果非常了解则设置一个固定的值
    }




    // 绑定
    /**
     * params3: int_cb 回调函数，用不上，传入 NULL
     * params4: options 私有协议的选项，没有的话，传入 NULL
    */
    ret = avio_open2(&targetFmtCtx->pb, dst, AVIO_FLAG_WRITE, NULL, NULL);
    if(ret < 0) {
        av_log(targetFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto _ERROR;
    }

    // step7: write media head info to target file
    ret = avformat_write_header(targetFmtCtx, NULL);
    if(ret < 0) {
        av_log(targetFmtCtx, AV_LOG_ERROR, "%s", av_err2str(ret));
        goto _ERROR;
    } 

    // step8: read video data from source file and write to target file
    while(av_read_frame(pFmtCtx, &pkt) >= 0){
        if(pkt.stream_index == index) { // 是视频流
            // 先改变时间戳: pts, dts
            /**
             * params2: 时间基
             * params3: 时间基
             * params4: 范围
            */
            pkt.pts = av_rescale_q_rnd(pkt.pts, inStream->time_base, outStream->time_base, 
            (AV_ROUND_PASS_MINMAX | AV_ROUND_NEAR_INF));

            // 视频的 dts 计算
            pkt.dts = av_rescale_q_rnd(pkt.dts, inStream->time_base, outStream->time_base, 
            (AV_ROUND_PASS_MINMAX | AV_ROUND_NEAR_INF)); 

            pkt.duration = av_rescale_q(pkt.duration, inStream->time_base, outStream->time_base);
            pkt.stream_index = 0; // 因为只抽取了一路音频，输出文件也只有一路音频，所以 设置为 0
            pkt.pos = -1; // 相对位置，设置为 -1，让系统自己计算

            // 把音频数据写到文件中
            av_interleaved_write_frame(targetFmtCtx, &pkt);
            av_packet_unref(&pkt); // 释放 pkt 
        }
    }

    // step9: write meida tail info 
    av_write_trailer(targetFmtCtx);

    // step10: release
_ERROR:    
    // release & exit
    if(pFmtCtx) {
        avformat_close_input(&pFmtCtx);
        pFmtCtx = NULL;
    }

    if(targetFmtCtx->pb) {
        avio_close(targetFmtCtx->pb);
    }
    if(targetFmtCtx) {
        avformat_free_context(targetFmtCtx);
        targetFmtCtx = NULL;
    }

    if(av_stream_map) {
        av_free(av_stream_map);
    }
    
    return 0;
}