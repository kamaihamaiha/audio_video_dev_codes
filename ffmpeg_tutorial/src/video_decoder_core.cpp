#include <iostream>
#include "io_data.h"
#include "video_decoder_core.h"

extern "C" {
 #include <libavcodec/avcodec.h>
}

#define INBUF_SIZE 4096


static AVCodecContext *codec_ctx = nullptr;
static AVCodecParserContext *parser = nullptr; // 码流解析器的句柄

static AVFrame *frame = nullptr;
static AVPacket *pkt = nullptr;

/**
 * 界面视频码流包
*/
static int32_t decode_packet(bool flushing){
  int32_t ret = 0;
  ret = avcodec_send_packet(codec_ctx, flushing ? nullptr : pkt);
  if (ret < 0) {
    return showError(ret, "Error: failed to send packet.", av_err2str(ret));
  }
  while (ret >= 0) {
    ret = avcodec_receive_frame(codec_ctx, frame);
    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
      return 1;
    } else if (ret < 0) {
      return showError(-1, "Error: failed to receive frame.");
    }

    if (flushing) {
      std::cout << "Flushing:";
    }

    std::cout << "Write frame pic_num:" << frame->coded_picture_number << std::endl;
    write_frame_to_yuv(frame);
  }
}


int32_t init_video_decoder(){
  const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
  if (!codec)  return showError(-1, "Error: could not find codec.");

  parser = av_parser_init(codec->id);
  if (!parser)  return showError(-1, "Error: could not init parse.");

  codec_ctx = avcodec_alloc_context3(codec);
  if (!codec_ctx) return showError(-1, "Error: could not alloc codec.");

  int32_t ret = avcodec_open2(codec_ctx, codec, nullptr);
  if (ret < 0) return showError(ret, "Error: could not open codec.");

  frame = av_frame_alloc();
  if (!frame) return showError(ret, "Error: could not allow frame.");

  pkt = av_packet_alloc();
  if (!pkt) return showError(ret, "Error: could not allow pkt.");

}

int32_t decoding() {
  uint8_t inbuf[INBUF_SIZE] = {0};
  int32_t result = 0;
  uint8_t *data = nullptr;
  int32_t data_size = 0;

  while (!end_of_input_file()) {
    result = read_data_to_buffer(inbuf, INBUF_SIZE, data_size);
    if (result < 0){
      return showError(-1, "Error: read_data_to_buf failed.");
    }

    data = inbuf;
    while (data_size > 0){
      result = av_parser_parse2(parser, codec_ctx, &pkt->data, &pkt->size, data, data_size,
                                AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
      if (result < 0){
        return showError(-1, "Error: av_parser_parse2 failed.");
      }

      data += result;
      data_size -= result;

      if (pkt->size) {
         std::cout << "Parsed packet size:" << pkt->size << std::endl;
         result = decode_packet(false);
         if(result < 0) break;
      }
    }
  }
  result = decode_packet(true);
  if(result < 0) return result;

  return 0;
}


void destroy_video_decoder(){
    av_parser_close(parser);
    avcodec_free_context(&codec_ctx);
    av_frame_free(&frame);
    av_packet_free(&pkt);
}

