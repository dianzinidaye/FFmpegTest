package com.example.ffmepgtest;

import android.Manifest;
import android.os.Build;
import android.widget.Toast;

import androidx.fragment.app.FragmentActivity;

import com.tbruyelle.rxpermissions2.RxPermissions;

import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

public class PermissionUtil {

    public static void rxPermissionTest(final FragmentActivity context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            RxPermissions rxPermissions = new RxPermissions(context);
            rxPermissions.request(Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE)
                    .subscribe(new Observer<Boolean>() {
                        @Override
                        public void onSubscribe(Disposable d) {

                        }

                        @Override
                        public void onNext(Boolean aBoolean) {
                            if (aBoolean) {

                            } else {
                                //只有用户拒绝开启权限，且选了不再提示时，才会走这里，否则会一直请求开启
                                Toast.makeText(context, "主人，我被禁止啦，去设置权限设置那把我打开哟", Toast.LENGTH_LONG)
                                        .show();
                            }
                        }

                        @Override
                        public void onError(Throwable e) {

                        }

                        @Override
                        public void onComplete() {

                        }
                    });
        }
/*
* 检查是否有通知栏权限,,没有则跳转到通知设置界面
* */
      /*  if (!NotificationPermissUtil.isPermissionOpen(context)){
            NotificationPermissUtil. openPermissionSetting(context);
        }*/
    }
}
