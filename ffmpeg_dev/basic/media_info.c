#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int argc, char* argv[]){

  int ret;
  const char* test_media_file = "./test.mp4";
  AVFormatContext *fmt_ctx = NULL;
  // 设置日志级别
  av_log_set_level(AV_LOG_INFO);

  // 该函数已经过时了，不调用也可以
//  av_register_all();

  // 第三个参数是格式，输入 NULL，就会根据实际的媒体文件后缀名解析文件
  ret = avformat_open_input(&fmt_ctx, test_media_file, NULL, NULL);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Can't open file: %s\n", av_err2str(ret));
    return -1;
  }

  // param2: 流的索引值，直接写 0
  // param4: 输入流（0）还是输出流（1）
  av_dump_format(fmt_ctx, 0, test_media_file, 0);

  avformat_close_input(&fmt_ctx);

  return 0;
}