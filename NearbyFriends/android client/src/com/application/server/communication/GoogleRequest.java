package com.application.server.communication;

import java.util.HashMap;
import java.util.Map;

import org.springframework.http.ResponseEntity;

import android.os.AsyncTask;
import android.util.Log;

import com.application.utils.Constants;
import com.application.utils.MapsUtil;
import com.application.utils.ServerConnection2;
import com.application.utils.UserDetails;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.model.Marker;

public class GoogleRequest extends AsyncTask<Void, Void, String> {	
	private UserDetails 			userDetails;
	private ServerConnection2 		serverConnection;
	private GoogleMap				map;
	private Map<String, Marker>		markers;
	
	public GoogleRequest (GoogleMap map, UserDetails userDetails, ServerConnection2 serverConnection) {
		this.userDetails = userDetails;
		this.serverConnection  = serverConnection;
		this.map = map;
		this.markers = new HashMap<String, Marker>();
	}
	
	@Override
	protected String doInBackground (Void... nothing) {
		String url = serverConnection.getBaseUri() + "locations/gps";
	
    	ResponseEntity<String> response = 
    			serverConnection.getConnection().postForEntity(url, userDetails, String.class);
    		
		return response.getBody();
	}
		
	@Override
	protected void onPostExecute(String result) {
		Log.d(Constants.MAP_TAG, "Users:" + result);
		if (result == null) {
			Log.d(Constants.MAP_TAG, "There are not user connected right now");
			return;
		}
		
		String users[] = result.split(";");
		for (int i = 0; i < users.length; i++) {
			UserDetails friendData = UserDetails.parseResponse(users[i]);
			MapsUtil.updateMarker(map, markers, friendData);
		}
	}
}