package com.application.notifications;

import java.io.Serializable;

public class NotificationsModel implements Serializable {
	private static final long serialVersionUID = 1L;

	private String notification;
	private String date;
	
	public String getNotification() {
		return notification;
	}
	
	public void setNotification(String notification) {
		this.notification = notification;
	}
	
	public String getDate() {
		return date;
	}
	
	public void setDate(String date) {
		this.date = date;
	}
}
