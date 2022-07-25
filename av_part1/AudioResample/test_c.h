//
// Created by 张开旭 on 2021/3/17.
//

#ifndef AV_TUTORIAL_AUDIORESAMPLE_TEST_C_H_
#define AV_TUTORIAL_AUDIORESAMPLE_TEST_C_H_

#include <stdio.h>
#include <string.h>
#include "libavutil/avutil.h"
#include "libavdevice//avdevice.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libswresample/swresample.h"

void set_status(int status);

void rec_audio(void);

#endif //AV_TUTORIAL_AUDIORESAMPLE_TEST_C_H_
