package com.application.database.manager;

import java.sql.Types;
import java.util.List;
import java.util.Map;

import com.application.models.User;

public class OracleDatabaseConnection extends DatabaseConnection {
	final static String INSERT_USER_STATEMENT = "insert into users (id, email, password) " +
												"select count(*)+1, ?, ? " +
												"from users";
	
	final static String SELECT_USER_BY_CREDENTIALS = "select count(*) " + 
			  						  				 "from users " +
			  						  				 "where email = ? and password = ?";
	
	final static String SELECT_USER_BY_EMAIL = "select count(*) " + 
				 							   "from users " +
				 							   "where email = ?";

	public OracleDatabaseConnection() {
		super();
	}
	
	public void querryDatabase() {
		List<Map<String, Object>> rows = jdbcTemplate.queryForList(INSERT_USER_STATEMENT);
		for (Map<?, ?> row : rows) {
			System.out.println(String.valueOf(row.get("id")));
			System.out.println(String.valueOf(row.get("email")));
			System.out.println(String.valueOf(row.get("pasword")));
		}
	}
	
	public boolean checkUser (User user) {
		Object[] params = new Object[] {user.getEmail(), user.getPassword()};
        
		if (jdbcTemplate.queryForObject(SELECT_USER_BY_CREDENTIALS, params, Integer.class) == 1) {
			return true;
		}
		
		return false;
	}

	public boolean insertUser (User user) {
		Object[] params = new Object[] {user.getEmail(), user.getPassword()};
        int[] types = new int[] {Types.VARCHAR, Types.VARCHAR};
		
		int count = jdbcTemplate.
				queryForObject(SELECT_USER_BY_EMAIL, new Object[]{user.getEmail()}, Integer.class);
		
		if (count == 0) {
        	jdbcTemplate.update(INSERT_USER_STATEMENT, params, types);
        	return true;
		}
        
		return false;
	}
}