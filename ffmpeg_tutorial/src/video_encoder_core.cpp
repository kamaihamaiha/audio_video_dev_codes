#include "video_encoder_core.h"
#include "io_data.h"
#include <iostream>
#include <string>

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavutil/opt.h"
    #include "libavutil/imgutils.h"
}

using namespace std;

static const AVCodec *codec = nullptr;
static AVCodecContext *codec_ctx = nullptr;
static AVFrame *frame = nullptr;
static AVPacket *pkt = nullptr;

/**
 * 1. 验证编码器名称
 * 2. 查找编码器
 * 3. 创建编码器上下文
 * 4. 配置编码参数
 * 5. 使用指定的 codec 初始化编码器上下文结构
 * 6. 创建图像帧与码流包结构
*/
int32_t init_video_encoder(const char* codec_name){
    // step1
    if(strlen(codec_name) == 0) {
        cerr << "Error: empty codec name:" << endl;
        return -1;
    }

    // step2
    codec = avcodec_find_encoder_by_name(codec_name);
    if (!codec) 
    {
        cerr << "Error: could not find codec with codec name:" << string(codec_name) << endl;
       
    }

    // step3
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        cerr << "Error: could not allocate video codec context!" << endl;
    }
    

    // step4
    codec_ctx->profile = FF_PROFILE_H264_HIGH;
    codec_ctx->bit_rate = 2000000;
    codec_ctx->width = 1280;
    codec_ctx->height = 720;
    codec_ctx->gop_size = 10; // 关键帧
    codec_ctx->time_base = (AVRational){1, 25}; 
    codec_ctx->framerate = (AVRational){25, 1};
    codec_ctx->max_b_frames = 3; // 在每个 I帧和 P帧之间插入 3 个B帧
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if(codec->id == AV_CODEC_ID_H264) {
        av_opt_set(codec_ctx->priv_data, "preset", "slow", 0);
    }

    // step5
    int32_t result = avcodec_open2(codec_ctx, codec, nullptr);
    if (result < 0)
    {
        cerr << "Error: could no open codec: " << string(av_err2str(result)) << endl;
        return -1; 
    }
    
    // step6
    pkt = av_packet_alloc();
    if(!pkt) {
        cerr << "Error: could not allocate AVPacket." << endl;
        return -1;
    }

    frame = av_frame_alloc();
    if(!frame) {
        cerr << "Error: could not allocate AVFrame." << endl;
        return -1;
    }

    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;
    frame->format = codec_ctx->pix_fmt;

    result = av_frame_get_buffer(frame, 0);
    if(result < 0){
        cerr << "Error: could not get AVFrame buffer." << endl;
        return -1;
    }

    return 0;
}

void destroy_video_encoder(){
    avcodec_free_context(&codec_ctx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

/**
 * 编码一帧图像数据
 * @param flushing
 * @return
 */
static int32_t encode_frame(bool flushing) {
    int32_t result = 0;
    if(!flushing){
        std::cout << "Send frame to encoder with pts: " << frame->pts << std::endl;
    }

    result = avcodec_send_frame(codec_ctx, flushing ? nullptr : frame);
    if (result < 0){
        std::cerr << "Error: avcodec_send_frame failed." << std::endl;
        return result;
    }

    while (result >= 0) {
        result = avcodec_receive_packet(codec_ctx, pkt);
        if (result == AVERROR(EAGAIN) || result == AVERROR_EOF) {
            return 1;
        } else if (result < 0) {
            std::cerr << "Error: avcodec_receive_packet failed." << std::endl;
            return result;
        }

        if (flushing){
            std::cout << "Flushing:";
        }
        std::cout << "Got encoded package with dts: " << pkt->dts << ", pts: " << pkt->pts << ", " << std::endl;
        write_pkt_to_file(pkt);
    }
    return 0;
}
int32_t encoding(int32_t frame_cnt){
    int ret = 0;
    for(size_t i = 0; i < frame_cnt; i++) {
        ret = av_frame_make_writable(frame);
        if (ret < 0){
            // tips error
            return showError(ret, "Error: could not av_frame_make_writable.");
        }

        ret = read_yuv_to_frame(frame);
        if (ret < 0) {
            //  tips error
            return showError(ret, "Error: read_yuv_to_frame failed.");
        }
        frame->pts = i;

        ret = encode_frame(false);
        if (ret < 0) {
            //  tips error
            return showError(ret, "Error: encode_frame failed.");
        }
    }
    ret = encode_frame(true);
    if (ret < 0) {
        //  tips error
        return showError(ret, "Error: flushing failed.");
    }

    return 0;
}
