package com.application.utils;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import android.location.Location;
import android.util.Log;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

public class MapsUtil {
	
	public static void showItemOnMap (GoogleMap map, UserDetails userDetails) {
		LatLng position = new LatLng(userDetails.getLatitude(), userDetails.getLongitude());
		
		if (userDetails.getLatitude() != 0.0 && userDetails.getLongitude() != 0.0) {
			map.addMarker(new MarkerOptions()
					.title(userDetails.getName())
					.position(position)
					.snippet(userDetails.getDescription()));
		} else {
			Log.d(Constants.MAP_TAG, "Position could not be determined");
		}
	}
	
	public static void updateMarker (GoogleMap map, Map<String, Marker> markers, UserDetails userDetails) {
		Marker marker = markers.get(userDetails.getId());
		if (userDetails.isEnabled() == false) {
			if (marker != null) {
				marker.remove();
				markers.remove(userDetails.getId());
			} 			
			return;
		}
		
		LatLng position = new LatLng(userDetails.getLatitude(), userDetails.getLongitude());
		if (marker == null) {
			marker = map.addMarker(new MarkerOptions()
					.title(userDetails.getName())
					.position(position)
					.snippet(userDetails.getDescription()));
		} else {
			marker.setTitle(userDetails.getName());
			marker.setPosition(position);
			marker.setSnippet(userDetails.getDescription());
		}
		markers.put(userDetails.getId(), marker);
	}
	
	public static void focusOnUser (GoogleMap map, UserDetails userDetails) {
		LatLng position = new LatLng(userDetails.getLatitude(), userDetails.getLongitude());;
		map.moveCamera(CameraUpdateFactory.newLatLngZoom(position, Constants.ZOOM_LEVEL));
	}
	
	public static float getDistance (UserDetails userDetails1, UserDetails userDetails2) {
		Location location1 = new Location(userDetails1.getName());
		Location location2 = new Location(userDetails2.getName());
		return location1.distanceTo(location2);
	}
	
	public static List<UserDetails> getVisibleUsers (UserDetails me, List<UserDetails> others, float radius) {
		List<UserDetails> visibleUsers = new ArrayList<UserDetails>();
		
		for (int i = 0; i < others.size(); i++) {
			if (getDistance(me, others.get(i)) <= radius) {
				visibleUsers.add(others.get(i));
			}
		}
		
		return visibleUsers;
	}
	
	public static void logUserDetails (String tag, UserDetails friendData) {
		Log.d(tag, friendData.getDescription());
		Log.d(tag, friendData.getName());
		Log.d(tag, friendData.getId()+"");
		Log.d(tag, friendData.getLatitude()+"");
		Log.d(tag, friendData.getLongitude()+"");
		Log.d(tag, friendData.isEnabled()+"");
		Log.d(tag, friendData.isRequestFocus()+"");
	}
	
	public static List<UserDetails> generateLocations() {
		List<UserDetails> list = new ArrayList<UserDetails>();
		
		UserDetails users = new UserDetails();
		users.setId(1+"");
		users.setDescription("Marker no. 1");
		users.setName("Data no. 1");
		users.setEnabled(true);
		users.setLatitude(45.248491);
		users.setLongitude(25.635721);
		list.add(users);
		
		users = new UserDetails();
		users.setId(2+"");
		users.setDescription("Marker no. 2");
		users.setName("Data no. 2");
		users.setEnabled(true);
		users.setLatitude(45.248057);
		users.setLongitude(25.637717);
		list.add(users);
		
		users = new UserDetails();
		users.setId(3+"");
		users.setDescription("Marker no. 3");
		users.setName("Data no. 3");
		users.setEnabled(true);
		users.setLatitude(45.258001);
		users.setLongitude(25.629843);
		list.add(users);
		
		users = new UserDetails();
		users.setId(4+"");
		users.setDescription("Marker no. 4");
		users.setName("Data no. 4");
		users.setEnabled(false);
		users.setLatitude(45.246678);
		users.setLongitude(25.636784);
		list.add(users);
		
		users = new UserDetails();
		users.setId(5+"");
		users.setDescription("Marker no. 5");
		users.setName("Data no. 5");
		users.setEnabled(false);
		users.setLatitude(45.247732);
		users.setLongitude(25.637717);
		list.add(users);
		
		return list;
	}
}