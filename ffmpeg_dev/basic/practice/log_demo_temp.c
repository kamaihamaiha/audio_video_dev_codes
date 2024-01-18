#include <stdio.h>
#include <libavutil/log.h>

int main(){
    av_log_set_level(AV_LOG_ERROR);

    av_log(NULL, AV_LOG_DEBUG, "this is ffmpeg debug log!\n");
    av_log(NULL, AV_LOG_ERROR, "this is ffmpeg error log!\n");

    return 0;
}
