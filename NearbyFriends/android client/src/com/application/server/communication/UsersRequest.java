package com.application.server.communication;

import java.io.IOException;

import org.springframework.http.ResponseEntity;

import android.os.AsyncTask;
import android.util.Log;

import com.application.facebook.controller.FacebookUtils;
import com.application.socialandroidapplication.HomeActivity;
import com.application.socialandroidapplication.UsersActivity;
import com.application.utils.Constants;
import com.application.utils.MapsUtil;
import com.application.utils.ServerConnection;
import com.application.utils.UserDetails;

public class UsersRequest extends AsyncTask<Void, Void, String> {	
	private UserDetails 			user;
	private ServerConnection		serverConnection;
	
	public UsersRequest (UserDetails user) {
		this.user = user;
		this.serverConnection = new ServerConnection();
	}
	
	@Override
	protected String doInBackground(Void... data) {
		String url = serverConnection.getBaseUri() + "locations/gps_radius";
		
		Log.d(Constants.MAP_TAG, "Sending to server the following user details...");
		MapsUtil.logUserDetails(Constants.MAP_TAG, user);
		
		ResponseEntity<String> response = 
    			serverConnection.getConnection().postForEntity(url, user, String.class);
    	return response.getBody();
	}
		
	@Override
	protected void onPostExecute (String result) {
		Log.d(Constants.MAP_TAG, "Receiving from server the following users list...");
		Log.d(Constants.MAP_TAG, "Users:" + result);

		if (result == null) {
			return;
		}
		
		synchronized (HomeActivity.usersList) {
			HomeActivity.usersList.clear();
			String users[] = result.split(";");
			
			for (int i = 0; i < users.length; i++) {
				final UserDetails friendData = UserDetails.parseResponse(users[i]);	
				HomeActivity.usersList.add(friendData);
				
				if (HomeActivity.userPhoto.get(friendData.getId()) == null) {
					new Thread(new Runnable() {
						@Override
						public void run() {
							String userId = friendData.getId();
							try {
								HomeActivity.userPhoto.put(userId, FacebookUtils.getUserProfilePhoto(userId));
							} catch (IOException e) {
								e.printStackTrace();
							}
						}
					}).start();
				} 
				
//				if (friendData.getNotification() != null && !friendData.getNotification().equals("")) {
//					String userId = friendData.getId();
//					NotificationsModel notificationModel = new NotificationsModel();
//					notificationModel.setNotification(friendData.getNotification());
//					notificationModel.setDate(JavaUtils.getCurrentDate(Constants.DATE_FORMAT));
//					
//					HomeActivity.notificationSharedPreferences.addNotification(userId, notificationModel);
//				}
			}
			
			//HomeActivity.notifications.clear();
			//HomeActivity.notifications = HomeActivity.notificationSharedPreferences.getNotifications();
		}
		
		if (UserDetails.class != null && UsersActivity.usersListAdapter != null) {
			UsersActivity.usersListAdapter.notifyDataSetChanged();
		}
		
//		if (NotificationsActivity.class != null && NotificationsActivity.notificationsListAdapter != null) {
//			NotificationsActivity.notificationsListAdapter.notifyDataSetChanged();
//		}
	}
}