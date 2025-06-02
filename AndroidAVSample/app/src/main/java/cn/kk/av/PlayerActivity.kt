package cn.kk.av

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import cn.kk.av.databinding.PlayerActivityBinding
import cn.kk.av.media.playback.IPlayerListener
import cn.kk.av.media.playback.LocalResourceManager
import cn.kk.av.media.playback.PlayerController
import cn.kk.av.util.CommonUtil
import cn.kk.av.util.UIHelper
import kotlinx.coroutines.launch
import java.lang.Exception

class PlayerActivity: AppCompatActivity() {
    val REQUEST_VIDEO_CODE = 100
    lateinit var mBind: PlayerActivityBinding
    lateinit var playerController: PlayerController
    val playerListener by lazy {
        object : IPlayerListener {
            override fun genericOnInfo(inObj: Any?, what: Int, extra: Int): Boolean {
                return false
            }

            override fun genericOnError(inObj: Any?, what: Int, extra: Int): Boolean {
                UIHelper.toastMsg(this@PlayerActivity, "error:")
                return true
            }

            override fun genericOnCompletion() {
            }

            override fun genericOnBufferingUpdate(percent: Int) {
            }

            override fun genericOnSeek() {
            }

            override fun genericOnPrepared() {
                playerController.mediaPlayer.start()
            }

            override fun onInternalError(ex: Exception?) {
                UIHelper.toastMsg(this@PlayerActivity, ex?.message)
            }

        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        mBind = PlayerActivityBinding.inflate(layoutInflater)
        setContentView(mBind.root)

        playerController = PlayerController(applicationContext, playerListener)
        playerController.createMedia3Player()

        mBind.btnSelectFile.setOnClickListener {
            CommonUtil.chooseFile(this, REQUEST_VIDEO_CODE)
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (data != null && data.data != null && requestCode == REQUEST_VIDEO_CODE) {
            lifecycleScope.launch {
                UIHelper.toastMsg(this@PlayerActivity, "loading...")
               val selectFile =  LocalResourceManager.prepareMedia(this@PlayerActivity, data.data!!)

                if (!selectFile.support) {
                    return@launch
                }

                mBind.tvCurFile.text = selectFile.fileName.plus("(${selectFile.path})")

                playerController.mediaPlayer.setDataSource(arrayOf(selectFile.path, ".local"))
                playerController.mediaPlayer.prepare()
            }

        }
    }

    override fun onResume() {
        super.onResume()

    }
    override fun onPause() {
        super.onPause()
        if (playerController.isPlaying) playerController.stop()
    }

    override fun onDestroy() {
        super.onDestroy()
        playerController.resetMediaPlayer()
    }
}