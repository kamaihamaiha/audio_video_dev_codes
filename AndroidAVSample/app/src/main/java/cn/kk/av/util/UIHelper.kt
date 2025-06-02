package cn.kk.av.util

import android.app.Activity
import android.view.View

object UIHelper {

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
}