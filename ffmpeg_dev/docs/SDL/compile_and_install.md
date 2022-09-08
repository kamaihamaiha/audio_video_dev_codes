## SDL 编译与安装

- Simple DirectMedia Layer
- [网址](https://www.libsdl.org/)
- 下载安装包或者源码

---

### 步骤

1. [下载源码](#下载源码)
2. 生成 Makefile
    - `./configure --prefix=/usr/local`
    - 就是编译脚本，里面是编译规则
3. 编译、安装
    - `sudo make -j 8 && make install`
    - make 是解析编译脚本的程序，实际编译是 gcc/clang
    - `-j 8` 是 8个线程
    

#### 下载源码

[下载地址](https://github.com/libsdl-org/SDL/releases/tag/release-2.24.0)

#### 生成 Makefile

- [源码解压后，进入源码根目录](#源码解压后，进入源码根目录)
- [生成 Makefile](#生成 Makefile)
- [编译&安装](#编译&安装)
 

##### 源码解压后，进入源码根目录

内容如下：   
```text
Android.mk                      Makefile.os2                    VisualC                         android-project-ant             include                         test
BUGS.txt                        Makefile.pandora                VisualC-GDK                     autogen.sh                      mingw                           visualtest
CMakeLists.txt                  Makefile.w32                    VisualC-WinRT                   build-scripts                   sdl2-config-version.cmake.in    wayland-protocols
CREDITS.txt                     README-SDL.txt                  WhatsNew.txt                    cmake                           sdl2-config.cmake.in
INSTALL.txt                     README.md                       Xcode                           cmake_uninstall.cmake.in        sdl2-config.in
LICENSE.txt                     SDL2.spec.in                    Xcode-iOS                       configure                       sdl2.m4
Makefile.in                     SDL2Config.cmake.in             acinclude                       configure.ac                    sdl2.pc.in
Makefile.minimal                TODO.txt                        android-project                 docs                            src
```
##### 生成 Makefile

- 执行 `./configure --prefix=/usr/local`，等待检查完毕. 再次查看，内容如下：   
```text
Android.mk                      Makefile.os2                    TODO.txt                        android-project-ant             docs                            sdl2-config.in
BUGS.txt                        Makefile.pandora                VisualC                         autogen.sh                      include                         sdl2.m4
CMakeLists.txt                  Makefile.rules                  VisualC-GDK                     build-scripts                   libtool                         sdl2.pc
CREDITS.txt                     Makefile.w32                    VisualC-WinRT                   cmake                           mingw                           sdl2.pc.in
INSTALL.txt                     README-SDL.txt                  WhatsNew.txt                    cmake_uninstall.cmake.in        sdl2-config                     src
LICENSE.txt                     README.md                       Xcode                           config.log                      sdl2-config-version.cmake       test
Makefile                        SDL2.spec                       Xcode-iOS                       config.status                   sdl2-config-version.cmake.in    visualtest
Makefile.in                     SDL2.spec.in                    acinclude                       configure                       sdl2-config.cmake               wayland-protocols
Makefile.minimal                SDL2Config.cmake.in             android-project                 configure.ac                    sdl2-config.cmake.in
```
第一列，倒数第三行，生成了 Makefile 

##### 编译&安装

执行 `sudo make -j 8 && make install`

安装的最后输出信息：
```text
/bin/sh ./libtool --quiet --mode=install /usr/bin/install -c build/libSDL2main.la /usr/local/lib/libSDL2main.la
/bin/sh ./libtool --quiet --mode=install /usr/bin/install -c build/libSDL2_test.la /usr/local/lib/libSDL2_test.la
/bin/sh ./build-scripts//mkinstalldirs /usr/local/share/aclocal
/usr/bin/install -c -m 644 /Users/kk/dev/av_dev/SDL-release-2.24.0/sdl2.m4 /usr/local/share/aclocal/sdl2.m4
/bin/sh ./build-scripts//mkinstalldirs /usr/local/lib/pkgconfig
/usr/bin/install -c -m 644 sdl2.pc /usr/local/lib/pkgconfig
/bin/sh ./build-scripts//mkinstalldirs /usr/local/lib/cmake/SDL2
/usr/bin/install -c -m 644 sdl2-config.cmake /usr/local/lib/cmake/SDL2
/usr/bin/install -c -m 644 sdl2-config-version.cmake /usr/local/lib/cmake/SDL2

```

进入：`/usr/local/lib/` 下，查看：
```text
kk@MacBook-Pro-kk lib % ls libSDL*
libSDL2-2.0.0.dylib     libSDL2.a               libSDL2.dylib           libSDL2.la              libSDL2_test.a          libSDL2_test.la         libSDL2main.a           libSDL2main.la
```