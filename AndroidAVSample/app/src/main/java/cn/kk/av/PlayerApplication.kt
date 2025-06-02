package cn.kk.av

import android.app.Application
import android.content.Context
import androidx.media3.common.util.UnstableApi
import androidx.media3.database.DatabaseProvider
import androidx.media3.database.StandaloneDatabaseProvider

@UnstableApi
class PlayerApplication: Application() {
    companion object {
        lateinit var media3CacheStandaloneDatabaseProvider: DatabaseProvider
        lateinit var appContext: Context
    }
    override fun onCreate() {
        super.onCreate()
        appContext = applicationContext
        media3CacheStandaloneDatabaseProvider = StandaloneDatabaseProvider(appContext)
    }
}