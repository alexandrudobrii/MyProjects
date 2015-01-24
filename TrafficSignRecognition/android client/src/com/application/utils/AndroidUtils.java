package com.application.utils;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.InputStream;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.ByteArrayBody;
import org.apache.http.entity.mime.content.InputStreamBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;
import org.opencv.highgui.Highgui;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Typeface;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

public abstract class AndroidUtils {

	public static byte[] executeMultiPartRequest(String urlString, InputStream file, String fileName) {
    	HttpClient client = new DefaultHttpClient();
        HttpPost postRequest = new HttpPost(urlString);
        
        try {
        	//Set various attributes 
            MultipartEntity multiPartEntity = new MultipartEntity();
            multiPartEntity.addPart("fileName", new StringBody(fileName));
 
            // prepare payload
            InputStreamBody fileBody = new InputStreamBody(file, "application/octect-stream");
            multiPartEntity.addPart("attachment", fileBody);
 
            //Set to request body
            postRequest.setEntity(multiPartEntity);
            
           //Send request
            HttpResponse response = client.execute(postRequest);
            
            //Verify response if any
            if (response != null) {
                Log.d("REST", response.getStatusLine().getStatusCode()+"");
            }
            
            return EntityUtils.toByteArray(response.getEntity());
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
        
        return null;
	}
	
	public static byte[] executeMultiPartRequest(String urlString, byte[] data, String fileName) {
    	HttpClient client = new DefaultHttpClient();
        HttpPost postRequest = new HttpPost(urlString);
        
        try {
        	//Set various attributes 
            MultipartEntity multiPartEntity = new MultipartEntity();
            multiPartEntity.addPart("fileName", new StringBody(fileName));
 
            // prepare payload
            ByteArrayBody byteArray = new ByteArrayBody(data, "application/octect-stream");
            multiPartEntity.addPart("attachment", byteArray);
 
            //Set to request body
            postRequest.setEntity(multiPartEntity);
            
           //Send request
            HttpResponse response = client.execute(postRequest);
            
            //Verify response if any
            if (response != null) {
                Log.d("REST", response.getStatusLine().getStatusCode()+"");
            }
            
            return EntityUtils.toByteArray(response.getEntity());
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
        
        return null;
	}
	
	public static Bitmap byteArrayToBitmap (byte[] array) {
		return BitmapFactory.decodeByteArray(array, 0, array.length);
	}
	
	public static Mat byteArrayToMat (byte[] array) {
		return Highgui.imdecode(new MatOfByte(array), Highgui.IMREAD_COLOR);
	}
	
	public static InputStream byteArrayToInputstream (byte[] array) {
		return new ByteArrayInputStream(array);
	}
	
	public static Bitmap matToBitmap (Mat mat) {
		Bitmap bitmap = Bitmap.createBitmap(mat.cols(), mat.rows(), Bitmap.Config.ARGB_8888);
		Utils.matToBitmap(mat, bitmap);
		
		return bitmap;
	}
	
	public static Mat bitmapToMat (Bitmap bitmap) {
		Mat mat = new Mat(bitmap.getHeight(), bitmap.getWidth(), CvType.CV_8UC1);
		Utils.bitmapToMat(bitmap, mat);
		
		return mat;
	}
	
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