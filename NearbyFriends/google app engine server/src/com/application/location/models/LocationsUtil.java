package com.application.location.models;

public class LocationsUtil {

	// get distance between two coordinates in meters
	public static float getDistance (UserDetails user1, UserDetails user2) {
	    double lat1 = user1.getLatitude();
	    double lng1 = user1.getLongitude();
	    double lat2 = user2.getLatitude();
	    double lng2 = user2.getLongitude();
		
		double earthRadius = 6371; //kilometers
	    double dLat = Math.toRadians(lat2-lat1);
	    double dLng = Math.toRadians(lng2-lng1);
	    double a = Math.sin(dLat/2) * Math.sin(dLat/2) +
	               Math.cos(Math.toRadians(lat1)) * Math.cos(Math.toRadians(lat2)) *
	               Math.sin(dLng/2) * Math.sin(dLng/2);
	    double c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));
	    float dist = (float) (earthRadius * c);

	    return dist * 1000.0f;	
	}
	
	public static boolean isInRange (UserDetails user1, UserDetails user2, float radius) {
		return getDistance(user1, user2) <= radius;
	}
	
}