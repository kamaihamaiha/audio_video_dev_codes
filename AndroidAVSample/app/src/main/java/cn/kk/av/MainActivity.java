package cn.kk.av;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import cn.kk.av.databinding.ActivityMainBinding;
import cn.kk.av.ui.BaseActivity;

public class MainActivity extends BaseActivity {

    // Used to load the 'av' library on application startup.
    static {
        System.loadLibrary("av");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // todo open next ui
        binding.btnOpenPlayer.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, PlayerActivity.class));
            }
        });
    }

    /**
     * A native method that is implemented by the 'av' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}