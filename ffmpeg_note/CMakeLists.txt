cmake_minimum_required(VERSION 3.17)
project(ffmpeg_note C)

set(CMAKE_C_STANDARD 99)

# 1. ffmpeg 安装路径
set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/4.2.1_1)

# 2. ffmpeg 头文件
include_directories(${FFMPEG_DIR}/include/)

# 3. ffmpeg 库文件
link_directories(${FFMPEG_DIR}/lib/)

add_executable(ffmpeg_note main.c)

# 4. 链接库
target_link_libraries(
        ffmpeg_note
        avutil
        avformat
        avfilter
        avdivice
        avcodec
        swscale
        swresample
)

