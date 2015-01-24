package com.application.database.manager;

import java.io.IOException;
import java.util.Properties;

import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.SingleConnectionDataSource;

public class DatabaseConnection {
	private final String CONFIG_FILE = "jdbc.properties";
	protected JdbcTemplate jdbcTemplate;
	
	public DatabaseConnection() {
		SingleConnectionDataSource dataSource = new SingleConnectionDataSource();
		
		Properties properties = new Properties();
		try {
			properties.load(DatabaseConnection.class.
							getClassLoader().getResourceAsStream(CONFIG_FILE));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		dataSource.setDriverClassName(properties.getProperty("driver.classname"));
		dataSource.setUrl(properties.getProperty("db.url"));
		dataSource.setUsername(properties.getProperty("db.username"));
		dataSource.setPassword(properties.getProperty("db.password"));
		
		jdbcTemplate = new JdbcTemplate(dataSource);
	}
}