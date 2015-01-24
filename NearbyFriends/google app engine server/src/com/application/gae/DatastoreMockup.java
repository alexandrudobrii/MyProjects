package com.application.gae;

import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Response;

import com.application.datastore.DatastoreManager;
import com.application.location.models.UserDetails;

@Path("/mockup")
public class DatastoreMockup {
	
	@GET
	@Path("/users")
	@Produces("application/json")
	public Response checkUser2 (@PathParam("id") Integer id) {
		return Response.status(200).entity(DatastoreManager.getActiveUsers(id.toString())).build();
	}
	
	@GET
	@Path("/insert_all")
	@Produces("application/json")
	public Response insertData () {
		List<UserDetails> locations = generateLocations();
		for (int i = 0; i < locations.size(); i++) {
			DatastoreManager.insertUser(locations.get(i));
		}
		return Response.status(200).entity(DatastoreManager.getActiveUsers("")).build();
	}
	
	@GET
	@Path("/insert_single1")
	@Produces("application/json")
	public Response insertSingleUser1() {
		UserDetails users = new UserDetails();
		users.setId(6+"");
		users.setDescription("Near me1!");
		users.setName("Near me1!");
		users.setEnabled(true);
		users.setLatitude(44.444639);
		users.setLongitude(26.034265);
		DatastoreManager.insertUser(users);
	   return Response.status(200).entity(users.toString()).build();
	}
	
	@GET
	@Path("/insert_single2")
	@Produces("application/json")
	public Response insertSingleUser2() {
		UserDetails users = new UserDetails();
		users.setId(7+"");
		users.setDescription("Near me2!");
		users.setName("Near me2!");
		users.setEnabled(false);
		users.setLatitude(44.445470);
		users.setLongitude(26.050101);
		DatastoreManager.insertUser(users);
		return Response.status(200).entity(users.toString()).build();
	}
	
	public static List<UserDetails> generateLocations() {
		List<UserDetails> list = new ArrayList<UserDetails>();
		
		UserDetails users = new UserDetails();
		users.setId("509585803");
		users.setDescription("Traian Rebedea");
		users.setName("Traian Rebedea");
		users.setEnabled(true);
		users.setLatitude(45.248491);
		users.setLongitude(25.635721);
		list.add(users);
		
		users = new UserDetails();
		users.setId("702191702");
		users.setDescription("Alex Eftimie");
		users.setName("Alex Eftimie");
		users.setEnabled(true);
		users.setLatitude(45.248057);
		users.setLongitude(25.637717);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1026332971");
		users.setDescription("Andrei Gliga");
		users.setName("Andrei Gliga");
		users.setEnabled(false);
		users.setLatitude(45.258001);
		users.setLongitude(25.629843);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1131754860");
		users.setDescription("Iulian Lazar");
		users.setName("Iulian Lazar");
		users.setEnabled(true);
		users.setLatitude(45.246678);
		users.setLongitude(25.636784);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1219985101");
		users.setDescription("Iulia Manda");
		users.setName("Iulia Manda");
		users.setEnabled(true);
		users.setLatitude(45.247732);
		users.setLongitude(25.637717);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1338671909");
		users.setDescription("Razvan Mihai Tache");
		users.setName("Razvan Mihai Tache");
		users.setEnabled(false);
		users.setLatitude(44.437496);
		users.setLongitude(26.047816);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1340276514");
		users.setDescription("Vlad Ploesteanu");
		users.setName("Vlad Ploesteanu");
		users.setEnabled(false);
		users.setLatitude(44.445524);
		users.setLongitude(25.960697);
		list.add(users);
		
		users = new UserDetails();
		users.setId("1461834155");
		users.setDescription("Marian Ciovirta");
		users.setName("Marian Ciovirta");
		users.setEnabled(true);
		users.setLatitude(44.372665);
		users.setLongitude(26.028442);
		list.add(users);
		
		users = new UserDetails();
		users.setId("10201881881967036");
		users.setDescription("Catalin Dinca");
		users.setName("Catalin Dinca");
		users.setEnabled(false);
		users.setLatitude(44.435942);
		users.setLongitude(26.046520);
		list.add(users);
		
		users = new UserDetails();
		users.setId("100000969503369");
		users.setDescription("Andrei Cristian Pavel");
		users.setName("Andrei Cristian Pavel");
		users.setEnabled(true);
		users.setLatitude(44.436432);
		users.setLongitude(26.045533);
		list.add(users);
		
		users = new UserDetails();
		users.setId("100001468769088");
		users.setDescription("Ionita Mircea");
		users.setName("Ionita Mircea");
		users.setEnabled(false);
		users.setLatitude(44.444915);
		users.setLongitude(26.053824);
		list.add(users);
		
		return list;
	}
}