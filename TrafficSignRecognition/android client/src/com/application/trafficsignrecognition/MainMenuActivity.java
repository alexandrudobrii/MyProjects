package com.application.trafficsignrecognition;

import android.app.ActionBar;
import android.app.ActionBar.Tab;
import android.app.FragmentTransaction;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.ViewPager;
import android.util.Log;

import com.application.adapters.TabsPagerAdapter;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;

public class MainMenuActivity extends FragmentActivity implements ActionBar.TabListener {
	private SharedPreferences       preferences;
	private ViewPager               viewPager;
	private TabsPagerAdapter        mAdapter;
	private ActionBar               actionBar;
	
	private static String           username = null;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d("NOTIFIER", "on Create - MainMenuActivity");
		
		setContentView(R.layout.activity_main_menu);
		
		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		AndroidUtils.setRotation(this, preferences.getBoolean(Constants.ROTATE_SCREEN, false));
		
		username = getIntent().getStringExtra(Constants.USERNAME);
		
		mAdapter = new TabsPagerAdapter(getSupportFragmentManager());
		viewPager = (ViewPager) findViewById(R.id.pager);
		viewPager.setAdapter(mAdapter);
		
		actionBar = getActionBar();
		actionBar.setHomeButtonEnabled(false);
		actionBar.setNavigationMode(ActionBar.NAVIGATION_MODE_TABS);		
		actionBar.setStackedBackgroundDrawable(new ColorDrawable(Color.BLUE));

		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_camera));
		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_gallery));
		if (username != null) {
			actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_activity));
		}
		actionBar.addTab(actionBar.newTab().setTabListener(this).setIcon(R.drawable.icon_settings));
		
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
	}
	
	public void onResume() {
		super.onResume();
		Log.d("NOTIFIER", "on Resume - MainMenuActivity");
		AndroidUtils.setRotation(this, preferences.getBoolean(Constants.ROTATE_SCREEN, false));
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
	
	public static boolean isLoginPresent() {
		return username != null;
	}
	
	public static String getUsername() {
		return username;
	}
	
	public void onStop() {
		super.onStop();
		Log.d("NOTIFIER", "on Stop - MainMenuActivity");
	}
}