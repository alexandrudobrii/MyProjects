package com.profile.models;


public class FeedModel {
	private int id;
	private String shortNews;
	private String longNews;
	private String category;
	private String image;
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getShortNews() {
		return shortNews;
	}
	public void setShortNews(String shortNews) {
		this.shortNews = shortNews;
	}
	public String getLongNews() {
		return longNews;
	}
	public void setLongNews(String longNews) {
		this.longNews = longNews;
	}
	
	public String getImage() {
		return image;
	}
	public void setImage(String image) {
		this.image = image;
	}
	public String getCategory() {
		return category;
	}
	public void setCategory(String category) {
		this.category = category;
	}
}