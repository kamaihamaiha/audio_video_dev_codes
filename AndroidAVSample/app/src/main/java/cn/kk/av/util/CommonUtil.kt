package cn.kk.av.util

import android.app.Activity
import android.content.Intent
import java.security.MessageDigest
import java.security.NoSuchAlgorithmException

object CommonUtil {

    fun chooseFile(activity: Activity, requestCode: Int) {
        val intent = Intent(Intent.ACTION_OPEN_DOCUMENT)
        intent.addCategory(Intent.CATEGORY_OPENABLE)
        intent.setType("*/*")
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION or Intent.FLAG_GRANT_PERSISTABLE_URI_PERMISSION)
        activity.startActivityForResult(intent, requestCode)
    }


    fun md5(s: String): String {
        try {
            // Create MD5 Hash
            val digest = MessageDigest.getInstance("MD5")
            digest.update(s.toByteArray())
            val messageDigest = digest.digest()

            // Create Hex String
            val hexString = StringBuffer()
            for (i in messageDigest.indices) {
                var thisStr = Integer.toHexString(0xFF and messageDigest[i].toInt())
                if (thisStr.length == 1) thisStr = "0$thisStr"
                hexString.append(thisStr)
            }
            return hexString.toString()
        } catch (e: NoSuchAlgorithmException) {
            e.printStackTrace()
        }
        return ""
    }
}