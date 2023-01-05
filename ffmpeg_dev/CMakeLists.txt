cmake_minimum_required(VERSION 3.17)
project(ffmpeg_dev C)

set(CMAKE_C_STANDARD 99)

# 特别注意，不同的电脑下安装路径不同！！！

# ffmpeg 安装路径，可以用 brew info ffmpeg 获取
set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/4.2.1_1)
#set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/4.3.2_3)

# SDL 安装路径
#set(SDL_DIR /usr/local/Cellar/sdl2/2.0.14_1)
set(SDL_DIR /usr/local/Cellar/sdl2/2.0.10)

# ffmpeg 头文件
include_directories(${FFMPEG_DIR}/include/)

# SDL 头文件
include_directories(${SDL_DIR}/include/SDL2)

# ffmpeg 库文件
link_directories(${FFMPEG_DIR}/lib/)

# SDL 库文件
link_directories(${SDL_DIR}/lib/)

add_executable(ffmpeg_dev  work/copy.c)
#add_executable(ffmpeg_dev  SDL/yuv_player.c)
#add_executable(ffmpeg_dev  SDL/sdl_texture.c)
#add_executable(ffmpeg_dev  SDL/sdl_event.c)
#add_executable(ffmpeg_dev  SDL/first.c)
#add_executable(ffmpeg_dev middle/decode_video.c)
#add_executable(ffmpeg_dev middle/encode_video.c)
#add_executable(ffmpeg_dev basic/cutvideo.c)
#add_executable(ffmpeg_dev basic/remuxing.c)
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
        sdl2
)