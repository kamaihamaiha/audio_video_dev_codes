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


int main(int argc, char **argv) {

  // 0. API 注册
  av_register_all();

  // 1. 申请 SVFormatContext
  AVFormatContext *fmt_ctx;
  AVOutputFormat *o_fmt;
  const char *file_name;
  avformat_alloc_output_context2(&fmt_ctx, NULL, "flv", file_name);

  av_log_set_level(AV_LOG_DEBUG);
  if (!fmt_ctx) {
    av_log(NULL, AV_LOG_ERROR, "cannot alloc flv format\n");
    return 1;
  }

  o_fmt = fmt_ctx->oformat;

  // 2. 申请 AVStream
  AVStream  *av_s;
  AVCodecContext *av_c_c;
  av_s = avformat_new_stream(fmt_ctx, NULL);
  if (!av_s) {
    av_log(NULL, AV_LOG_ERROR, "Could not allocate stream \n");
    return 1;
  }
  return 0;
}
