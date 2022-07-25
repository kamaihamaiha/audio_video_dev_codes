cmake_minimum_required(VERSION 3.17)
#project(AV_Tutorial C)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} C)

set(CMAKE_C_STANDARD 99)

#add_executable(AV_Tutorial main.c)
# 添加 ffmpeg 的头文件目录
include_directories("/usr/local/Cellar/ffmpeg/4.2.1_1/include")

file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/AudioResample*/*.c")
foreach(file ${files})
    get_filename_component(name ${file} NAME)
    add_executable(${name} ${file})
    target_compile_definitions(${name} PUBLIC DEBUG)
endforeach()

