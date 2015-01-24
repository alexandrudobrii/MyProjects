package com.application.socialandroidapplication;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Timer;
import java.util.TimerTask;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.FragmentTransaction;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;

import com.application.adapters.TabsPagerAdapter;
import com.application.google.maps.GPSTracker;
import com.application.server.communication.UsersRequest;
import com.application.social.maps.R;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.UserDetails;

public class HomeActivity extends FragmentActivity implements ActionBar.TabListener {
	private SharedPreferences       	preferences;
	private ViewPager               	viewPager;
	private TabsPagerAdapter        	mAdapter;
	private ActionBar              	 	actionBar;
	private GPSTracker 					gpsTracker;
	public static Map<String, Bitmap>	userPhoto;
	private static Timer				timer;
	
	public static UserDetails			myDetails;
	public static List<UserDetails> 	usersList;
	
//	public static List<NotificationsModel> 	notifications;
//	public static NotificationSharedPreferences notificationSharedPreferences;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_home);
		
		timer = new Timer();
		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		AndroidUtils.setRotation(this, false);
		
		mAdapter = new TabsPagerAdapter(getSupportFragmentManager());
		viewPager = (ViewPager) findViewById(R.id.pager);
		viewPager.setAdapter(mAdapter);
		
		actionBar = getActionBar();
		actionBar.setHomeButtonEnabled(false);
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);		
		actionBar.setStackedBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_map));
		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_list));
		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_settings));
		//actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_settings));
		
		viewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {
			@Override
			public void onPageSelected(int position) {
				actionBar.setSelectedNavigationItem(position);
			}

			@Override
			public void onPageScrolled(int arg0, float arg1, int arg2) {
			}

			@Override
			public void onPageScrollStateChanged(int arg0) {
			}
		});
		
//		notifications = new ArrayList<NotificationsModel>();
//		notificationSharedPreferences = new NotificationSharedPreferences(this);

		userPhoto = new HashMap<String, Bitmap>();
		gpsTracker = new GPSTracker(HomeActivity.this);
		usersList = new ArrayList<UserDetails>();
		myDetails = (UserDetails)getIntent().getSerializableExtra(Constants.USER_PROFILE_TAG);
		
		updateUsersListRequest();
	}
	
	public void onResume() {
		super.onResume();
		AndroidUtils.setRotation(this, false);
	}
	
	@Override
	public void onBackPressed() {
	}
	
	@Override
	public void onTabReselected(Tab tab, FragmentTransaction ft) {
	}

	@Override
	public void onTabSelected(Tab tab, FragmentTransaction ft) {
		viewPager.setCurrentItem(tab.getPosition());
	}

	@Override
	public void onTabUnselected(Tab tab, FragmentTransaction ft) {
	}
	
	public void updateUsersListRequest() {
	    final Handler handler = new Handler();
	    TimerTask doAsynchronousTask = new TimerTask() {       
	        @Override
	        public void run() {
	            handler.post(new Runnable() {
	                public void run() {       
	                    try {
	                    	setUserDetails();
	                    	UsersRequest usersRequest = new UsersRequest(myDetails);
	                		usersRequest.execute();
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
	
	public void setUserDetails() {
		if (gpsTracker.canGetLocation()) {
			myDetails.setLatitude(gpsTracker.getLatitude());
			myDetails.setLongitude(gpsTracker.getLongitude());
			myDetails.setEnabled(!preferences.getBoolean(Constants.ENABLE_PRIVACY, true));
			myDetails.setRadius(preferences.getFloat(Constants.SET_RADIUS, 0.0f));
			myDetails.setDescription("I`m awsome");
		} else {
			gpsTracker.showSettingsAlert();
		}
	}
}