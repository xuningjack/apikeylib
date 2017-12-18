# apikeylib
Generating sign when access the Internet. (访问网络时签名sign的生成)   
包括按照map的key排序后value的拼接逻辑。  


目前提供以下几种签名生成的逻辑：  
1、request_body(格式如：key1=value1&key2=value2)按照key排序后value拼接。
MD5源数据的字符串后，拼接user_token、appkey、timestamp后md5。  
调用方法：  
SignHelper.getAuthorization(final String requestBody, final String userToken, final long timestamp)  

2、MD5源数据的字符串后，再拼接appkey后，再MD5，生成最终的字符串。  
调用方法：  
SignHelper.getJniEncrptyBytesByMd5(String sign)