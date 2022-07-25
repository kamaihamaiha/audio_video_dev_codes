#include <stdio.h>
#include <libavformat/avformat.h>

int main(){


  int ret;

  // rename file
  ret = avpriv_io_move("a.log", "b.log");
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Failed to rename file a.log to b.log");
    return -1;
  }
  av_log(NULL, AV_LOG_INFO, "Success to rename file\n");

  // delete file
  // 注意这个文件是在编译后可执行的文件的相对目录，而不是源代码的目录。因此放到 cmake-build-debug 下面测试！！！
  const char file_path[] = "./test.txt";
  ret = avpriv_io_delete(file_path);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Failed to delete file %s", file_path);
    return -1;
  }
  av_log(NULL, AV_LOG_INFO, "Success to delete file");

  return 0;
}