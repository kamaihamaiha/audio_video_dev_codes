# Mac 下编译安装 FFmpeg
### 安装方式：
- brew 方式：不用下载源码，不用编译，直接安装，方便。但是无法定制化。
- 通过源码方式
---
## 1. brew 方式

1. 打开 Terminal
   ```shell
    brew search ffmpeg
   ```
   结果如下：
   ```shell
    ==> Formulae
    ffmpeg ✔            ffmpeg2theora       ffmpeg@2.8          ffmpegthumbnailer
   ```
   注意：✔ 是因为本地已经安装了。
2. 安装
    ```shell
     brew install ffmpeg
    ```

## 2. 通过源码方式

步骤：
0. 下载源码
1. 编译
2. 安装
3. 配置环境变量

### step0 下载源码
- [FFmpeg 源码下载](http://ffmpeg.org/download.html)

### step1 运行编译脚本
进入到源码根目录，然后执行脚本：
```shell
./configure --prefix=/usr/local/ffmpeg \
--enable-gpl \
--enable-nonfree \
--enable-libfdk-aac \
--enable-libx264 \
--enable-libx265 \
--enable-filter=delogo \
--enable-debug \
--disable-optimizations \
--enable-libspeex \
--enable-videotoolbox \
--enable-shared \
--enable-pthreads \
--enable-version3 \
--enable-hardcoded-tables \
--cc=clang \
--host-cflags= \
--host-ldflags=

# 上面用到了很多参数，也可以少一点参数，如：
./configure --prefix=/usr/local/ffmpeg 
--enable-debug=3 
--disable-static 
--disable-shared
```
在该目录下生成了 config.asm   
注：可以用 `./configure --help` 查看所有到参数。
### step2 安装
```shell
# 执行安装命令
sudo make -j 8 && sudo make install
```

---

理想情况是上面的，实际上肯定会遇到问题，因为缺少很多东西。

####  缺少 nasm/yasm

**处理:**
```
#安装yasm
# 1. 下载
curl http://www.tortall.net/projects/yasm/releases/yasm-1.2.0.tar.gz >yasm.tar.gz
# 2. 解压
sudo tar xzvf yasm.tar.gz -C /usr/local/
# 3. 切换目录
cd /usr/local/yasm-1.20
# 4. 运行
sudo ./configure
# 5. 编译文件
sudo make
# 6. 安装
sudo make install
```
####  缺少 libfdk_aac
1. 下载

```shell
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null
```



2. 安装

```shell
brew install fdk-aac
```

#### 安装 pkg-config / x264 / x265 / speed

```shell
brew install x264
brew install x265
brew insall speex
brew insall pkg-config
```

#### 配置 pkg-config-path

```shell
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/user/local/lib/pkgconfig:/user/lib/pkgconfig:/user/local/SDL2/lib/pkgconfig:/user/local/ffmpeg/lib/pkgconfig
export PATH=$PATH:/user/local/ffmpeg/bin
```

#### 安装的 FFmpeg 没有 ffplay

是因为没有 SDL 库。所以在执行 .configure XXX 的时候，在目录下生成的 configure.asm 文件中，%define CONFIG_FFPLAY 0，而不是 1。也就是说你自定义安装的 FFmpeg 的配置中，没有 ffplay。所以要安装 SDL。

我安装了 1.2 版本，不行。又去官网下载了 2.0 版本，也不行。直接用 brew 命令安装可以了。

```shell
brew install sdl2
```

##### 然后，重新来一遍

```shell
# 清除旧的 config.asm
make distclean
# my_build.sh 把执行命令写成脚本了。
sudo ./my_build.sh
# 这时候，再看下 config.asm, %define CONFIG_PLAY 1。开始编译安装
sudo make && make install
```

##### 等待。。。

大约 8 分钟后，完成了。cd /usr/local/ffmpeg。看看自己编译安装的 FFmpeg。

##### 安装后

在安装的目录下有四个文件夹：
1. bin
2. include
3. lib
4. share

###### 1. bin
存放 ffmpeg 所有的命令、工具。里面有：
```shell
bin
├── ffmpeg
├── ffplay
└── ffprobe
```
###### 2. include
存放所有的头文件，里面有：
```shell
include
├── libavcodec
├── libavdevice
├── libavfilter
├── libavformat
├── libavutil
├── libpostproc
├── libswresample
└── libswscale
```

###### 3. lib
生成的动态库或者静态库，里面有：
```shell
lib
├── libavcodec.58.59.101.dylib
├── libavcodec.58.dylib
├── libavcodec.a
├── libavcodec.dylib -> libavcodec.58.dylib
├── libavdevice.58.9.100.dylib
├── libavdevice.58.dylib
├── libavdevice.a
├── libavdevice.dylib -> libavdevice.58.dylib
├── libavfilter.7.61.100.dylib
├── libavfilter.7.dylib
├── libavfilter.a
├── libavfilter.dylib -> libavfilter.7.dylib
├── libavformat.58.33.100.dylib
├── libavformat.58.dylib
├── libavformat.a
...
```

###### 4. share
例子和文档，里面有：
```shell
share
├── ffmpeg
└── man
```

### step3 配置环境变量

打开 .bash_profile 然后，添加 path：
```shell
export FFMPEG_HOME="/usr/local/ffmpeg"
...

export PATH=$PATH:$FFMPEG_HOME/bin
```