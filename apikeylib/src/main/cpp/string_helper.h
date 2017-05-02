#include <string>
#include <sstream>
#include <iostream>
#include <android/log.h>
#include <vector>
#include <iterator>

#ifndef APIKEYNATIVELIB_PWRD_HELPER_H
#define APIKEYNATIVELIB_PWRD_HELPER_H

// 定义日志语句
static const char *TAG = "jack-jni";
#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, PWRDTAG, __VA_ARGS__))
#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, PWRDTAG, __VA_ARGS__))


// 获取数组的大小
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

// 定义转string的方法
namespace jack {
    template<typename T>
    std::string to_string(T value) {
        std::stringstream os;
        os << value;
        return os.str();
    }

    static void split_string(const std::string &s, std::vector<std::string> &v,
                             const std::string &c) {
        std::string::size_type pos1, pos2;
        pos2 = s.find(c);
        pos1 = 0;
        while (std::string::npos != pos2) {
            v.push_back(s.substr(pos1, pos2 - pos1));

            pos1 = pos2 + c.size();
            pos2 = s.find(c, pos1);
        }
        if (pos1 != s.length()) {
            v.push_back(s.substr(pos1));
        }
    }

    static std::string jstring_to_str(JNIEnv *env, jstring jstr) {
        char *rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring urlencode = env->NewStringUTF("UTF-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, urlencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);

        if (alen > 0) {
            rtn = (char *) malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        std::string stemp = (rtn == NULL) ? "" : std::string(rtn);
        free(rtn);
        return stemp;
    }

    static std::string string_vector_2_string(std::vector<std::string> v) {
        std::ostringstream oss;
        if (!v.empty()) {
            std::vector<std::string>::iterator it;
            for (it = v.begin(); it < v.end(); it++) {
                oss << it->c_str();
                if (it != v.end() - 1) {
                    oss << ", ";
                }
            }
        }
        return oss.str();
    }

    void throw_by_name(JNIEnv *env, const char *name, const char *msg) {
        // 查找异常类
        jclass cls = env->FindClass(name);
        /* 如果这个异常类没有找到，VM会抛出一个NowClassDefFoundError异常 */
        if (cls != NULL) {
            env->ThrowNew(cls, msg);  // 抛出指定名字的异常
        }
        /* 释放局部引用 */
        env->DeleteLocalRef(cls);
    }

    void throw_by_name(JNIEnv *env, const char *msg) {
        // 查找异常类
        jclass cls = env->FindClass("java/lang/Exception");
        /* 如果这个异常类没有找到，VM会抛出一个NowClassDefFoundError异常 */
        if (cls != NULL) {
            env->ThrowNew(cls, msg);  // 抛出指定名字的异常
        }
        /* 释放局部引用 */
        env->DeleteLocalRef(cls);
    }

}

#endif //APIKEYNATIVELIB_PWRD_HELPER_H
