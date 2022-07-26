#include <libavutil/log.h>
#include <libavformat/avformat.h>

int main(int argc, char* argv[]){

  int ret;
  AVIODirContext  *ctx = NULL;
  AVIODirEntry  *entry = NULL;
  // 设置 av 级别
  av_log_set_level(AV_LOG_INFO);

  // open dir
  const char dir_path[] = "./";
  ret = avio_open_dir(&ctx, dir_path, NULL);
  if (ret < 0) {
    av_log(NULL, AV_LOG_ERROR, "Cant ope dir: %s\n", av_err2str(ret));
    goto __fail;
//    return -1;
  }

  // 访问目录中的所有项
  while (1) {
    ret = avio_read_dir(ctx, &entry);
    if (ret < 0) {
      av_log(NULL, AV_LOG_ERROR, "Cant ope dir: %s\n", av_err2str(ret));
      return -1;
    }

    // 判断是否到了最后一个
    if (!entry) {
      break;
    }

    // 打印每一项的信息
    av_log(NULL, AV_LOG_INFO, "%12"PRId64" %s \n", entry->size, entry->name);

    // release entry
    avio_free_directory_entry(&entry);
  }

  __fail:
  // close
  avio_close_dir(&ctx);

  return 0;
}