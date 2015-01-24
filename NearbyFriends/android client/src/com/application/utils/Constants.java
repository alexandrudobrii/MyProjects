package com.application.utils;

public abstract class Constants {

	// tags
	public static final String MAP_TAG 						= "MAP";
	public static final String FACEBOOK_TAG 				= "FACEBOOK";
	public static final String SETTINGS_TAG					= "SETTINGS";
	public static final String USER_PROFILE_TAG				= "USER_PROFILE";
	public static final String USER_ARRAY_POSITION			= "USER_POSITION";
	
	// date format
	public static final String DATE_FORMAT					= "HH:mm";
	
	// map
	public static final int NONE							= -1;
	public static final int CURRENT_USER_ID					= -2;
	public static final int ZOOM_LEVEL		  				= 15;
	public static final int REQUEST_GAP						= 5000;
	
	// server connection
	public static final String BASE_URI_PROPERTY  			= "base.uri";
	
	// shared preferences
	public static final String PREFERENCES_FILE            	= "PREFERENCES";
	public static final String NOTIFICATIONS				= "NOTIFICATIONS";
	
	// layout fonts
	public static final String TEXT_FONT                   = "fonts/chalkdust.ttf";
	
	// settings activity
	public static final String ENABLE_PRIVACY				= "Enable privacy";
	public static final String SET_RADIUS					= "Set radius";
	public static final String RADIUS_INDEX					= "Radius index";
	public static final String CLEAR_NOTIFICATIONS			= "Clear notifications";
	public static final String NOTIFICATIONS_CLEARED		= "Notifications cleared!";
	public static final String CLEAR_CACHE                 	= "Clear cache";
	public static final String CACHE_CLEARED             	= "Cache cleared!";
	public static final String ROTATE_SCREEN 			   	= "Rotate screen";
	public static final String LOGOUT                   	= "Logout";
	
	// users status
	public static final String USER_NOT_IN_RANGE			= "not in range";
	public static final String USER_ONLINE					= "online";
	public static final String USER_OFFLINE					= "offline";
	public static final String USER_UNKNOWN					= "unknown";
	
	// radius setting list
	public static final String ALL_MAP_VISIBILITY			= "All map visibility";
	public static final CharSequence[] selectionRadiusList  = {ALL_MAP_VISIBILITY,
															   "250 meters",
															   "500 meters",
															   "1 kilometer", 
															   "5 kilometers", 
															   "10 kilometers"};
	
}