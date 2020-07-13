#include <jni.h>
#include <string>
// for native window JNI
#include <android/native_window_jni.h>
#include <android/native_window.h>
#include <unistd.h>
#include <stdint.h>
#include<android/log.h>

//因为ffmpeg是用C语言写的,所有这里要引入C的编译器,引入头文件
extern "C" {
#include <libavutil/dict.h>
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

#define TAG "mytag"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, TAG, __VA_ARGS__)



extern "C" JNIEXPORT jstring JNICALL
Java_com_example_test1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(av_version_info());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_ffmepgtest_HuGePlayer_native_1start(JNIEnv *env, jobject thiz, jstring path_,
                                                jobject surface) {
    ANativeWindow * nativeWindow = ANativeWindow_fromSurface(env,surface);
    LOGI("进来jni了");
    const char *path = env->GetStringUTFChars(path_, 0);
    //FFmepg 视频绘制

    //初始化网络模块 因为FFmpeg也可以播放网络视频数据
    avformat_network_init();
    LOGI("结束了1");
    //获取AVFormatContext 这个上下文  该上下文是视频的总上下文
    AVFormatContext *formatContext = avformat_alloc_context();
    //设置打开视频的超时时间
    LOGI("结束了2");
    AVDictionary * opts = NULL;
    av_dict_set(&opts, "timeout", "3000000", 0);
    av_register_all(); //在使用之前，注册
    int ret = avformat_open_input(&formatContext, path, NULL, &opts);
    LOGI("结束了3");
    if (ret) {
        LOGI("结束了4");
        return;
    }

    //寻找视频流
    int vidio_stream_idx = -1;
    avformat_find_stream_info(formatContext, NULL);
    LOGI("结束了5");
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            vidio_stream_idx = i;
            break;
        }
    }
    LOGI("结束了6");
    //视频流索引
    AVCodecParameters *codecpar = formatContext->streams[vidio_stream_idx]->codecpar;

    //拿到解码器
    AVCodec *dec = avcodec_find_decoder(codecpar->codec_id);
    //解码器的上下文
    AVCodecContext *codecContext = avcodec_alloc_context3(dec);
    //将解码器参数copy到解码器上下文
    avcodec_parameters_to_context(codecContext, codecpar);
    //
    avcodec_open2(codecContext, dec, NULL);
    //解码
    AVPacket *packet = av_packet_alloc();
    SwsContext *swsContext = sws_getContext(codecContext->width, codecContext->height,
                                            codecContext->pix_fmt,
                                            codecContext->width, codecContext->height,
                                            AV_PIX_FMT_RGBA, SWS_BILINEAR, 0, 0, 0);
    ANativeWindow_setBuffersGeometry(nativeWindow,codecContext->width,codecContext->height,WINDOW_FORMAT_RGBA_8888);
    //codecContext->pix_fmt;
    ANativeWindow_Buffer outBuffer;
    LOGI("结束了7");
    while (av_read_frame(formatContext, packet) >= 0) {
      //  LOGI("结束了8");
        avcodec_send_packet(codecContext, packet);

        AVFrame *frame = av_frame_alloc();
        ret = avcodec_receive_frame(codecContext, frame);
        if (ret == AVERROR(EAGAIN)) {
           // LOGI("结束了9");
            continue;
        } else if (ret < 0) {
           // LOGI("结束了10");
            break;
        }
        //接收的容器
        uint8_t  *dst_data[4];
        //每一行的首地址
        int dst_linesize[4];
        av_image_alloc(dst_data,dst_linesize,codecContext->width,codecContext->height,AV_PIX_FMT_RGBA,1);

        sws_scale(swsContext,frame->data,frame->linesize,0,frame->height,dst_data,dst_linesize);

        ANativeWindow_lock(nativeWindow,&outBuffer,NULL);
        uint8_t *firstWindow = static_cast<uint8_t *>(outBuffer.bits);
        uint8_t *src_data = dst_data[0];

        int destStride = outBuffer.stride*4;
        int src_linesize = dst_linesize[0];
        for (int i = 0; i < outBuffer.height; ++i) {
            //一行行的进行内存copy来进行渲染
            memcpy(firstWindow+i*destStride,src_data+i*src_linesize,destStride);
        }


        ANativeWindow_unlockAndPost(nativeWindow);
        usleep(1000*16);
        av_frame_free(&frame);
       // LOGI("结束了11");
    }

    env->ReleaseStringUTFChars(path_, path);
}