package com.application.utils;

import android.graphics.Bitmap;

public class LogsDomainObject {
	private Bitmap   bitmap;
	private String   latitude;
	private String   longitude;
	private String   date;
	
	public Bitmap getBitmap() {
		return bitmap;
	}
	
	public void setBitmap(Bitmap bitmap) {
		this.bitmap = bitmap;
	}
	
	public String getLatitude() {
		return latitude;
	}

	public void setLatitude(String latitude) {
		this.latitude = latitude;
	}

	public String getLongitude() {
		return longitude;
	}

	public void setLongitude(String longitude) {
		this.longitude = longitude;
	}

	public String getDate() {
		return date;
	}

	public void setDate(String date) {
		this.date = date;
	}
	
	public String getPosition() {
		return "[" + latitude + ", " + longitude + "]";
	}
}
