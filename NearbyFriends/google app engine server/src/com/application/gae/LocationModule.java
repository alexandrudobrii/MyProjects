package com.application.gae;

import java.io.File;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;

import com.application.datastore.DatastoreManager;
import com.application.location.models.UserDetails;

@Path("/locations")
public class LocationModule {
	
	@GET
	@Path("/users")
	@Produces("application/json")
	public Response checkUser2 (@PathParam("id") Integer userId) {
		return Response.status(200).entity(DatastoreManager.getActiveUsers(userId.toString())).build();
	}
	
	@POST
	@Path("/gps")
	@Produces("application/json")
	public Response insertAndReturnUsers (@Context UriInfo uriInfo, UserDetails friendData) {
		DatastoreManager.insertUser(friendData);
		return Response.status(200).entity(DatastoreManager.getActiveUsers(friendData.getId())).build();
	}
	
	@POST
	@Path("/gps_radius")
	@Produces("application/json")
	public Response insertAndReturnUsersByRadius (@Context UriInfo uriInfo, UserDetails friendData) {
		DatastoreManager.insertUser(friendData);
		return Response.status(200).entity(DatastoreManager.getUserByRadius(friendData)).build();
	}
	
	@GET
	@Produces(MediaType.APPLICATION_OCTET_STREAM)
	public Response getFile() {
		File file = new File(""); // Initialize this to the File path you want to serve.
		return Response.ok(file, MediaType.APPLICATION_OCTET_STREAM)
	      .header("Content-Disposition", "attachment; filename=\"" + file.getName() + "\"" ) //optional
	      .build();
	}
}