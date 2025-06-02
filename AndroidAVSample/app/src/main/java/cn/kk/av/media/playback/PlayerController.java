package cn.kk.av.media.playback;

import android.content.Context;
import android.media.AudioManager;
import androidx.annotation.NonNull;
import androidx.media3.common.PlaybackException;
import androidx.media3.common.Player;

public class PlayerController {
    public static final int PLAYER_EXO = 0;
    public static final int PLAYER_EXO_MEDIA3 = 1;
    public static final int PLAYER_LEGACY_SYSTEM = 2;
    private Context context;
    private IPlayer mediaPlayer;
    private IPlayerListener playerListener;
    private int playerType;
    KKMedia3Player.EventListener mMedia3EventListener;

    public PlayerController(Context context, IPlayerListener iPlayerListener) {
        this.context = context;
        this.playerListener = iPlayerListener;
    }

    public void createMedia3Player(){
        createPlayer(PLAYER_EXO_MEDIA3);
    }

    public void createPlayer(int type) {
        this.playerType = type;
        switch (type) {
            case PLAYER_EXO_MEDIA3:
                mediaPlayer = new KKMedia3Player(context);
                break;
        }
        mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
        setPlayerListener(mediaPlayer);
    }

    public void setPlayerListener(IPlayer mp) {
        switch (playerType) {
            case PLAYER_EXO_MEDIA3:
                if(mMedia3EventListener == null) {
                    mMedia3EventListener = new KKMedia3Player.EventListener() {

                        @Override
                        public void controlNext() {

                        }

                        @Override
                        public void controlPrevious() {

                        }

                        @Override
                        public void controlPause() {

                        }

                        @Override
                        public void controlPlay() {

                        }

                        @Override
                        public void playerError(@NonNull PlaybackException error) {
                            playerListener.genericOnError(error, 0, 0);
                        }

                        @Override
                        public void playerStateChanged(boolean playWhenReady, int playbackState) {
                            switch (playbackState) {
                                case Player.STATE_READY:
                                    playerListener.genericOnPrepared();
                                    break;
                                case Player.STATE_ENDED:
                                    if (playWhenReady) {
                                        playerListener.genericOnCompletion();
                                    } else {
                                        //ignore
                                    }
                                    break;
                            }
                        }
                    };
                }
                ((KKMedia3Player) mp).addListener(mMedia3EventListener);
                break;
        }
    }

    public synchronized IPlayer getMediaPlayer() {
        return this.mediaPlayer;
    }

    public String getPlayerName(){
        switch (playerType){
            case PLAYER_EXO:
                return "ExoPlayer";
            case PLAYER_EXO_MEDIA3:
                return "ExoMedia3";
            case PLAYER_LEGACY_SYSTEM:
                return "SystemPlayer";
        }
        return "Unknown";
    }

    boolean requestReset = false;

    public void stop(){
        getMediaPlayer().stop();
    }

    public synchronized void resetMediaPlayer() {
        if (requestReset) {
            return;
        }
        requestReset = true;
        try {
            if (isPlaying()) {
                mediaPlayer.stop();
            }
        } catch (Exception e) {
        }

        try {
            if (mediaPlayer != null) {
                mediaPlayer.reset();
                mediaPlayer.release();
            }
            mediaPlayer = null;
        } catch (Exception e) {
            e.printStackTrace();
        }

        requestReset = false;
    }

    public boolean isPlaying() {
        try {
            return getMediaPlayer() != null && getMediaPlayer().isPlaying();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

}
