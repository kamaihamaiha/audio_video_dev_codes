#!/bin/zsh

clang -g -o extra_video extra_video_v2.c `pkg-config --libs --cflags libavutil libavformat`