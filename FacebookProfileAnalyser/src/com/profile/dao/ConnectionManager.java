package com.profile.dao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;

public class ConnectionManager {
	private static Connection connection = null;
	private static Statement statement = null;

	public static Connection getConnection() {
		if (connection != null) {
			return connection;
		}
		
		try {
			String url = "jdbc:mysql://localhost:3306/";
			String dbName = "profileanalyser"; 
			String uname = "root";
			//String pwd = "headdy1234";

			Class.forName("com.mysql.jdbc.Driver");
			try {
				connection = DriverManager.getConnection(url + dbName, uname, "");
			} catch (SQLException ex) {
				ex.printStackTrace();
			}
		} catch (ClassNotFoundException e) {
			System.out.println(e);
		}
		return connection;
	}
	
	public static Statement getStatement() {
		if (connection == null) {
			connection = ConnectionManager.getConnection();
		}
		
		try {
			statement = connection.createStatement();
		} catch (SQLException e) {
			System.out.println("ERROR OCCURRED WHILE CREATING STATEMENT " + e.getMessage());
		}
		
		return statement;
	}
}