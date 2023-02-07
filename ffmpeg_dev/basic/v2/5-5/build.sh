#!/bin/zsh

clang -g -o remux remux_v2.c `pkg-config --libs --cflags libavutil libavformat libavcodec`