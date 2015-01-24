package com.application.datastore;

import java.util.ArrayList;
import java.util.List;

import com.application.location.models.Constants;
import com.application.location.models.LocationsUtil;
import com.application.location.models.UserDetails;
import com.google.appengine.api.datastore.DatastoreService;
import com.google.appengine.api.datastore.DatastoreServiceFactory;
import com.google.appengine.api.datastore.Entity;
import com.google.appengine.api.datastore.EntityNotFoundException;
import com.google.appengine.api.datastore.FetchOptions;
import com.google.appengine.api.datastore.Key;
import com.google.appengine.api.datastore.KeyFactory;
import com.google.appengine.api.datastore.Query;
import com.google.appengine.api.datastore.Query.FilterOperator;

public abstract class DatastoreManager {
	public static final String DATASTORE_NAME 		= "USERS";
	
	public static final String USER_ID 					= "USER_ID";
	public static final String USER_LONGITUDE 			= "USER_LONGITUDE";
	public static final String USER_LATITUDE 			= "USER_LATITUDE";
	public static final String USER_FOCUS 				= "USER_FOCUS";
	public static final String USER_ENABLED 			= "USER_ENABLED";
	public static final String USER_NAME 				= "USER_NAME";
	public static final String USER_DESCRIPTION 		= "USER_DESCRIPTION";
	public static final String USER_NOTIFICATION		= "USER_NOTIFICATION";
	
	private static Entity populateEntity (Key key, UserDetails user) {
		Entity entity = new Entity(key);
		entity.setProperty(USER_ID, user.getId());
		entity.setProperty(USER_LONGITUDE, user.getLongitude());
		entity.setProperty(USER_LATITUDE, user.getLatitude());
		entity.setProperty(USER_FOCUS, user.isRequestFocus());
		entity.setProperty(USER_ENABLED, user.isEnabled());
		entity.setProperty(USER_NAME, user.getName());
		entity.setProperty(USER_DESCRIPTION, user.getDescription());
		entity.setProperty(USER_NOTIFICATION, user.getNotification());
		return entity;
	}
	
	private static UserDetails populateUser (Entity entity) {
		UserDetails user = new UserDetails();
		user.setId((String)entity.getProperty(USER_ID));
		user.setLatitude((Double)entity.getProperty(USER_LATITUDE));
		user.setLongitude((Double)entity.getProperty(USER_LONGITUDE));
		user.setName((String)entity.getProperty(USER_NAME));
		user.setDescription((String)entity.getProperty(USER_DESCRIPTION));
		user.setEnabled((Boolean)entity.getProperty(USER_ENABLED));
		user.setRequestFocus((Boolean)entity.getProperty(USER_FOCUS));
		user.setNotification((String)entity.getProperty(USER_NOTIFICATION));
		return user;
	}
	
	public static List<Entity> populateEntities (Key key, List<UserDetails> users) {
		List<Entity> entities = new ArrayList<Entity>();
		for (UserDetails user : users) {
			entities.add(populateEntity(key, user));
		}
		return entities;
	}
	
//	private static List<FriendData> populateUsers (List<Entity> entities) {
//		List<FriendData> users = new ArrayList<FriendData>();
//		for (Entity entity : entities) {
//			users.add(populateUser(entity));
//		}
//		return users;
//	}
	
	public static void insertUser (UserDetails friendData) {
		Key userKey = KeyFactory.createKey(DATASTORE_NAME, friendData.getId()+"");
		DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
		
		try {
			// delete the old key
			datastore.get(userKey);
			
//		    Entity entity = datastore.get(userKey);
//		    UserDetails userDetails = populateUser(entity);
		    
//		    if  (friendData.isEnabled() != userDetails.isEnabled() && friendData.isEnabled() == true) {
//		    	friendData.setNotification(friendData.getName() + " is now online");
//		    } else if (friendData.isEnabled() != userDetails.isEnabled() && friendData.isEnabled() == false) {
//		    	friendData.setNotification(friendData.getName() + " went offline");
//		    } else if (friendData.isEnabled() == userDetails.isEnabled()) {
//		    	friendData.setNotification("");
//		    }
		    
//		    Key[] keys = new Key[] { userKey };
//		    datastore.delete(keys);
		} catch (EntityNotFoundException e) {
			System.out.println("User " + friendData.getName() + " was not found in the datastore");
//			if (friendData.isEnabled() == true) {
//				friendData.setNotification(friendData.getName() + " is now online");
//			} else {
//				friendData.setNotification("");
//			}
		}
		
		// add new key
		datastore.put(populateEntity(userKey, friendData));
		System.out.println("The entity " + friendData.getId() + " was successfully inserted in the datastore");
	}
	
	@SuppressWarnings("deprecation")
	public static String getActiveUsers (String userId) {
		Query findActiveUsersQuery = new Query(DATASTORE_NAME);
		findActiveUsersQuery.addFilter(USER_ID, FilterOperator.NOT_EQUAL, userId);
		
		String users = "";
		DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
		
		List<Entity> usersEntity = datastore.prepare(findActiveUsersQuery).asList(FetchOptions.Builder.withDefaults());
		System.out.println(usersEntity.size());
		
		for (int i = 0; i < usersEntity.size(); i++) {
			users += populateUser(usersEntity.get(i)).toString();
			if (i != usersEntity.size()-1) {
				users += ";";
			}
		}
		
		return users;
	}
	
	@SuppressWarnings("deprecation")
	public static String getUserByRadius (UserDetails userDetails) {
		Query findActiveUsersQuery = new Query(DATASTORE_NAME);
		findActiveUsersQuery.addFilter(USER_ID, FilterOperator.NOT_EQUAL, userDetails.getId());
		
		String users = "";
		DatastoreService datastore = DatastoreServiceFactory.getDatastoreService();
		List<Entity> usersEntity = datastore.prepare(findActiveUsersQuery).asList(FetchOptions.Builder.withDefaults());
		
		for (int i = 0; i < usersEntity.size(); i++) {
			UserDetails other = populateUser(usersEntity.get(i));
			float distance = LocationsUtil.getDistance(userDetails, other);
			if (other.isEnabled() == false) {
				other.setStatus(Constants.USER_OFFLINE);
				//actualNotification = other.getName() + " is " + Constants.USER_OFFLINE;
			} else if (other.isEnabled() && userDetails.getRadius() != 0.0f && distance > userDetails.getRadius()) {
				other.setStatus(Constants.USER_NOT_IN_RANGE);
				//actualNotification = other.getName() + " is " + Constants.USER_NOT_IN_RANGE;
				other.setEnabled(false);
			} else if (other.isEnabled() && (userDetails.getRadius() == 0.0f || distance <= userDetails.getRadius())) {
				other.setStatus(Constants.USER_ONLINE);
				//actualNotification = other.getName() + " is " + Constants.USER_ONLINE;
			}
			//other.setNotification(updateAndSendNotifications(actualNotification, other));
			
			users += other.toString();
			if (i != usersEntity.size()-1) {
				users += ";";
			}
		}
		
		return users;
	}
}