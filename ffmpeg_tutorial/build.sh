#!/bin/zsh

rm -rf output # clear temp dir
mkdir output
cd output
export LD_LIBRARY_PATH=./demo/dep/FFmpeg/libs
./configure --prefix=./demo/dep/FFmpeg --enable-gpl --enable-shared --enable-libx265

cmake .. # 在临时目录中创建工程
make
# ./hello
# ./cmd-dir .
#./video_encoder /Users/kk/Documents/videos/martin.yuv output.h264 libx264
#./video_decoder ~/Video/es.h264 output.yuv
./audio_encoder ~/Music/foo.pcm output.mp3 MP3