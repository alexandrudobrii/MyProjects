package com.profile.dao;

public abstract class Querries {

	public static String userQuerry (int id) {
		if (id < 0) {
			return "select id, username, password from users";
		}
		
		return "select id, username, password from users where id = " + id;
	}
	
	public static String feedQuerry (int id) {
		if (id < 0) {
			return "select id, short_news, long_news, category, image from feed";
		}
		
		return "select id, short_news, long_news, category, image from feed where id = " + id;
	}
	
	public static String sportsQuerry() {
		return "select id, short_news, long_news, category, image from feed where category = 'sports'";
	}
	
	public static String locationsQuerry() {
		return "select id, short_news, long_news, category, image from feed where category = 'locations'";
	}
	
	public static String moviesQuerry() {
		return "select id, short_news, long_news, category, image from feed where category = 'movies'";
	}
	
	public static String musicQuerry() {
		return "select id, short_news, long_news, category, image from feed where category = 'music'";
	}
	
}
