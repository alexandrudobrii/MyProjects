package com.application.notifications;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

import com.application.utils.Constants;
import com.google.gson.Gson;

public class NotificationSharedPreferences {
	private SharedPreferences 		sharedPreferences;
	
	@SuppressLint("UseSparseArrays") public NotificationSharedPreferences (Activity activity) {
		sharedPreferences = activity.getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
	}
	
	public void addNotification (String userId, NotificationsModel notificationModel) {
		List<NotificationsModel> notifications = getNotifications();
		notifications.add(0, notificationModel);
		addNotifications(notifications);
	}
	
	public void removeNotification (String userId, NotificationsModel notificationModel) {
		List<NotificationsModel> notifications = getNotifications();
		if (notifications != null) {
			notifications.remove(notificationModel);
			addNotifications(notifications);
		}
	}
	
	public void clearNotifications() {
		List<NotificationsModel> notifications = getNotifications();
		if (notifications != null) {
			notifications.clear();
			addNotifications(notifications);
		}
	}
	
	public void addNotifications(List<NotificationsModel> favorites) {
		Gson gson = new Gson();
		String jsonFavorites = gson.toJson(favorites);
		sharedPreferences.edit().putString(Constants.NOTIFICATIONS, jsonFavorites).commit();
	}
	
	public List<NotificationsModel> getNotifications() {
		if (sharedPreferences.contains(Constants.NOTIFICATIONS)) {
			String jsonNotifications = sharedPreferences.getString(Constants.NOTIFICATIONS, null);
			Gson gson = new Gson();
			NotificationsModel[] favoriteItems = gson.fromJson(jsonNotifications, NotificationsModel[].class);
			List<NotificationsModel> favorites = Arrays.asList(favoriteItems);
	        favorites = new ArrayList<NotificationsModel>(favorites);
	        return favorites;
		}
		return new ArrayList<NotificationsModel>();
	}
}