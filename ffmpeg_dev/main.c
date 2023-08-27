#include <stdio.h>

#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
#include "libavutil/avutil.h"

int main() {
    printf("av_version_info: %s\n", av_version_info());
    printf("avcodec_configuration: %s", avcodec_configuration());
    return 0;
}
