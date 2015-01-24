package com.application.facebook.controller;

import java.io.IOException;
import java.net.URL;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v4.app.FragmentActivity;
import android.util.Log;

import com.application.utils.Constants;
import com.application.utils.UserDetails;
import com.facebook.HttpMethod;
import com.facebook.Request;
import com.facebook.Request.GraphUserCallback;
import com.facebook.Request.GraphUserListCallback;
import com.facebook.Response;
import com.facebook.Session;
import com.facebook.model.GraphUser;

public abstract class FacebookUtils {

	public static boolean checkPermissions (Session session, String permission) {
		if (session != null) {
			return session.getPermissions().contains(permission);
		} else
			return false;
	}

	public static boolean requestPermissions (Session session, FragmentActivity activity, List<String> permissions) {
		if (session != null) {
			session.requestNewPublishPermissions(new Session.NewPermissionsRequest(activity, permissions));
			return true;
		}
		return false;
	}
	
	public static void getFriendsList (Session session) {
		boolean activeSession = (session != null && session.isOpened());

        if (activeSession == true) {
        	Request.newMyFriendsRequest(session, new GraphUserListCallback() {
        		@Override
        		public void onCompleted(List<GraphUser> users, Response response) {
        			if (response.getError() == null) {
        				Log.d(Constants.FACEBOOK_TAG, "FRIENDS: " + response.toString());
                    	for (int i = 0; i < users.size(); i++) {
        					//Log.i(Constants.FACEBOOK_TAG, "users " + users.get(i).getName());
        				}
        			} else {
        				Log.d(Constants.FACEBOOK_TAG, "The request has completed with errors!");
        			}
        		}
        	}).executeAsync();
        } else {
        	Log.d(Constants.FACEBOOK_TAG, "The session is not active!");
        }
	}
	
	public static void sendUserDetails (Session session, final Activity activity, final Class<?> clazz) {
		boolean activeSession = (session != null && session.isOpened());
		
		if (activeSession == true) {
			final UserDetails userProfile = new UserDetails();
			Request.newMeRequest(session,  new GraphUserCallback() {
				@Override
				public void onCompleted (GraphUser user, Response response) {
					if (response.getError() == null) {
						Log.d(Constants.FACEBOOK_TAG, "ME: " + response.toString());
						userProfile.setId(user.getId());
						userProfile.setName(user.getName());
						
						new Thread(new Runnable() {
							@Override
							public void run() {
								try {
									FacebookMainFragment.myPhoto = getUserProfilePhoto(userProfile.getId());
								} catch (IOException e) {
									e.printStackTrace();
								}
							}
						}).start();
						
						Intent intent = new Intent(activity, clazz);
				        intent.putExtra(Constants.USER_PROFILE_TAG, userProfile);
				        activity.startActivity(intent);
					} else {
						Log.d(Constants.FACEBOOK_TAG, "The request has completed with errors!");
					}
				}
			}).executeAsync();
		} else {
        	Log.d(Constants.FACEBOOK_TAG, "The session is not active!");
        }
	}
	
	public static void getUserPicture (Session session) {
		new Request(
			    session,
			    "/me/picture",
			    null,
			    HttpMethod.GET,
			    new Request.Callback() {
			        public void onCompleted (Response response) {
			            if (response.getError() == null) {
			            	Log.d(Constants.FACEBOOK_TAG, "USER PROFILE PICTURE: " + response.toString());
			            }
			        }
			    }
			).executeAsync();
	}
	
	public static Bitmap getUserProfilePhoto (String userId) throws IOException {
		Session session = Session.getActiveSession();
		boolean activeSession = (session != null && session.isOpened());
		
		if (activeSession == true) {
			Log.d(Constants.FACEBOOK_TAG, "Retrieving user profile photo...");
			URL userPhoto = new URL("https://graph.facebook.com/"+userId+"/picture?type=large");
			Bitmap bitmap = BitmapFactory.decodeStream(userPhoto.openConnection().getInputStream());
			
			if (bitmap == null) {
				Log.d(Constants.FACEBOOK_TAG, "bitmap is nul");
			} else {
				Log.d(Constants.FACEBOOK_TAG, "bitmap is NOT nul");
			}
			return bitmap;
		}
		
		Log.d(Constants.FACEBOOK_TAG, "The session is not active");
		return null;
	}
	
	public static void parseResponse (String response) throws JSONException {
		JSONObject jsonObj = new JSONObject(response);
		JSONArray jarray = jsonObj.getJSONArray("");
		for (int i = 0; i < jarray.length(); i++) {
			JSONObject element = jarray.getJSONObject(i);
			Log.d(Constants.FACEBOOK_TAG, element.getString(""));
		}
	}
}