package com.application.gae;

import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;

@Path("/user-management")
public class ReferenceModule {
	
	@GET
	@Path("/users/{id}")
	@Produces("application/json")
	public Response getUserById(@PathParam("id") Integer id) {
		return Response.status(200).entity("GET_"+id).build();
	}
	
	@POST
	@Path("/sign_in")
	@Produces("application/json")
	public Response checkUser (@Context UriInfo uriInfo, Object object) {
		// TODO - process object
		return Response.status(200).entity("POST").build();
	}
	
	@GET
	@Path("{year}/{month}/{day}")
	public Response getUserHistory(
			@PathParam("year") int year,
			@PathParam("month") int month, 
			@PathParam("day") int day) {
 
	   String date = year + "/" + month + "/" + day;
 
	   return Response.status(200)
		.entity("getUserHistory is called, year/month/day : " + date)
		.build();
	}
}