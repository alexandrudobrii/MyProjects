package com.application.google.maps;

import java.util.HashMap;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;

import com.application.social.maps.R;
import com.application.socialandroidapplication.HomeActivity;
import com.application.utils.Constants;
import com.application.utils.MapsUtil;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.MapFragment;
import com.google.android.gms.maps.model.Marker;

@SuppressLint("UseSparseArrays")
public class SocialMapActivity extends Activity {
	private GPSTracker 				gpsTracker;
	private GoogleMap 				map;
	private Map<String, Marker>		markers;
	private boolean 				requestFocus;
	private static Timer 			timer;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_social_map);
		
		map = ((MapFragment) getFragmentManager().findFragmentById(R.id.map)).getMap();
		map.setMyLocationEnabled(true);
        
		timer = new Timer();
		markers = new HashMap<String, Marker>();
		gpsTracker = new GPSTracker(SocialMapActivity.this);
		
		if (gpsTracker.canGetLocation()) {
			requestFocus = false;
        	updateUsersLocationTask(getIntent().getIntExtra(Constants.USER_PROFILE_TAG, Constants.NONE));
        }  else {
        	gpsTracker.showSettingsAlert();
        }
	}
	
	public void updateUsersLocationTask (final int userPosition) {
	    final Handler handler = new Handler();
	    TimerTask doAsynchronousTask = new TimerTask() {       
	        @Override
	        public void run() {
	            handler.post(new Runnable() {
	                public void run() {       
	                    try {
	                    	synchronized (HomeActivity.usersList) {
	                    		for (int i = 0; i < HomeActivity.usersList.size(); i++) {
		                    		MapsUtil.updateMarker(map, markers, HomeActivity.usersList.get(i));
		                    	}
		                    	if (requestFocus == false) {
		                    		if (userPosition == Constants.CURRENT_USER_ID) {
		                    			MapsUtil.focusOnUser(map, HomeActivity.myDetails);
		                    		} else if (userPosition != Constants.NONE) {
		                    			MapsUtil.focusOnUser(map, HomeActivity.usersList.get(userPosition));
		                    		}
		                    		requestFocus = true;
		                    	}
							}
	                    } catch (Exception e) {
	                    	/* catch nothing */
	                    }
	                }
	            });
	        }
	    };
	    timer.schedule(doAsynchronousTask, 0, Constants.REQUEST_GAP);	
	}
	
	public static void stopTimer() {
		if (timer != null) {
			timer.cancel();
		}
	}
}