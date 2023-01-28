#!/bin/zsh

clang -g -o extra_audio extra_audio_v2.c `pkg-config --libs --cflags libavutil libavformat`