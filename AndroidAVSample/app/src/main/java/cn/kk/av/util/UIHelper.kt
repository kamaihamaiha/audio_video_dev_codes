package cn.kk.av.util

import android.app.Activity
import android.content.Context
import android.graphics.Color
import android.os.Looper
import android.text.TextUtils
import android.view.View
import android.widget.Toast

object UIHelper {

    /**
     * 设置状态栏透明，且独自占用空间
     */
    fun setStatusBarTrans(activity: Activity){
        // 表示让应用主题内容占据系统状态栏的空间
        val option = View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN or View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            activity.window.decorView.systemUiVisibility = option

        // 状态栏透明
        activity.window.statusBarColor = Color.TRANSPARENT
    }

    // 白天状态栏；也就是字体图标为黑色
    fun setLightStatusBar(context: Activity) {
        var flags = context.window.decorView.systemUiVisibility
        flags = flags or View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR
        context.window.decorView.systemUiVisibility = flags
    }

    fun setDarkStatusBar(context: Activity) {
        var flags = context.window.decorView.systemUiVisibility
        flags = flags and View.SYSTEM_UI_FLAG_LIGHT_STATUS_BAR.inv()
        context.window.decorView.systemUiVisibility = flags
    }

    //endregion
    //endregion
    //region Toast & Dialog
    fun toastMsg(context: Context?, msg: String?) {
        if (context == null || msg == null || msg.isEmpty()) return
        showToast(context, msg, Toast.LENGTH_SHORT)
    }

    fun toastMsg(context: Context?, Res: Int) {
        if (context == null) return
        toastMsg(context, context.getString(Res))
    }

    fun showToast(context: Context?, text: CharSequence?, duration: Int) {
        if (context == null || TextUtils.isEmpty(text)) return
        if (Looper.myLooper() == Looper.getMainLooper()) {
            Toast.makeText(context, text, duration).show()
        } else {
            ThreadUtil.INST.executeInMainThread { Toast.makeText(context, text, duration).show() }
        }
    }
}