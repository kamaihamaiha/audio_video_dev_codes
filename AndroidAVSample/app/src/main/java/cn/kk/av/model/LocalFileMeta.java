package cn.kk.av.model;

import android.text.TextUtils;

import cn.kk.av.util.MyResourceUtil;

public class LocalFileMeta {
    public String mimeType;
    public int width;
    public int height;
    public String path;
    public String fileName;
    public String coverImgPath; // 封面图
    public long fileSize;
    public long duration; // ms
    public boolean videoType;
    public boolean support;

    public boolean isVideoType() {
        if (TextUtils.isEmpty(mimeType)) return false;
        videoType = mimeType.startsWith("video/");
        return videoType;
    }

    public boolean isAudioType() {
        if (TextUtils.isEmpty(mimeType)) return false;
        return mimeType.startsWith("audio/");
    }

    public boolean support(){
        boolean formatSupport = MyResourceUtil.INSTANCE.supportPlayType(getFileExtensionName());
        if (formatSupport && (isAudioType() || isVideoType())) return true;

        return false;
    }

    public String getFileExtensionName() {
        if (TextUtils.isEmpty(fileName)) return "";
        int index = fileName.lastIndexOf(".");
        if (index == -1) return "";
        return fileName.substring(index + 1);
    }
}
