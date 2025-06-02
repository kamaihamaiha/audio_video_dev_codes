package cn.kk.av.media.playback

import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.view.SurfaceHolder
import androidx.annotation.OptIn
import androidx.media3.common.C
import androidx.media3.common.ForwardingPlayer
import androidx.media3.common.MediaItem
import androidx.media3.common.PlaybackException
import androidx.media3.common.PlaybackParameters
import androidx.media3.common.Player
import androidx.media3.common.util.UnstableApi
import androidx.media3.common.util.Util
import androidx.media3.datasource.DataSource
import androidx.media3.datasource.DefaultDataSourceFactory
import androidx.media3.datasource.DefaultHttpDataSource
import androidx.media3.datasource.cache.CacheDataSource
import androidx.media3.datasource.cache.LeastRecentlyUsedCacheEvictor
import androidx.media3.datasource.cache.SimpleCache
import androidx.media3.datasource.okhttp.OkHttpDataSource
import androidx.media3.exoplayer.DefaultLoadControl
import androidx.media3.exoplayer.DefaultRenderersFactory
import androidx.media3.exoplayer.ExoPlayer
import androidx.media3.exoplayer.PlayerMessage
import androidx.media3.exoplayer.dash.DashMediaSource
import androidx.media3.exoplayer.hls.HlsMediaSource
import androidx.media3.exoplayer.smoothstreaming.SsMediaSource
import androidx.media3.exoplayer.source.DefaultMediaSourceFactory
import androidx.media3.exoplayer.source.MediaSource
import androidx.media3.exoplayer.source.ProgressiveMediaSource
import androidx.media3.exoplayer.trackselection.DefaultTrackSelector
import androidx.media3.exoplayer.upstream.DefaultAllocator
import androidx.media3.exoplayer.upstream.DefaultBandwidthMeter
import androidx.media3.session.MediaSession
import cn.kk.av.PlayerApplication
import okhttp3.CacheControl
import okhttp3.Call
import okhttp3.OkHttpClient
import okhttp3.Request
import java.io.File
import java.security.SecureRandom
import java.security.cert.CertificateException
import java.security.cert.X509Certificate
import javax.net.ssl.HostnameVerifier
import javax.net.ssl.SSLContext
import javax.net.ssl.TrustManager
import javax.net.ssl.X509TrustManager

@UnstableApi
class KKMedia3Player(val ctx: Context): IPlayer, Player.Listener {
    val MAX_CACHE_SIZE = 100 * 1024 * 1024L
    val EXO_MEDIA3_CACHE_DIR: String = "exo_media3_cache"
    lateinit var mPlayer: ExoPlayer
    private var dataSource = arrayOf("", "")
    // 用户播放器由于资源被释放恢复播放进度
    private var resumePosition = C.TIME_UNSET
    private var rePrepareState = false

    private val dataSourceFactory: DataSource.Factory = DefaultHttpDataSource.Factory()
    val mSimpleCache: SimpleCache = SimpleCache(File(PlayerApplication.appContext.cacheDir,
        EXO_MEDIA3_CACHE_DIR), LeastRecentlyUsedCacheEvictor(MAX_CACHE_SIZE), PlayerApplication.media3CacheStandaloneDatabaseProvider)
    private val mDefaultCacheMediaSourceFactory by lazy {
        ProgressiveMediaSource.Factory(CacheDataSource.Factory()
            .setCache(mSimpleCache)
            .setUpstreamDataSourceFactory(DefaultHttpDataSource.Factory().setAllowCrossProtocolRedirects(true)))
        }
    val eventListeners = mutableListOf<EventListener>()
    init {
        initPlayer()
    }

    private fun initPlayer() {
        val loadControl = DefaultLoadControl.Builder()
            .apply {
                setAllocator(DefaultAllocator(true, C.DEFAULT_BUFFER_SEGMENT_SIZE))
                setBufferDurationsMs(
                    DefaultLoadControl.DEFAULT_MIN_BUFFER_MS, DefaultLoadControl.DEFAULT_MAX_BUFFER_MS * 2 * 2,
                    DefaultLoadControl.DEFAULT_BUFFER_FOR_PLAYBACK_MS, DefaultLoadControl.DEFAULT_BUFFER_FOR_PLAYBACK_AFTER_REBUFFER_MS
                )
                setBackBuffer(DefaultLoadControl.DEFAULT_MAX_BUFFER_MS, true)
            }
            .build()

        val renderersFactory = DefaultRenderersFactory(ctx)

        val playerBuilder = ExoPlayer.Builder(ctx, renderersFactory)


        playerBuilder.apply {
            setTrackSelector(DefaultTrackSelector(ctx))
            setLoadControl(loadControl)
            setMediaSourceFactory(mDefaultCacheMediaSourceFactory)
        }
        mPlayer = playerBuilder.build()
        mPlayer.addListener(this)

    }

