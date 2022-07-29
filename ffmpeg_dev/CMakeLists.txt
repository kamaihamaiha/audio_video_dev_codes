cmake_minimum_required(VERSION 3.17)
project(ffmpeg_dev C)

set(CMAKE_C_STANDARD 99)

# ffmpeg 安装路径，可以用 brew info ffmpeg 获取
set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/4.2.1_1)

# ffmpeg 头文件
include_directories(${FFMPEG_DIR}/include/)

# ffmpeg 库文件
link_directories(${FFMPEG_DIR}/lib/)

add_executable(ffmpeg_dev basic/remuxing.c)
#add_executable(ffmpeg_dev basic/extr_video.c)
#add_executable(ffmpeg_dev basic/extra_audio.c)
#add_executable(ffmpeg_dev basic/media_info.c)
#add_executable(ffmpeg_dev basic/ffmpeg_list.c)
#add_executable(ffmpeg_dev basic/ffmpeg_file.c)
#add_executable(ffmpeg_dev basic/log_demo.c)
#add_executable(ffmpeg_dev main.c basic/extr_video.c)

# 添加链接库
target_link_libraries(
        ffmpeg_dev
        avcodec
        avutil
        avdevice
        avfilter
        avformat
)