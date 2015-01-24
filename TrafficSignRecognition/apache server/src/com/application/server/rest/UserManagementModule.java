package com.application.server.rest;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;

import com.application.database.manager.OracleDatabaseConnection;
import com.application.models.User;

@Path("/user-management")
public class UserManagementModule {
	@GET
	@Path("/users/{id}")
	@Produces("application/json")
	public Response getUserById(@PathParam("id") Integer id) {
		User user = new User();
		
		return Response.status(200).entity(user).build();
	}
	
	@POST
	@Path("/sign_in")
	@Produces("application/json")
	public Response checkUser (@Context UriInfo uriInfo, User user) {
		OracleDatabaseConnection databaseConnection = new OracleDatabaseConnection();
		
		return Response.status(200).entity(databaseConnection.checkUser(user)).build();
	}
	
	@POST
	@Path("/sign_up")
	@Produces("application/json")
	public Response addUser (@Context UriInfo uriInfo, User user) {
		OracleDatabaseConnection databaseConnection = new OracleDatabaseConnection();
		
		return Response.status(200).entity(databaseConnection.insertUser(user)).build();
	}
}