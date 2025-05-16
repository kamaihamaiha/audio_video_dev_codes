package cn.kk.av

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import cn.kk.av.databinding.PlayerActivityBinding

class PlayerActivity: AppCompatActivity() {
    lateinit var mBind: PlayerActivityBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = PlayerActivityBinding.inflate(layoutInflater)
        setContentView(mBind.root)


    }
}