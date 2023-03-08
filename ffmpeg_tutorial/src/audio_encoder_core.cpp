#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/frame.h>
#include <libavutil/samplefmt.h>
}

#include "audio_encoder_core.h"

using namespace std;

static AVCodec *codec = nullptr;
static AVCodecContext *codec_ctx = nullptr;
static AVFrame *frame = nullptr;
static AVPacket *pkt = nullptr;

static enum AVCodecID audio_codec_id;

int32_t init_audio_encoder(const char* codec_name){
  // 判断是否为 MP3 或者 AAC
  if (strcmp(codec_name, "MP3") == 0) {
    audio_codec_id = AV_CODEC_ID_MP3;
    cout << "Select code id: MP3" << endl;
  } else if (strcmp(codec_name, "AAC")) {
    audio_codec_id = AV_CODEC_ID_AAC;
    cout << "Select code id: AAC" << endl;
  } else {
    cerr << "Error invalid audio format." << endl;
    return -1;
  }

  // find encoder by code_name
  codec = avcodec_find_encoder(audio_codec_id);
  if (!codec) {
    cerr << "Error: could not find codec." << endl;
    return -1;
  }

  // alloc avcodec context
  codec_ctx = avcodec_alloc_context3(codec);
  if (!codec_ctx) {
    cerr << "Error: could not alloc codec." << endl;
    return -1;
  }

  // set audio encoder params
  codec_ctx->bit_rate = 12800;
  codec_ctx->sample_fmt = AV_SAMPLE_FMT_FLTP; // 设置采样格式为 fltp
  codec_ctx->sample_rate = 44100;
  codec_ctx->channel_layout = AV_CHANNEL_LAYOUT_STEREO; // 声道布局为立体声
  codec_ctx->channels = 2; // 双声道

  // open codec
  int32_t ret = avcodec_open2(codec_ctx, codec, nullptr);
  if (ret < 0) {
    cerr << "Error: could not open codec." << endl;
    return -1;
  }

  // alloc frame
  frame = av_frame_alloc();
  if (!frame){
    cerr << "Error: could not alloc frame." << endl;
    return -1;
  }

  frame->nb_samples = codec_ctx->frame_size;
  frame->format = codec_ctx->sample_fmt;
  frame->channel_layout = codec_ctx->channel_layout;
  ret = av_frame_get_buffer(frame, 0);
  if (ret < 0){
    cerr << "Error: AVFrame could not get buffer." << endl;
    return -1;
  }

  // alloc packet
  pkt = av_packet_alloc();
  if (!pkt) {
    cerr << "Error: could not alloc packet." << endl;
    return -1;
  }

  return 0;
}


