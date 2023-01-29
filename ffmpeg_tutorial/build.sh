#!/bin/zsh

rm -rf output # clear temp dir
mkdir output
cd output

cmake .. # 在临时目录中创建工程
make
# ./hello
# ./cmd-dir .
#./video_encoder /Users/kk/Documents/videos/martin.yuv output.h264 libx264
./video_decoder ~/Codes/audio_video_dev_codes/ffmpeg_dev/basic/v2/5-4/1.h264 out.yuv