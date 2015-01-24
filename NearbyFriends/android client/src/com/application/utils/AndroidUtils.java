package com.application.utils;

import java.io.File;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Typeface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.widget.TextView;
import android.widget.Toast;

public abstract class AndroidUtils {

	public static boolean isInternetAvailable (Activity activity) {
		ConnectivityManager cm = (ConnectivityManager)activity.getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo netInfo = cm.getActiveNetworkInfo();
		
		return (netInfo != null) && (netInfo.isConnectedOrConnecting());
	}
	
	public static void showToast (Activity activity, String text, int duration) {
		Context context = activity.getApplicationContext();
		Toast toast = Toast.makeText(context, text, duration);
		toast.show();
	}
	
	public static void setFont (Activity activity, TextView textView, String style, String text) {
		Typeface tf = Typeface.createFromAsset(activity.getAssets(), style);
		textView.setTypeface(tf);
		textView.setText(text);
	}
	
	public static void clearSharedPreferences(Context ctx) {
	    File dir = new File(ctx.getFilesDir().getParent() + "/shared_prefs/");
	    String[] children = dir.list();
	    for (int i = 0; i < children.length; i++) {
	        // clear each of the prefrances
	        ctx.getSharedPreferences(children[i].replace(".xml", ""), Context.MODE_PRIVATE).edit().clear().commit();
	    }
	    // Make sure it has enough time to save all the commited changes
	    try { Thread.sleep(1000); } catch (InterruptedException e) {}
	    for (int i = 0; i < children.length; i++) {
	        // delete the files
	        new File(dir, children[i]).delete();
	    }
	}
	
	public static void setRotation (Activity activity, boolean rotate) {
		if (rotate == false) {
			activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		} else {
			activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		}
	}
}