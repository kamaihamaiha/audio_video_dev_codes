#include <stdio.h>
#include <libavutil/log.h>

/**
 * 命令行下编译：
 * 方式一（路径根据实际安装的 FFmpeg 路径而定）：clang -g -o log_demo log_demo.c -I/usr/local/ffmpeg/include -L/usr/local/ffmpeg/lib -lavutil
 * 方式二：clang -g -o log_demo log_demo.c `pkg-config --cflags --libs libavutil`
 *
 */
// ide（CLion） 编译，配置好 CMakeLists.txt 直接运行就可以
int main(){

  // 设置日志级别: 注意正式版的程序就不使用 DEBUG 级别的日志了（为了提高运行效率）
  av_log_set_level(AV_LOG_DEBUG);

  av_log(NULL, AV_LOG_DEBUG, "Hello debug!\n");
  av_log(NULL, AV_LOG_INFO, "Hello info!\n");
  av_log(NULL, AV_LOG_ERROR, "Hello error!\n");

  return 0;
}