    val trustAllCerts = arrayOf<TrustManager>(
        object : X509TrustManager {
            @Throws(CertificateException::class)
            override fun checkClientTrusted(chain: Array<X509Certificate>, authType: String) {
            }

            @Throws(CertificateException::class)
            override fun checkServerTrusted(chain: Array<X509Certificate>, authType: String) {
            }

            override fun getAcceptedIssuers(): Array<X509Certificate> {
                return arrayOf()
            }
        }
    )

    @OptIn(UnstableApi::class)   fun getDefaultMediaSourceFactory(): DefaultMediaSourceFactory {
        val factory = buildDataSourceFactory()
        if (factory != null) return DefaultMediaSourceFactory(factory)
        return DefaultMediaSourceFactory(ctx)
    }
    @OptIn(UnstableApi::class)  private fun buildDataSourceFactory(): DataSource.Factory? {
        try {
            // Install the all-trusting trust manager
            val sslContext = SSLContext.getInstance("SSL")
            sslContext.init(null, trustAllCerts, SecureRandom())
            // Create an ssl socket factory with our all-trusting manager
            val sslSocketFactory = sslContext.socketFactory
            val builder: OkHttpClient.Builder = OkHttpClient.Builder()
            if (!trustAllCerts.isNullOrEmpty()) {
                builder.sslSocketFactory(sslSocketFactory, trustAllCerts.get(0) as X509TrustManager)
            }
            builder.hostnameVerifier(HostnameVerifier { hostname, session -> true })
            val client: OkHttpClient = builder.build()

            val factory = object : Call.Factory {
                override fun newCall(request: Request): Call {
                    return client.newCall(request)
                }

            }
            //不使用okhttp缓存
            // EuDictHttpApi.ShareDictInstance().userAgent, null, CacheControl.FORCE_NETWORK
            // .setUserAgent(EuDictHttpApi.ShareDictInstance().userAgent)
            val okHttpDataSourceFactory = OkHttpDataSource.Factory(factory)
                .setCacheControl(null)
                .setCacheControl(CacheControl.FORCE_NETWORK)
            return DefaultDataSourceFactory(ctx, DefaultBandwidthMeter.Builder(ctx).build() ,okHttpDataSourceFactory)
        } catch (ex: Exception) {
            ex.printStackTrace()
        }
        return null
    }

    fun addListener(listener: EventListener){
        eventListeners.add(listener)
    }

    fun removeListener(listener: EventListener){
        eventListeners.remove(listener)
    }
    @OptIn(UnstableApi::class) private fun createMediaSource(url: String, type: String): MediaSource {
        val suffixType = type.replace(".","") // .mp4 -> mp4, inferContentTypeForExtension 方法参数要求
        val contentType = if (suffixType.isEmpty()) Util.inferContentType(Uri.parse(url)) else Util.inferContentTypeForExtension(suffixType)
        val mediaItem = MediaItem.fromUri(url)
        val mediaSource = when(contentType) {
            C.CONTENT_TYPE_DASH -> DashMediaSource.Factory(dataSourceFactory).createMediaSource(mediaItem)
            C.CONTENT_TYPE_SS -> SsMediaSource.Factory(dataSourceFactory).createMediaSource(mediaItem)
            C.CONTENT_TYPE_HLS -> HlsMediaSource.Factory(buildDataSourceFactory()?:dataSourceFactory).createMediaSource(mediaItem)
            C.CONTENT_TYPE_OTHER -> {
                if (".local".equals(type) || ".offline".equals(type)) {
                    getDefaultMediaSourceFactory().createMediaSource(mediaItem)
                } else {
                    mDefaultCacheMediaSourceFactory.createMediaSource(mediaItem)
                }
            }
            else -> throw IllegalArgumentException("Unsupported type: ${type}")
        }
        return mediaSource
    }

