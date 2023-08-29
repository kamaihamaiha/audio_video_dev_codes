## 编译 Android 平台的 FFmpeg

### 交叉编译

- 概念：在一个平台上（如：Linux）编译出另外一个平台（如：Android）的工具。
- 工具链：需要交叉编译工具链
- 编写编译脚本

[具体操作，可直接看知乎专栏，之前是成功编译过的](https://zhuanlan.zhihu.com/p/100278969)

---

#### 工具链

在 Android Studio NDK 目录中，如下：

```shell
// /ndk/21.1.6352462/toolchains/aarch64-linux-android-4.9/prebuilt/darwin-x86_64/bin
aarch64-linux-android-addr2line
aarch64-linux-android-ar
aarch64-linux-android-as // 汇编器
aarch64-linux-android-c++filt
aarch64-linux-android-dwp
aarch64-linux-android-elfedit
aarch64-linux-android-gprof
aarch64-linux-android-ld // 链接器
aarch64-linux-android-ld.bfd
aarch64-linux-android-ld.gold
aarch64-linux-android-nm
aarch64-linux-android-objcopy
aarch64-linux-android-objdump
aarch64-linux-android-ranlib
aarch64-linux-android-readelf // 对输出的文件做分析
aarch64-linux-android-size
aarch64-linux-android-strings
aarch64-linux-android-strip // 抽取符号表

```



##### 生成交叉编译工具链(很关键★★★★★)

ndk 下载，课程给了下载地址，在视频 05:50 秒处。

在 ndk 目录下，如：ndk/21.1.6352462/build/tools

`build/tools/make-standalone-toolchain.sh`

参数：

- `--toolchain=arm-linux-androideabi-4.9` : 工具链版本，最新的目前是 4.9
- `--platform=android-19`: 最低支持的 Android 平台版本
- `--install-dir=../toolchain`: 工具链生成在哪个目录

##### 执行命令

```shell
// cd 到 make-standalone-toolchain.sh 的目录
./make-standalone-toolchain.sh \
--toolchain=arm-linux-androideabi-4.9 \
--platform=android-19 \
--install-dir=~/Desktop/toolchain
```

实际上就是把 相应的命令 copy 到指定目录中。

列出 toolchain 的工具：

<img src="./imgs/img-1.png" alt="image-20220215141259329" style="zoom: 50%;" />

---



#### 编写编译脚本

实际就是调用工具链中的工具，生成目标程序。

- [老师写的脚本](https://git.imooc.com/coding-279/coding-279/src/master/build_ffmpeg_for_android.sh)
- [脚本内容](sh_file/build_ffmpeg_sh.md)

注意：

1. 如果执行脚本过程中提示没有权限，则用超级管理员权限运行即可，如``sudo ./xxx.sh``

---

#### 交叉编译结果

```shell
.
├── includes
│   └── armeabi-v7a
│       ├── libavcodec
│       │   ├── ac3_parser.h
│       │   ├── adts_parser.h
│       │   ├── avcodec.h
│       │   ├── avdct.h
│       │   ├── avfft.h
│       │   ├── d3d11va.h
│       │   ├── dirac.h
│       │   ├── dv_profile.h
│       │   ├── dxva2.h
│       │   ├── jni.h
│       │   ├── mediacodec.h
│       │   ├── qsv.h
│       │   ├── vaapi.h
│       │   ├── vdpau.h
│       │   ├── version.h
│       │   ├── videotoolbox.h
│       │   ├── vorbis_parser.h
│       │   └── xvmc.h
│       ├── libavdevice
│       │   ├── avdevice.h
│       │   └── version.h
│       ├── libavfilter
│       │   ├── avfilter.h
│       │   ├── buffersink.h
│       │   ├── buffersrc.h
│       │   └── version.h
│       ├── libavformat
│       │   ├── avformat.h
│       │   ├── avio.h
│       │   └── version.h
│       ├── libavutil
│       │   ├── adler32.h
│       │   ├── aes.h
│       │   ├── aes_ctr.h
│       │   ├── attributes.h
│       │   ├── audio_fifo.h
│       │   ├── avassert.h
│       │   ├── avconfig.h
│       │   ├── avstring.h
│       │   ├── avutil.h
│       │   ├── base64.h
│       │   ├── blowfish.h
│       │   ├── bprint.h
│       │   ├── bswap.h
│       │   ├── buffer.h
│       │   ├── camellia.h
│       │   ├── cast5.h
│       │   ├── channel_layout.h
│       │   ├── common.h
│       │   ├── cpu.h
│       │   ├── crc.h
│       │   ├── des.h
│       │   ├── dict.h
│       │   ├── display.h
│       │   ├── downmix_info.h
│       │   ├── encryption_info.h
│       │   ├── error.h
│       │   ├── eval.h
│       │   ├── ffversion.h
│       │   ├── fifo.h
│       │   ├── file.h
│       │   ├── frame.h
│       │   ├── hash.h
│       │   ├── hdr_dynamic_metadata.h
│       │   ├── hmac.h
│       │   ├── hwcontext.h
│       │   ├── hwcontext_cuda.h
│       │   ├── hwcontext_d3d11va.h
│       │   ├── hwcontext_drm.h
│       │   ├── hwcontext_dxva2.h
│       │   ├── hwcontext_mediacodec.h
│       │   ├── hwcontext_qsv.h
│       │   ├── hwcontext_vaapi.h
│       │   ├── hwcontext_vdpau.h
│       │   ├── hwcontext_videotoolbox.h
│       │   ├── imgutils.h
│       │   ├── intfloat.h
│       │   ├── intreadwrite.h
│       │   ├── lfg.h
│       │   ├── log.h
│       │   ├── lzo.h
│       │   ├── macros.h
│       │   ├── mastering_display_metadata.h
│       │   ├── mathematics.h
│       │   ├── md5.h
│       │   ├── mem.h
│       │   ├── motion_vector.h
│       │   ├── murmur3.h
│       │   ├── opt.h
│       │   ├── parseutils.h
│       │   ├── pixdesc.h
│       │   ├── pixelutils.h
│       │   ├── pixfmt.h
│       │   ├── random_seed.h
│       │   ├── rational.h
│       │   ├── rc4.h
│       │   ├── replaygain.h
│       │   ├── ripemd.h
│       │   ├── samplefmt.h
│       │   ├── sha.h
│       │   ├── sha512.h
│       │   ├── spherical.h
│       │   ├── stereo3d.h
│       │   ├── tea.h
│       │   ├── threadmessage.h
│       │   ├── time.h
│       │   ├── timecode.h
│       │   ├── timestamp.h
│       │   ├── tree.h
│       │   ├── twofish.h
│       │   ├── tx.h
│       │   ├── version.h
│       │   └── xtea.h
│       ├── libswresample
│       │   ├── swresample.h
│       │   └── version.h
│       └── libswscale
│           ├── swscale.h
│           └── version.h
├── libs
│   └── armeabi-v7a
│       ├── libavcodec.so
│       ├── libavdevice.so
│       ├── libavfilter.so
│       ├── libavformat.so
│       ├── libavutil.so
│       ├── libswresample.so
│       └── libswscale.so
└── pkgconfig
    └── armeabi-v7a
        ├── libavcodec.pc
        ├── libavdevice.pc
        ├── libavfilter.pc
        ├── libavformat.pc
        ├── libavutil.pc
        ├── libswresample.pc
        └── libswscale.pc
```



