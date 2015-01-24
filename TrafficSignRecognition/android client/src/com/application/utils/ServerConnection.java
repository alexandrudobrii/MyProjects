package com.application.utils;

import java.io.IOException;
import java.util.Properties;

import org.springframework.http.converter.json.MappingJacksonHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

import android.content.SharedPreferences;

public class ServerConnection {
	private final String CONFIG_FILE = "server.properties";
		
	private String ip;
	private String port;
	private String name;
	private String protocol;
	
	private RestTemplate connection;
	
	public String getIp() {
		return ip;
	}

	public void setIp(String ip) {
		this.ip = ip;
	}

	public String getPort() {
		return port;
	}

	public void setPort(String port) {
		this.port = port;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public String getProtocol() {
		return protocol;
	}

	public void setProtocol(String protocol) {
		this.protocol = protocol;
	}
	
	public RestTemplate getConnection() {
		return connection;
	}

	public void setConnection(RestTemplate connection) {
		this.connection = connection;
	}

	public String getURI() {
		return protocol + "://" + ip + ":" + port + "/" + name + "/";
	}

	public ServerConnection (SharedPreferences preferences) {
		Properties properties = new Properties();
		try {
			properties.load(ServerConnection.class.getClassLoader().getResourceAsStream(CONFIG_FILE));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		String ip = preferences.getString(Constants.SERVER_IP_TAG, properties.getProperty("server.ip"));
		String port = preferences.getString(Constants.SERVER_PORT_TAG, properties.getProperty("server.port"));
		
		setIp(ip);
		setPort(port);
		setName(properties.getProperty("server.name"));
		setProtocol(properties.getProperty("application.protocol"));
		
		System.setProperty("http.keepAlive", "false");
		connection = new RestTemplate();
		connection.getMessageConverters().add(new MappingJacksonHttpMessageConverter());
	}
}