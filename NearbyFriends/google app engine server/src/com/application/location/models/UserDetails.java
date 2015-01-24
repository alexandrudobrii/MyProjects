package com.application.location.models;

import java.io.Serializable;

import javax.xml.bind.annotation.XmlAccessType;
import javax.xml.bind.annotation.XmlAccessorType;
import javax.xml.bind.annotation.XmlAttribute;
import javax.xml.bind.annotation.XmlElement;
import javax.xml.bind.annotation.XmlRootElement;

@XmlAccessorType(XmlAccessType.NONE)
@XmlRootElement(name = "user")
public class UserDetails implements Serializable {
	private static final long serialVersionUID = 1L;
	
	@XmlAttribute(name = "id")
	private String 		id;
	
	@XmlElement(name = "longitude")
	private double 		longitude;
	
	@XmlElement(name = "latitude")
	private double 		latitude;
	
	@XmlElement(name = "requestFocus")
	private boolean 	requestFocus;
	
	@XmlElement(name = "enabled")
	private boolean 	enabled;
	
	@XmlElement(name = "name")
	private String 		name;
	
	@XmlElement(name = "description")
	private String 		description;
	
	@XmlElement (name = "radius")
	private float radius;
	
	@XmlElement(name = "status")
	private String status;
	
	@XmlElement(name = "notification")
	private String notification;
	
	public UserDetails() { 
	}
	
	public UserDetails (double latitude, double longitude) {
		this.latitude = latitude;
		this.longitude = longitude;
	}
	
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public void setLongitude(double longitude) {
		this.longitude = longitude;
	}
	public void setLatitude(double latitude) {
		this.latitude = latitude;
	}
	public boolean isRequestFocus() {
		return requestFocus;
	}
	public void setRequestFocus(boolean requestFocus) {
		this.requestFocus = requestFocus;
	}
	public double getLongitude() {
		return longitude;
	}
	public double getLatitude() {
		return latitude;
	}
	public boolean isEnabled() {
		return enabled;
	}
	public void setEnabled(boolean enabled) {
		this.enabled = enabled;
	}
	public String getNotification() {
		return notification;
	}
	public void setNotification(String notification) {
		this.notification = notification;
	}
	
	public float getRadius() {
		return radius;
	}

	public void setRadius(float radius) {
		this.radius = radius;
	}
	public String getStatus() {
		return status;
	}
	public void setStatus(String status) {
		this.status = status;
	}
	
	public String toString() {
		return "[" + id + "," + longitude + "," + latitude +  "," + 
				requestFocus + "," + enabled + "," + name +  "," + description + "," + 
				radius + "," + status + "," + notification + "]";
	}
	
	public static UserDetails parseResponse (String response) {
		UserDetails user = new UserDetails();
		String temp = response.substring(response.indexOf("[") + 1, response.indexOf("]"));
		String[] data = temp.split(",");
		
		user.setId(data[0]);
		user.setLongitude(Double.parseDouble(data[1]));
		user.setLatitude(Double.parseDouble(data[2]));
		user.setRequestFocus(Boolean.parseBoolean(data[3]));
		user.setEnabled(Boolean.parseBoolean(data[4]));
		user.setName(data[5]);
		user.setDescription(data[6]);
		user.setRadius(Float.parseFloat(data[7]));
		
		if (data.length == 9 || data.length == 10) {
			user.setStatus(data[8]);
		}
		if (data.length == 10) {
			user.setNotification(data[9]);
		}
		
		return user;
	}

}