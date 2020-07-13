# 用来指定 CMake 最低版本为3.4.1，如果没指定，执行 cmake 命令时可能会出错
cmake_minimum_required(VERSION 3.4.1)

# 添加在native层log库
find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log )
#添加一个库,根据native-lib.cpp源文件编译一个native-lib的动态库
add_library( native-lib
        SHARED
        native-lib.cpp
        # TODO 我们之后自己编写的cpp文件都会添加在这里比如
        # src/main/cpp/test1.cpp
        # src/main/cpp/test2.cpp
        )



set(distribution_DIR ${PROJECT_SOURCE_DIR}/../../../libs)

#打印信息
MESSAGE("PROJECT_SOURCE_DIR  = ${PROJECT_SOURCE_DIR}")
MESSAGE("CMAKE_SOURCE_DIR  = ${CMAKE_SOURCE_DIR}")
MESSAGE("distribution_DIR  = ${distribution_DIR}")

#设置头文件目录
#include_directories(${distribution_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
#在全局变量CMAKE_CXX_FLAGS后面加上so库文件的目录
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${distribution_DIR}/${CMAKE_ANDROID_ARCH_ABI}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}")


#[[

# FFmpeg编译出了6个库,这里添加----avutil
add_library( avutil-55
        SHARED
        IMPORTED )
set_target_properties( avutil-55
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libavutil-55.so )

# FFmpeg编译出了6个库,这里添加----swresample
add_library( swresample-2
        SHARED
        IMPORTED )
set_target_properties( swresample-2
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libswresample-2.so )

# FFmpeg编译出了6个库,这里添加----avcodec
add_library( avcodec-57
        SHARED
        IMPORTED )
set_target_properties( avcodec-57
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libavcodec-57.so )

# FFmpeg编译出了6个库,这里添加----avfilter
add_library( avdevice-57
        SHARED
        IMPORTED )
set_target_properties( avdevice-57
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libavdevice-57.so )

# FFmpeg编译出了6个库,这里添加----swscale
add_library( swscale-4
        SHARED
        IMPORTED)
set_target_properties( swscale-4
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libswscale-4.so )

# FFmpeg编译出了6个库,这里添加----avformat
add_library( avformat-57
        SHARED
        IMPORTED)
set_target_properties( avformat-57
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libavformat-57.so )

# FFmpeg编译出了6个库,这里添加----avformat
add_library( avfilter-6
        SHARED
        IMPORTED)
set_target_properties( avfilter-6
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/armeabi-v7a/libavfilter-6.so )

]]

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++11 -fPIC -w")


#设置依赖的库(第一个参数必须为目标模块,顺序不能换)
target_link_libraries( # Specifies the target library.
        native-lib
        avutil-55 swresample-2 avcodec-57 avdevice-57 swscale-4 avformat-57 avfilter-6
       # GLESv2 EGL
       # OpenSLES
        -landroid
        # 这里需要注意下,下面这些ffmpeg的so库编译是有先后顺序的
        # 下面的顺序是没问题的,如果想验证编译顺序,可以自己变换顺序试试.
    #    avutil avformat avcodec swscale  swresample avfilter

        # Links the target library to the log library
        # included in the NDK.

        ${log-lib}
        )

