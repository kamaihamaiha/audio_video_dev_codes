package cn.kk.av.ui

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import cn.kk.av.util.UIHelper

open class BaseActivity: AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        UIHelper.setStatusBarTrans(this)
        UIHelper.setLightStatusBar(this)
    }
}