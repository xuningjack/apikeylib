package cn.jack.apikeynativelib;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.jack.apikeylib.SignHelper;



public class MainActivity extends Activity {

    public static final String APP_KEY = "123456";

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String resource = "源字符串：a=1&b=2&c=3&d=4";

        final TextView tv = (TextView) findViewById(R.id.sample_text);
        try {
            final StringBuilder sb = new StringBuilder(128);
            sb.append("origin :").append(resource).append("\r\n");

            sb.append("SignHelper c++(按照key排序后value拼接，格式如：" +
                    "key1=value1&key2=value2MD5源数据的字符串后，" +
                    "拼接user_token、appkey、timestamp后md5) :" +
                    SignHelper.getAuthorization(resource, null, 0)).append("\r\n");

            String javaTemp = EncryptUtil.md5("1234") + APP_KEY;
            Log.d("jack", " =======temp==========" + javaTemp);
            sb.append("java MD5字符串结果: ").append(EncryptUtil.md5(javaTemp)).append("\r\n");
            tv.setText(sb.toString());
        } catch (Exception e) {
            Log.e("JNI error", Log.getStackTraceString(e));
            tv.setText("JNI error");
        }

        TextView getJniEncryptBytes = (TextView)findViewById(R.id.getJniEncryptBytes);
        String sign = SignHelper.getJniEncryptBytes(resource);
        getJniEncryptBytes.setText("原有sign加上appkey后，" +
                "使用MD5加密拼接好的字节数组(在getSignBySort之后使用) getJniEncryptBytes: " + sign);


        TextView getJniEncrptyBytesByMd5 = (TextView)findViewById(R.id.getJniEncrptyBytesByMd5);
        sign = SignHelper.getJniEncrptyBytesByMd5(resource);
        getJniEncrptyBytesByMd5.setText("MD5源数据的字符串后，再拼接appkey后，再MD5，" +
                "生成最终的字符串getJniEncrptyBytesByMd5：" + sign);


    }

}
