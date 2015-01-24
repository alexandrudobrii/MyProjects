package com.application.utils;

import java.io.IOException;
import java.util.Properties;

import org.springframework.http.ResponseEntity;
import org.springframework.http.converter.json.MappingJacksonHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

public class ServerConnection {
	private final String CONFIG_FILE = "server.properties";
	private String 						baseUri;	
	private RestTemplate 				connection;
	
	public RestTemplate getConnection() {
		return connection;
	}

	public void setConnection(RestTemplate connection) {
		this.connection = connection;
	}
	
	public String getBaseUri() {
		return baseUri;
	}

	public void setBaseUri(String baseUri) {
		this.baseUri = baseUri;
	}

	public ServerConnection() {
		Properties properties = new Properties();
		try {
			properties.load(ServerConnection.class.getClassLoader().getResourceAsStream(CONFIG_FILE));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		baseUri = (String)properties.get(Constants.BASE_URI_PROPERTY);
		System.setProperty("http.keepAlive", "false");
		
		connection = new RestTemplate();
		connection.getMessageConverters().add(new MappingJacksonHttpMessageConverter());
	}
	
	public ResponseEntity<Object> doGet (String url, Class<Object> responseType) {
		return connection.getForEntity(url, responseType);
	}
	
	public ResponseEntity<Object> doPost (String url, Object request, Class<Object> responseType) {
		return connection.postForEntity(url, request, responseType);
	}
}