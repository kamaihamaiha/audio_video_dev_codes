package cn.kk.av.util

import android.media.MediaMetadataRetriever
import android.net.Uri
import android.provider.MediaStore
import android.provider.OpenableColumns
import android.text.TextUtils
import cn.kk.av.PlayerApplication
import cn.kk.av.model.LocalFileMeta
import kotlinx.coroutines.suspendCancellableCoroutine
import java.io.FileNotFoundException
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.runBlocking

object MyResourceUtil {

    // 不同类型的资源的uuid前缀
    val NETDISK_BAIDU_PLAY_LIST_TYPE_PREFIX = "bdbd0000-"
    val NETDISK_BAIDU_VIDEO_TYPE_PREFIX = "bdbd0101-"
    val NETDISK_BAIDU_AUDIO_TYPE_PREFIX = "bdbd0202-"
    val LOCAL_VIDEO_TYPE_PREFIX = "ff000103-"
    val LOCAL_AUDIO_TYPE_PREFIX = "ff000104-"
    val SUPPORT_MEDIA_FORMATS = arrayOf("mp4", "m4a", "mkv", "webm", "mp3", "ogg", "wav", "aac")
    fun getFileHumanName(fileName: String): String {
        // server_filename hello.mkv -> hello
        if (TextUtils.isEmpty(fileName)) return fileName
        val pointIndex: Int = fileName.lastIndexOf('.')
        return if (pointIndex == -1) fileName else fileName.substring(0, pointIndex)
    }

    fun getHumanSize(size: Long): String {
        val kb: Float = size / 1024f
        val mb = kb / 1024f
        val gb = mb / 1024f
        val tb = gb / 1024f
        val threadhold = 1.00f
        var hmSize = ""
        hmSize = if (tb >= threadhold) {
            String.format("%.2f TB", tb)
        } else if (gb >= threadhold) {
            String.format("%.2f GB", gb)
        } else if (mb >= threadhold) {
            String.format("%.2f MB", mb)
        } else if (kb >= threadhold) {
            String.format("%.2f KB", kb)
        } else {
            String.format("%d B", size)
        }
        return hmSize
    }


    fun generateLocalResourceArticleUUID(fileName: String, fileByteSize: Long, videoType: Boolean): String {
        val prefix = if (videoType) LOCAL_VIDEO_TYPE_PREFIX else LOCAL_AUDIO_TYPE_PREFIX
        val fileInfo = fileName + "_" + fileByteSize.toString() // 用来生成md5 使用的字符串，fileName 包含文件扩展名
        // get fileInfo md5
         val fileInfoMd5 = CommonUtil.md5(fileInfo)
        val padded = fileInfoMd5.substring(Math.max(0, fileInfoMd5.length - 24))
        val suffix = padded.substring(0, 4) + "-" + padded.substring(4, 8) + "-" + padded.substring(8, 12) + "-" + padded.substring(12)
        return prefix + suffix
    }


    fun isMyResourceArticle(uuid: String): Boolean{
        if (uuid.startsWith(NETDISK_BAIDU_VIDEO_TYPE_PREFIX) || uuid.startsWith(NETDISK_BAIDU_AUDIO_TYPE_PREFIX) || isLocalResourceArticle(uuid)) {
            return true
        }
        return false
    }
    fun isLocalResourceArticle(uuid: String): Boolean{
        if (uuid.startsWith(LOCAL_VIDEO_TYPE_PREFIX) || uuid.startsWith(LOCAL_AUDIO_TYPE_PREFIX)) return true
        return false
    }

    fun isLocalResourceVideoArticle(uuid: String): Boolean{
        if (uuid.startsWith(LOCAL_VIDEO_TYPE_PREFIX)) return true
        return false
    }

    fun isFileExists(uri: Uri): Boolean {
        try {
           val openFileDescriptor = PlayerApplication.appContext.contentResolver.openFileDescriptor(uri, "r") ?: return false
        } catch (e: FileNotFoundException) {
            e.printStackTrace()
            return false
        } catch (e: SecurityException) {
            e.printStackTrace()
            return false
        }
        return true
    }


    /**
     * 耗时操作
     */
    suspend fun getFileMetaInfo(contentUri: Uri): LocalFileMeta = suspendCancellableCoroutine { continuation ->
        val localMediaFile = LocalFileMeta()
        try { // 选择 rmvb 会得到这种 application/octet-stream
            val mimeType: String? = contentUri.let { returnUri ->
                PlayerApplication.appContext.contentResolver.getType(returnUri)
            }
            localMediaFile.mimeType = mimeType ?: ""
            localMediaFile.path = contentUri.toString()
            val mmr = MediaMetadataRetriever()
            mmr.setDataSource(PlayerApplication.appContext, contentUri)

            if (localMediaFile.isVideoType || localMediaFile.isAudioType) {
               localMediaFile.duration = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION)!!.toLong()
            }
            if (localMediaFile.isVideoType) {
                val rotation = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_VIDEO_ROTATION)?.toInt()?:0
                var width = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_VIDEO_WIDTH)?.toInt()?:0
                var height = mmr.extractMetadata(MediaMetadataRetriever.METADATA_KEY_VIDEO_HEIGHT)?.toInt()?:0
                if (rotation == 90 || rotation == 270) {
                    val temp = width
                    width = height
                    height = temp
                }
                localMediaFile.width = width
                localMediaFile.height = height
            }
            mmr.release()

            val cursor = PlayerApplication.appContext.contentResolver.query(contentUri, null, null, null, null)
            if (cursor == null) {
                localMediaFile.fileName = contentUri.path ?: ""
            } else {
                cursor.moveToFirst()
                val displayNameIndex = cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.DISPLAY_NAME)
                val sizeIndex = cursor.getColumnIndex(OpenableColumns.SIZE)
                localMediaFile.fileName = cursor.getString(displayNameIndex)
                localMediaFile.fileSize = cursor.getLong(sizeIndex)
                cursor.close()
            }

            // 获取视频文件封面图
            if (localMediaFile.isVideoType && localMediaFile.support()) {
                runBlocking {
                    coroutineScope {
                        // todo; 用ffmpeg 实现
//                        val coverImgPath = extractImgFromVideo(contentUri.toString(), localMediaFile.fileName, localMediaFile.fileSize)
//                        localMediaFile.coverImgPath = coverImgPath
                    }
                }
            }

        } catch (e: Exception) { // 有的文件，比如 rmvb 文件会有这个错: java.lang.RuntimeException: setDataSource failed: status = 0x80000000
            val fileName = getFileName(contentUri)
            localMediaFile.fileName = fileName
            e.printStackTrace()
        }
        continuation.resume(localMediaFile, null)
    }




    fun getFileName(contentUri: Uri): String {
        var fileName: String = ""
        val cursor = PlayerApplication.appContext.contentResolver.query(contentUri, null, null, null, null)
        if (cursor == null) {
            fileName = contentUri.path?:""
        } else {
            cursor.moveToFirst()
            val idx = cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.DISPLAY_NAME)
            fileName = cursor.getString(idx)
            cursor.close()
        }
        return fileName
    }

    fun supportPlayType(fileExtension: String): Boolean {
        SUPPORT_MEDIA_FORMATS.forEach { supportFormat ->
            if (fileExtension.equals(supportFormat, ignoreCase = true)) {
                return true
            }
        }
        return false
    }
}