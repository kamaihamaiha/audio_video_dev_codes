#include <libavutil/log.h>
#include <libavformat/avformat.h>

/**
 * meta info
 *
 */

int main(){

    int ret;
    // input file
    const char *media_file = "../test.mp4";
    AVFormatContext *avFmtCtx = NULL;

    // set log level
    av_log_set_level(AV_LOG_DEBUG);

    ret = avformat_open_input(&avFmtCtx, media_file, NULL, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Can't open file: %s\n", av_err2str(ret));
        return -1;
    }

    av_dump_format(avFmtCtx, 0, media_file, 0);

    avformat_close_input(&avFmtCtx);
    return 0;
}
