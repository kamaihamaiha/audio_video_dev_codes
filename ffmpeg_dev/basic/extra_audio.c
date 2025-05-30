#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>
#define ADTS_HEADER_LEN  7;

/**
 * 1. 通过命令行输入媒体文件名称、输入的 aac 音频文件
 * 2. get stream
 * 3. write audio data to aac file
 * 4. 音频文件要加上 ADTS 头
 */

void adts_header(char *szAdtsHeader, int dataLen){

  int audio_object_type = 2;
  int sampling_frequency_index = 7;
  int channel_config = 2;

  int adtsLen = dataLen + 7;

  szAdtsHeader[0] = 0xff;         //syncword:0xfff                          高8bits
  szAdtsHeader[1] = 0xf0;         //syncword:0xfff                          低4bits
  szAdtsHeader[1] |= (0 << 3);    //MPEG Version:0 for MPEG-4,1 for MPEG-2  1bit
  szAdtsHeader[1] |= (0 << 1);    //Layer:0                                 2bits
  szAdtsHeader[1] |= 1;           //protection absent:1                     1bit

  szAdtsHeader[2] = (audio_object_type - 1)<<6;            //profile:audio_object_type - 1                      2bits
  szAdtsHeader[2] |= (sampling_frequency_index & 0x0f)<<2; //sampling frequency index:sampling_frequency_index  4bits
  szAdtsHeader[2] |= (0 << 1);                             //private bit:0                                      1bit
  szAdtsHeader[2] |= (channel_config & 0x04)>>2;           //channel configuration:channel_config               高1bit

  szAdtsHeader[3] = (channel_config & 0x03)<<6;     //channel configuration:channel_config      低2bits
  szAdtsHeader[3] |= (0 << 5);                      //original：0                               1bit
  szAdtsHeader[3] |= (0 << 4);                      //home：0                                   1bit
  szAdtsHeader[3] |= (0 << 3);                      //copyright id bit：0                       1bit
  szAdtsHeader[3] |= (0 << 2);                      //copyright id start：0                     1bit
  szAdtsHeader[3] |= ((adtsLen & 0x1800) >> 11);           //frame length：value   高2bits

  szAdtsHeader[4] = (uint8_t)((adtsLen & 0x7f8) >> 3);     //frame length:value    中间8bits
  szAdtsHeader[5] = (uint8_t)((adtsLen & 0x7) << 5);       //frame length:value    低3bits
  szAdtsHeader[5] |= 0x1f;                                 //buffer fullness:0x7ff 高5bits
  szAdtsHeader[6] = 0xfc;
}

/**
 * 1. 处理参数
 * 2. 打开多媒体文件
 * 3. 从多媒体文件中找到音频流
 * 4. 打开输出文件上下文
 * 5. 为输出文件创建一个新的音频流
 * 6. 设置输出音频参数
 * 7. 写多媒体文件头到输出文件
 * 8. 从源多媒体文件中读音频数据到输出文件中
 * 9. 写多媒体文件尾 到输出文件
 * 10. 释放申请的资源
 */
int main(int argc, char* argv[]){

  int ret;
  int audio_index;
  size_t len;
  const char* src = NULL; // 源文件
  const char* dst = NULL; // 输出文件
  const char* test_media_file = "./test.mp4";
  AVPacket pkt;
  AVFormatContext *fmt_ctx = NULL;
  // 设置日志级别
  av_log_set_level(AV_LOG_INFO);

  // 该函数已经过时了，不调用也可以
//  av_register_all();

  // step1: read two params from console
  if (argc < 3) {
    av_log(NULL, AV_LOG_ERROR, "The count of params should be more than three!\n");
    return -1;
  }

  src = argv[1];
  dst = argv[2];

  if (!src || !dst) {
    av_log(NULL, AV_LOG_ERROR, "src or dst is null!\n");
    return -1;
  }

  // step2: 打开多媒体文件
  // 第三个参数是格式，输入 NULL，就会根据实际的媒体文件后缀名解析文件
  ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Can't open file: %s\n", av_err2str(ret));
    return -1;
  }

  // open binary file
  FILE *dst_fd = fopen(dst, "wb");
  if (!dst_fd) {
    av_log(NULL, AV_LOG_ERROR, "File open failed!\n");
    // close src media file
    avformat_close_input(&fmt_ctx);
    return -1;
  }

  // param2: 流的索引值，直接写 0
  // param4: 输出流（0）还是输出流（1）
  av_dump_format(fmt_ctx, 0, test_media_file, 0);

  // step3: get stream:
  // param2(流的信息，这里是音频流 AVMEDIA_TYPE_AUDIO)
  // param3(流的索引号，这里不知道，就填入 -1)
  // param4(相关流的索引号，这里不知道，就填入 -1)
  audio_index = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
  if (audio_index < 0) {
    av_log(NULL, AV_LOG_ERROR, "Can't find the best stream!\n");
    avformat_close_input(&fmt_ctx);
    fclose(dst_fd);
    return -1;
  }

  // read frame
  av_init_packet(&pkt);
  while (av_read_frame(fmt_ctx, &pkt) >= 0) {
    if (pkt.stream_index == audio_index) { // step3: write audio data to aac file
      char adts_header_buf[7];
      adts_header(adts_header_buf, pkt.size);
      fwrite(adts_header_buf, 1, 7, dst_fd); // 每一个音频数据前面都要加 adts 头

      len = fwrite(pkt.data, 1, pkt.size, dst_fd);
      if (len != pkt.size) { // 发生了错误
        av_log(NULL, AV_LOG_WARNING, "Warning, length of data is not equal size of pkt!\n");
      }
    }

    // release
    av_packet_unref(&pkt);
  }

  avformat_close_input(&fmt_ctx);
  fclose(dst_fd);

  return 0;
}