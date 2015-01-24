package com.application.trafficsignrecognition;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;

import com.application.utils.AndroidUtils;
import com.application.utils.Constants;

public class LogoActivity extends Activity {
	private SharedPreferences preferences;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_logo);
		
		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		AndroidUtils.setRotation(this, preferences.getBoolean(Constants.ROTATE_SCREEN, false));
		
		LayoutRedirect layoutRedirect  = new LayoutRedirect();
		layoutRedirect.start();
	}
	
	private class LayoutRedirect extends Thread {
		public void run() {
			try {
				Thread.sleep(Constants.PAUSE_VALUE);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			if (preferences.getBoolean(Constants.SKIP_LOGIN, false) == true) {
				Intent intent = new Intent(LogoActivity.this, MainMenuActivity.class);
				startActivity(intent);
			} else {
				Intent intent = new Intent(LogoActivity.this, LoginActivity.class);
				startActivity(intent);
			}
		}
	}
	
	public void onPause() {
		super.onPause();
		finish();
	}
	
	public void onStop() {
		super.onStop();
		
		Log.d("NOTIFIER", "Thread was stopped by LogoActivity");
		UserActivity.finished = true;
	}
} 