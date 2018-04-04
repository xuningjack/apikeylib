#include <jni.h>
#include <vector>
#include <algorithm>
#include "string_helper.h"
#include "api_helper.h"
#include "encrypt/md5.cpp"

/**
 * 为了扩展性，使用动态注册的方式来加载类，如果需要改动绑定的java类和方法只需要改动 JNI_REG_CLASS 和 JNI_REG_METHOD
 */
// 指定要注册的类和方法
#define JNI_REG_CLASS "com/jack/apikeylib/SignHelper"
/**签名时需要用的appkey*/
const char *appKeyChars = "123456";
/**是否显示log*/
const bool SHOW_LOG = true;

using namespace std;
using namespace jack;

/**
 * Java和JNI函数的绑定表
 */
static const JNINativeMethod method_table[] = {
        {"get_Authorization", "(Ljava/lang/String;Ljava/lang/String;J)Ljava/lang/String;", (void *) get_1Authorization},
        {"get_JniEncryptBytes", "(Ljava/lang/String;)Ljava/lang/String;", (void *) get_1JniEncryptBytes},
        {"get_JniEncrptyBytesByMd5", "(Ljava/lang/String;)Ljava/lang/String;", (void *) get_1JniEncrptyBytesByMd5}
};


/**
 * request_body按照key排序后value拼接，格式如：key1=value1&key2=value2
 * MD5源数据的字符串后，拼接user_token、appkey、timestamp后md5
 * @return 返回签名字符串为小写字母
 */
JNIEXPORT static jstring JNICALL get_1Authorization(JNIEnv *env, jobject,
                                                    jstring request_body,
                                                    jstring user_token,
                                                    jlong timestamp) {
    vector<string> string_param_vector;
    stringstream ss;
    if(request_body != NULL){
        std::string request_body_str = jstring_to_str(env, request_body);
        // 根据&拆分为请求参数param
        split_string(request_body_str, string_param_vector, "&");
        vector<Api_Param> param_vector;
        // 将param排序
        string_vector_2_param_vector(string_param_vector, param_vector);
        sort(param_vector.begin(), param_vector.end(), lessApiParam);
        if(SHOW_LOG) {
            LOGI("=======jni-api-params-sorted==========\r\n%s",
                 param_vector_2_string(param_vector).c_str());
        }
        // 将requestBody,user_token,api_key,timestamp 依次拼接起来
        ss << param_vector_2_string(param_vector);
    }
    if(SHOW_LOG){
        LOGI("=======temp before==========\r\n%s", ss.str().c_str());
    }
    //参数列表拼接md5后
    string temp = MD5(ss.str()).toStr();
    if(SHOW_LOG){
        LOGI("=======temp==========\r\n%s", temp.c_str());
        LOGI("=======ss.str() clear before==========\r\n%s", ss.str().c_str());
    }
    ss.str("");  //todo清空字符串缓冲区
    ss << temp;   //拼接md5原先的value数据源
    if(user_token != NULL){   //再拼接usertoken
        ss << jstring_to_str(env, user_token);
    }
    ss << appKeyChars;   //再拼接appkey
    if(timestamp != 0){   //在拼接timestamp
        ss << (long) timestamp;
    }
    if(SHOW_LOG){
        LOGI("=======ss.str() final==========\r\n%s", ss.str().c_str());
    }
    //返回拼接结果md5值+user_token+api_key+timestamp的md5
    return env->NewStringUTF(MD5(ss.str()).toStr().c_str());
}


/**
 * 原有数据源的字符串加上appkey后，使用MD5加密拼接好的字节数组
 */
JNIEXPORT static jstring JNICALL get_1JniEncryptBytes(JNIEnv *env, jobject, jstring sign) {
    stringstream ss;
    ss << jstring_to_str(env, sign);
    if (appKeyChars != NULL) {
        ss << appKeyChars;
    }
    return env->NewStringUTF(MD5(ss.str()).toStr().c_str());
}


/**
 * MD5源数据的字符串后，再拼接appkey后，再MD5，生成最终的字符串
 */
JNIEXPORT static jstring JNICALL get_1JniEncrptyBytesByMd5(JNIEnv *env, jobject, jstring sign) {
    stringstream ss;
    if (sign != NULL) {   //MD5源数据的字符串后
        string signString = jstring_to_str(env, sign);
        ss << jstring_to_str(env, env->NewStringUTF(MD5(signString).toStr().c_str()));
    }
    if (appKeyChars != NULL) {   //再拼接appkey后
        ss << appKeyChars;
    }
    return env->NewStringUTF(MD5(ss.str()).toStr().c_str());   //再MD5，生成最终的字符串
}


int register_ndk_load(JNIEnv *env) {
    // 调用注册方法
    return registerNativeMethods(env, JNI_REG_CLASS, method_table, NELEM(method_table));
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    // Get jclass with env->FindClass.
    // Register methods with env->RegisterNatives.
    register_ndk_load(env);
    return JNI_VERSION_1_6;
}