    @OptIn(UnstableApi::class) fun initMediaSession(activityCls: Class<*>): MediaSession{
        val mediaSession = MediaSession.Builder(ctx, object: ForwardingPlayer(mPlayer) {
            override fun getAvailableCommands(): Player.Commands {
                var commands = super.getAvailableCommands()
                if (!commands.contains(Player.COMMAND_SEEK_TO_NEXT)) commands = commands.buildUpon().add(Player.COMMAND_SEEK_TO_NEXT).build()
                if (!commands.contains(Player.COMMAND_SEEK_TO_NEXT_MEDIA_ITEM)) commands = commands.buildUpon().add(Player.COMMAND_SEEK_TO_NEXT_MEDIA_ITEM).build()
                return commands
            }
            override fun play() {
                for (eventListener in eventListeners) {
                    eventListener.controlPlay()
                }
            }

            override fun setPlayWhenReady(playWhenReady: Boolean) {
                super.setPlayWhenReady(playWhenReady)
            }

            override fun pause() {
                for (eventListener in eventListeners) {
                    eventListener.controlPause()
                }
            }

            override fun seekToPrevious() {
                for (eventListener in eventListeners) {
                    eventListener.controlPrevious()
                }
            }

            override fun seekToNext() {
                for (eventListener in eventListeners) {
                    eventListener.controlNext()
                }
            }

            override fun seekToNextMediaItem() {
                for (eventListener in eventListeners) {
                    eventListener.controlNext()
                }
            }
        }).build()
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            mediaSession.setSessionActivity(PendingIntent.getActivity(ctx, 0, Intent(ctx, activityCls), PendingIntent.FLAG_MUTABLE))
        } else {
            mediaSession.setSessionActivity(PendingIntent.getActivity(ctx, 0, Intent(ctx, activityCls), 0))
        }
        return mediaSession
    }

    // region deal player operation

    @OptIn(UnstableApi::class) private fun dealPrepare() {
        val mediaSources = mutableListOf<MediaSource>()
        val mediaSource = createMediaSource(dataSource[0], dataSource[1])
//        val simulateNexSource = mediaSource
        mediaSources.add( mediaSource)
//        mediaSources.add(simulateNexSource) // 使得 控制中心的 next 按钮点击可用

        if (mPlayer.availableCommands.contains(Player.COMMAND_CHANGE_MEDIA_ITEMS)) {
            mPlayer.clearMediaItems()
        }
        mPlayer.addMediaSources(mediaSources)
        mPlayer.prepare()
        rePrepareState = false
    }
    private fun dealStart() {
        if (mPlayer.playbackState == ExoPlayer.STATE_IDLE) {
            rePrepareState = true
            dealPrepare()
        }
        mPlayer.playWhenReady = true
    }

    private fun dealPause(){
        mPlayer.playWhenReady = false
    }
    private fun dealStop(){
        mPlayer.stop()
    }

    private fun dealReset(){
        mPlayer.stop()
    }

    @OptIn(UnstableApi::class) private fun dealRelease(){
        mPlayer.release()
        mSimpleCache.release()
    }

    override fun onPositionDiscontinuity(oldPosition: Player.PositionInfo, newPosition: Player.PositionInfo, reason: Int) {
        super.onPositionDiscontinuity(oldPosition, newPosition, reason)
        if (rePrepareState) {
            resumePosition = oldPosition.positionMs
        }
    }

    private fun dealCanSetSpeed(): Boolean {
        return true
    }
    private fun dealGetCurrentPitchStepsAdjustment() = 0f

    private fun dealGetCurrentPosition(): Int {
        return mPlayer.currentPosition.toInt()
    }

    private fun dealGetBufferedPosition(): Int {
        return mPlayer.bufferedPosition.toInt()
    }

    private fun dealGetDuration(): Int {
        return mPlayer.duration.toInt()
    }

    private fun dealIsPlaying(): Boolean {
        return mPlayer.playWhenReady
    }

    private fun dealSeekTo(msec: Int){
        mPlayer.seekTo(msec.toLong())
    }

    private fun dealGetDataSource(): String {
        return dataSource?.get(0) ?: ""
    }

    private fun dealSetDisplay(sh: SurfaceHolder?){
        mPlayer.setVideoSurfaceHolder(sh)
    }

    fun getResumePosition(): Long {
        return resumePosition
    }

    fun clearResumePosition(){
        resumePosition = C.TIME_UNSET
    }

    private var playSpeed = 1f
    private var defaultPlaySpeed = 1f
    private var playbackPitch = 1f
    private fun setPlaybackParameters(){
        mPlayer.playbackParameters = PlaybackParameters(playSpeed, playbackPitch)
    }

    private fun dealSetVolume(left: Float){
        mPlayer.volume = left
    }

    private fun dealGetVolume(): Float = mPlayer.volume

    @OptIn(UnstableApi::class) private fun dealGetVideoWidth(): Int = mPlayer.videoFormat?.width ?: 0

    @OptIn(UnstableApi::class) private fun dealGetVideoHeight(): Int = mPlayer.videoFormat?.height ?: 0
    // endregion

    // region IPlayer implementation
    override fun canSetSpeed(): Boolean = dealCanSetSpeed()

    override fun getCurrentPitchStepsAdjustment(): Float = dealGetCurrentPitchStepsAdjustment()

    override fun getCurrentPosition(): Int = dealGetCurrentPosition()

    override fun getBufferedPosition(): Int = dealGetBufferedPosition()

    override fun getDuration(): Int = dealGetDuration()

    override fun isPlaying(): Boolean = dealIsPlaying()

    override fun pause() {
        dealPause()
    }

    override fun prepare() {
        dealPrepare()
    }

    override fun prepareAsync() {
        dealPrepare()
    }

    override fun release() {
        dealRelease()
    }

    override fun reset() {
        dealReset()
    }

    override fun seekTo(msec: Int) {
        dealSeekTo(msec)
    }

    override fun setAudioStreamType(streamtype: Int) {
    }

    override fun setScreenOnWhilePlaying(screenOn: Boolean) {
    }

    override fun setDataSource(path: Array<String>) {
        dataSource = path
        clearResumePosition()
    }

    override fun getDataSource(): String = dealGetDataSource()

    override fun setDisplay(sh: SurfaceHolder?) {
        dealSetDisplay(sh)
    }

    override fun setLooping(looping: Boolean) {
    }

    override fun setPlaybackPitch(f: Float) {
        playbackPitch = f
        setPlaybackParameters()
    }

    override fun setPlaybackSpeed(f: Float, updateDefault: Boolean) {
        playSpeed = f
        if (updateDefault) {
            defaultPlaySpeed = f
        }
        setPlaybackParameters()
    }

    override fun getPlaybackSpeed(): Float = playSpeed

    override fun resetPlaybackSpeed() {
        playSpeed = defaultPlaySpeed
        setPlaybackParameters()
    }

    override fun setVolume(left: Float, right: Float) {
        dealSetVolume(left)
    }

    override fun getVolume(): Float = dealGetVolume()

    override fun onPlayerError(error: PlaybackException) {
        for (eventListener in eventListeners) {
            eventListener.playerError(error)
        }
    }

    @OptIn(UnstableApi::class)  override fun onPlayerStateChanged(playWhenReady: Boolean, playbackState: Int) {
        for (eventListener in eventListeners) {
            eventListener.playerStateChanged(playWhenReady, playbackState)
        }
    }

    @OptIn(UnstableApi::class) override fun start() {
        dealStart()
    }

    override fun stop() {
        dealStop()
    }

    override fun getVideoWidth(): Int = dealGetVideoWidth()

    override fun getVideoHeight(): Int = dealGetVideoHeight()

    @OptIn(UnstableApi::class) override fun <T : Any?> setCallbackAtPosition(positionMs: Long, payload: Any?, target: T) {
        if (target is PlayerMessage.Target) {
            mPlayer.createMessage(target)
                .setPosition(/* mediaItemIndex= */ 0, /* positionMs= */ positionMs)
                .setDeleteAfterDelivery(true)
                .setPayload(payload)
                .send()
        }

    }


    // endregion

    interface EventListener {
        fun playerStateChanged(playWhenReady: Boolean, playbackState: Int)

        fun playerError(error: PlaybackException)

        fun controlPlay()
        fun controlPause()
        fun controlPrevious()
        fun controlNext()
    }
}