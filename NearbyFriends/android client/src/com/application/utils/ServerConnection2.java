package com.application.utils;

import java.io.IOException;
import java.util.Properties;

import org.springframework.http.converter.json.MappingJacksonHttpMessageConverter;
import org.springframework.web.client.RestTemplate;

public class ServerConnection2 {
	private final String CONFIG_FILE = "test.properties";
		
	private String ip;
	private String port;
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

	public String getBaseUri() {
		return protocol + "://" + ip + ":" + port + "/";
	}

	public ServerConnection2() {
		Properties properties = new Properties();
		try {
			properties.load(ServerConnection.class.getClassLoader().getResourceAsStream(CONFIG_FILE));
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		setIp(properties.getProperty("server.ip"));
		setPort(properties.getProperty("server.port"));
		setProtocol(properties.getProperty("application.protocol"));
		
		System.setProperty("http.keepAlive", "false");
		connection = new RestTemplate();
		connection.getMessageConverters().add(new MappingJacksonHttpMessageConverter());
	}
}