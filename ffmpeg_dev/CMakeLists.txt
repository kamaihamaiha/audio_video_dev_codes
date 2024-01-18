cmake_minimum_required(VERSION 3.17)
project(ffmpeg_dev C)

set(CMAKE_C_STANDARD 99)

# 特别注意，不同的电脑下安装路径不同！！！

# ffmpeg 安装路径，可以用 brew info ffmpeg 获取
#get_filename_component(parent_dir ${PROJECT_SOURCE_DIR} DIRECTORY)
#set(FFMPEG_DIR “${parent_dir}/ffmpeg_tutorial/demo/dep/FFmpeg”)
# set(FFMPEG_DIR /usr/local/ffmpeg)
# 我自己的 MacBook Pro 上
#set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/4.2.1_1)
set(FFMPEG_DIR /usr/local/Cellar/ffmpeg/6.0)

# SDL 安装路径
#set(SDL_DIR /usr/local/Cellar/sdl2/2.0.14_1)
# 我自己的 MacBook Pro 上
#set(SDL_DIR /usr/local/Cellar/sdl2/2.0.10)
#set(SDL_DIR /usr/local/Cellar/sdl2/2.26.2)

# ffmpeg 头文件
include_directories(${FFMPEG_DIR}/include/)

# SDL 头文件
#include_directories(${SDL_DIR}/include/SDL2)

# ffmpeg 库文件
link_directories(${FFMPEG_DIR}/lib/)

# SDL 库文件
#link_directories(${SDL_DIR}/lib/)

# 指定输出目录为 out
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/basic/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/basic/avformat/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/basic/practice/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/middle/*.c"
#        "${CMAKE_CURRENT_SOURCE_DIR}/SDL/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/work/*.c"
        )

# NAME_WE 去掉文件名后缀
foreach(file ${files})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_compile_definitions(${name} PUBLIC DEBUG)

    target_link_libraries(
            ${name}
            avcodec
            avutil
            avdevice
            avfilter
            avformat
            swresample
            swscale
#            sdl2
    )
endforeach()

#add_executable(ffmpeg_dev  work/copy.c)
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
#add_executable(ffmpeg_dev basic/extr_video.c)
# add_executable(ffmpeg_dev basic/avformat/demo_muxing.c)
#add_executable(ffmpeg_dev basic/v2/5-5/remux_v2.c)
#add_executable(ffmpeg_dev main.c)
# 
# 添加链接库
#[[
target_link_libraries(
        ffmpeg_dev
        avcodec
        avutil
        avdevice
        avfilter
        avformat
        swresample
        swscale
        sdl2
)]]
