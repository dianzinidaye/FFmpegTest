package com.example.ffmepgtest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private SurfaceView surfaceView;
    private HuGePlayer huGePlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        PermissionUtil.rxPermissionTest(this);
        // Example of a call to a native method
        surfaceView = findViewById(R.id.surfaceView);
        surfaceView.setOnClickListener(this);
        findViewById(R.id.bottom).setOnClickListener(this);
        huGePlayer = new HuGePlayer();
        huGePlayer.setSurfaceView(surfaceView);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
 //   public native String stringFromJNI();

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.bottom:
               /* File file = new File(Environment.getExternalStorageDirectory(), "input.mp4");
                Log.i("TAG", "onClick: " + file.getAbsolutePath());
                boolean isExists = file.exists();
                Log.i("TAG", "onClick: " + isExists);
                huGePlayer.start(file.getAbsolutePath());*/

                break;
            case R.id.surfaceView:
                File file = new File(Environment.getExternalStorageDirectory(), "input.mp4");
                Log.i("TAG", "onClick: " + file.getAbsolutePath());
                boolean isExists = file.exists();
                Log.i("TAG", "onClick: " + isExists);
                huGePlayer.start(file.getAbsolutePath());

                break;
        }
    }
}
