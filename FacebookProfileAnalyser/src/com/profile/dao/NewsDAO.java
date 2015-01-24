package com.profile.dao;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import com.profile.models.FeedModel;
import com.profile.models.UserModel;

public class NewsDAO {

	public static List<UserModel> getUsers() {
		ResultSet result = null;
		List<UserModel> users = new ArrayList<UserModel>();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.userQuerry(-1));
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				int id = result.getInt("id");
				String username = result.getString("username");
				String password = result.getString("password");
				users.add(new UserModel(id, username, password));
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return users;
	}
	
	public static List<FeedModel> getAllNews() {
		List<FeedModel> feeds = new ArrayList<FeedModel>();
		feeds.addAll(getMusicNews());
		feeds.addAll(getMoviesNews());
		feeds.addAll(getSportsNews());
		feeds.addAll(getLocationsNews());
		return feeds;
	}
	
	public static FeedModel getNewsFeed (int id) {
		ResultSet result = null;
		FeedModel feed = new FeedModel();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.feedQuerry(id));
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				feed.setId(result.getInt("id"));
				feed.setShortNews(result.getString("short_news"));
				feed.setLongNews(result.getString("long_news"));
				feed.setCategory(result.getString("category"));
				feed.setImage(result.getString("image"));
				
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return feed;
	}
	
	public static List<FeedModel> getSportsNews() {
		ResultSet result = null;
		List<FeedModel> feeds = new ArrayList<FeedModel>();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.sportsQuerry());
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				FeedModel feed = new FeedModel();
				feed.setId(result.getInt("id"));
				feed.setShortNews(result.getString("short_news"));
				feed.setLongNews(result.getString("long_news"));
				feed.setCategory(result.getString("category"));
				feed.setImage(result.getString("image"));
				feeds.add(feed);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return feeds;
	}
	
	public static List<FeedModel> getLocationsNews() {
		ResultSet result = null;
		List<FeedModel> feeds = new ArrayList<FeedModel>();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.locationsQuerry());
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				FeedModel feed = new FeedModel();
				feed.setId(result.getInt("id"));
				feed.setShortNews(result.getString("short_news"));
				feed.setLongNews(result.getString("long_news"));
				feed.setCategory(result.getString("category"));
				feed.setImage(result.getString("image"));
				feeds.add(feed);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return feeds;
	}
	
	public static List<FeedModel> getMusicNews() {
		ResultSet result = null;
		List<FeedModel> feeds = new ArrayList<FeedModel>();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.musicQuerry());
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				FeedModel feed = new FeedModel();
				feed.setId(result.getInt("id"));
				feed.setShortNews(result.getString("short_news"));
				feed.setLongNews(result.getString("long_news"));
				feed.setCategory(result.getString("category"));
				feed.setImage(result.getString("image"));
				feeds.add(feed);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return feeds;
	}
	
	public static List<FeedModel> getMoviesNews() {
		ResultSet result = null;
		List<FeedModel> feeds = new ArrayList<FeedModel>();
		try {
			result = ConnectionManager.getStatement().executeQuery(Querries.moviesQuerry());
			if (result == null) {
				return null;
			}
			
			while (result.next()) {
				FeedModel feed = new FeedModel();
				feed.setId(result.getInt("id"));
				feed.setShortNews(result.getString("short_news"));
				feed.setLongNews(result.getString("long_news"));
				feed.setCategory(result.getString("category"));
				feed.setImage(result.getString("image"));
				feeds.add(feed);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return feeds;
	}
}