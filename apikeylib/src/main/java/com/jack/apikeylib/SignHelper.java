package com.jack.apikeylib;

/**
 * 签名的工具类
 * @author Jack
 */
public final class SignHelper {

    private SignHelper() {
    }

    /**
     * request_body按照key排序后value拼接，格式如：key1=value1&key2=value2
     * MD5源数据的字符串后，拼接user_token、appkey、timestamp后md5
     * @param requestBody 请求参数string，格式如：key1=value1&key2=value2
     * @param userToken   用户token       若无用null
     * @param timestamp   请求失效时间戳   若无用0
     */
    public static String getAuthorization(final String requestBody, final String userToken, final long timestamp) {
        return get_Authorization(requestBody, userToken, timestamp);
    }

    /**
     * 原有sign加上appkey后，使用MD5加密拼接好的字节数组(在getSignBySort之后使用)
     * @param sign
     * @return
     */
    public static String getJniEncryptBytes(String sign) {
        return get_JniEncryptBytes(sign);
    }

    /**
     * 新的签名方式：MD5源数据的字符串后，再拼接appkey后，再MD5，生成最终的字符串
     * @param sign
     * @return
     */
    public static String getJniEncrptyBytesByMd5(String sign){
        return get_JniEncrptyBytesByMd5(sign);
    }


    /**
     * request_body按照key排序后value拼接，格式如：key1=value1&key2=value2
     * MD5源数据的字符串后，拼接user_token、appkey、timestamp后md5
     * @param requestBody
     * @param userToken
     * @param timestamp
     * @return
     */
    private static native String get_Authorization(String requestBody, String userToken, long timestamp);

    /**
     * 原有sign加上appkey后，使用MD5加密拼接好的字节数组(在getSignBySort之后使用)
     * @param sign
     * @return
     */
    private static native String get_JniEncryptBytes(String sign);

    /**
     * 新的签名方式：MD5源数据的字符串后，再拼接appkey后，再MD5，生成最终的字符串
     * @param sign
     * @return
     */
    private static native String get_JniEncrptyBytesByMd5(String sign);

    static {
        System.loadLibrary("api_helper");
    }
}
