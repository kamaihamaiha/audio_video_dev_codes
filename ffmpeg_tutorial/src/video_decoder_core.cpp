#include <ostream>
#include "io_data.h"
#include "video_decoder_core.h"

extern "C" {
 #include <libavcodec/avcodec.h>
}

static AVCodec *codec = nullptr;
static AVCodecContext *codec_ctx = nullptr;
static AVCodecParserContext *parser = nullptr; // 码流解析器的句柄

static AVFrame *frame = nullptr;
static AVPacket *pkt = nullptr;

int32_t init_video_decoder(){
  codec = avcodec_find_decoder(AV_CODEC_ID_H264);
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

void destroy_video_decoder(){
    av_parser_close(parser);
    avcodec_free_context(codec_ctx);
    av_frame_free(frame);
    av_packet_free(pkt);
}

