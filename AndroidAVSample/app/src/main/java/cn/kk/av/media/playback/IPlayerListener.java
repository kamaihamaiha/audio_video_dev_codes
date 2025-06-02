package cn.kk.av.media.playback;

public interface IPlayerListener {

    boolean genericOnInfo(Object inObj, int what, int extra);

    boolean genericOnError(Object inObj, int what, int extra);

    void genericOnCompletion();

    void genericOnBufferingUpdate(int percent);

    void genericOnSeek();

    void genericOnPrepared();

    void onInternalError(Exception ex);
}
