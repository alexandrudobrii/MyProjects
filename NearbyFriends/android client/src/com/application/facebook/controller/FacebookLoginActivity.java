package com.application.facebook.controller;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

import com.facebook.AppEventsLogger;

public class FacebookLoginActivity extends FragmentActivity {
	private FacebookMainFragment facebookMainFragment;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);

	    if (savedInstanceState == null) {
	        // Add the fragment on initial activity setup
	    	facebookMainFragment = new FacebookMainFragment();
	        getSupportFragmentManager()
	        .beginTransaction()
	        .add(android.R.id.content, facebookMainFragment)
	        .commit();
	    } else {
	        // Or set the fragment from restored state info
	    	facebookMainFragment = (FacebookMainFragment) getSupportFragmentManager()
	        .findFragmentById(android.R.id.content);
	    }
	}
	
	@Override
	protected void onResume() {
	  super.onResume();

	  // Logs 'install' and 'app activate' App Events.
	  AppEventsLogger.activateApp(this);
	}
	
	@Override
	protected void onPause() {
	  super.onPause();

	  // Logs 'app deactivate' App Event.
	  AppEventsLogger.deactivateApp(this);
	}
}