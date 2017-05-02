#include <jni.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include "encrypt/urlEncode.h"

#ifndef APIKEYNATIVELIB_API_HELPER_H
#define APIKEYNATIVELIB_API_HELPER_H

int register_ndk_load(JNIEnv *env);

/**
 * request_body按照key排序后value拼接，格式如：key1=value1&key2=value2
 * MD5源数据的字符串后，拼接user_token、appkey、timestamp后md5
 */
JNIEXPORT static jstring JNICALL get_1Authorization(JNIEnv *env, jobject,
                                                    jstring request_body,
                                                    jstring user_token,
                                                    jlong timestamp);
/**
 * 原有数据源的字符串加上appkey后，使用MD5加密拼接好的字节数组
 */
JNIEXPORT static jstring JNICALL get_1JniEncryptBytes(JNIEnv *env,
                                                      jobject,
                                                      jstring sign);
/**
 * MD5源数据的字符串后，再拼接appkey后，再MD5，生成最终的字符串
 */
JNIEXPORT static jstring JNICALL get_1JniEncrptyBytesByMd5(JNIEnv *env,
                                                           jobject, jstring sign);



/**
 * 注册native方法到java中
 * @param env
 * @param className
 * @param gMethods
 * @param numMethods
 * @return
 */
int registerNativeMethods(JNIEnv *env, const char *className,
                          const JNINativeMethod *gMethods, int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }

    env->DeleteLocalRef(clazz);

    return JNI_TRUE;
}

namespace jack {

    class Api_Param {
    public:
        std::string to_string() {
            std::stringstream os;
            //os << key << "=" << value;
            os<<value;
            return os.str();
        }


    public:
        Api_Param(std::string _key, std::string _value) : key(_key), value(_value) {}

        std::string key, value;
    };

    /**
     * apiparam按照key的升序排序
     * @param param1
     * @param param2
     * @return
     */
    bool lessApiParam(Api_Param param1, Api_Param param2) {
        return std::strcmp(param1.key.c_str(), param2.key.c_str()) < 0;
    }

    /**
     * 把键值对拼接成字符串
     * @param v
     * @return
     */
    static std::string param_vector_2_string(std::vector<Api_Param> v) {
        std::ostringstream oss;
        if (!v.empty()) {
            std::vector<Api_Param>::iterator it;
            for (it = v.begin(); it < v.end(); it++) {
                oss << it->to_string().c_str();
                if (it != v.end() - 1) {
                    //oss << "&";
                }
            }
        }
        return oss.str();
    }

    /**
     * 返回键值对的个数
     * @param params_entry
     * @param param_vector
     * @return
     */
    static unsigned int string_vector_2_param_vector(std::vector<std::string> params_entry,
                                                     std::vector<Api_Param> &param_vector) {
        unsigned int size = params_entry.size();

        int i;
        for (i = 0; i < size; i++) {
            std::vector<std::string> entry_vector;
            split_string(params_entry[i], entry_vector, "=");   //按照等号来分隔key和value

            if (!entry_vector.empty()) {
                int entry_param_size = entry_vector.size();
                param_vector.push_back(Api_Param(UrlEncode(entry_vector[0]),
                                                 (entry_param_size > 1) ?
                                                 UrlEncode(entry_vector[1]) : ""));
            }
        }
        return size;
    }
}

#endif //APIKEYNATIVELIB_API_HELPER_H
