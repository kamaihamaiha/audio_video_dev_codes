package cn.kk.av.media.playback

import android.view.SurfaceHolder
import java.io.IOException

interface IPlayer {

    fun canSetSpeed(): Boolean

    fun getCurrentPitchStepsAdjustment(): Float

    fun getCurrentPosition(): Int

    fun getBufferedPosition(): Int

    fun getDuration(): Int

    fun isPlaying(): Boolean

    fun pause()

    @Throws(IllegalStateException::class, IOException::class)
    fun prepare()

    fun prepareAsync()

    fun release()

    fun reset()

    fun seekTo(msec: Int)

    fun setAudioStreamType(streamtype: Int)

    fun setScreenOnWhilePlaying(screenOn: Boolean)

    //[uri, media format]
    @Throws(
        IllegalStateException::class,
        IOException::class,
        IllegalArgumentException::class,
        SecurityException::class
    )
    fun setDataSource(path: Array<String>)

    //获取当前正在播放的url 方便调试
    fun getDataSource(): String?

    fun setDisplay(sh: SurfaceHolder?)

    fun setLooping(looping: Boolean)

    fun setPlaybackPitch(f: Float)

    fun setPlaybackSpeed(f: Float, updateDefault: Boolean)

    fun getPlaybackSpeed(): Float

    fun resetPlaybackSpeed()

    fun setVolume(left: Float, right: Float)

    fun getVolume(): Float

    fun start()

    fun stop()

    fun getVideoWidth(): Int

    fun getVideoHeight(): Int

    fun <T> setCallbackAtPosition(positionMs: Long, payload: Any?, target: T)

}