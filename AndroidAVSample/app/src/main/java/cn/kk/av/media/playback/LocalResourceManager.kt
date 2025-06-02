package cn.kk.av.media.playback

import android.app.Activity
import android.content.Intent
import android.net.Uri
import cn.kk.av.PlayerApplication
import cn.kk.av.model.LocalFileMeta
import cn.kk.av.util.MyResourceUtil
import cn.kk.av.util.UIHelper

object LocalResourceManager {
    val LOCAL_RESOURCE_NOT_FOUND = "local_resource_not_found"
    suspend fun prepareMedia(source: Activity, fileUri: Uri): LocalFileMeta {

        // 持久Uri 权限
        val contentResolver = PlayerApplication.appContext.contentResolver
        contentResolver.takePersistableUriPermission(fileUri, Intent.FLAG_GRANT_READ_URI_PERMISSION)

        val fileMetaInfo = MyResourceUtil.getFileMetaInfo(fileUri)

        if (!fileMetaInfo.support()) {
            UIHelper.toastMsg(source, "不支持该类型文件")
            return fileMetaInfo
        }
        fileMetaInfo.support = true

//        val metaInfo = Gson().toJson(fileMetaInfo)
//        val articleUUID = MyResourceUtil.generateLocalResourceArticleUUID(fileMetaInfo.fileName, fileMetaInfo.fileSize, fileMetaInfo.videoType)
        return fileMetaInfo
    }
}