#include <stdio.h>
#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int argc, char **argv) {
    /**
     * 1. 处理一些参数
     * 2. 打开多媒体文件
     * 3. 从多媒体文件中找到音频流
     * 4. 打开目标文件的上下文
     * 5. 为目标文件，创建一个新的音频流
     * 6. 设置输出音频参数
     * 7. 写多媒体文件到目标文件
     * 8. 从源多媒体文件中读到音频数据到目标文件中
     * 9. 写多媒体文件尾到文件中
     * 10. 将申请的资源释放掉
    */

    int ret;
    AVFormatContext *fmt_ctx = NULL;
    AVFormatContext *target_fmt_ctx = NULL;
    AVStream *output_stream = NULL;

    // av log
    av_log_set_level(AV_LOG_DEBUG);

    if (argc < 3) {
        av_log(NULL, AV_LOG_ERROR, "params must be more than 2!\n");
        return -1;
    }

    // step1: src(input file) & dest(output file)
    const char* src;
    const char* dest;

    src = argv[1];
    dest = argv[2];

    if (dest == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can't find output file!");
        return -1;
    }

    // step2
    ret = avformat_open_input(&fmt_ctx, src, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open input file: %s\n", av_err2str(ret));
        avformat_close_input(&fmt_ctx);
        return -1;
    }

    // step3
    ret = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't find audio stream: %s\n", av_err2str(ret));
        return -1;
    }

    // step4
    target_fmt_ctx = avformat_alloc_context();
    if (target_fmt_ctx == NULL) {
        av_log(NULL, AV_LOG_ERROR, "Can't alloc context for target file: %s\n", av_err2str(ret));
        return -1;
    }
    target_fmt_ctx->oformat = av_guess_format(NULL, dest, NULL);

    // step5
    output_stream = avformat_new_stream(target_fmt_ctx, NULL);

    // step6


    return 0;
}