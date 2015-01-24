package com.application.server.rest;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MultivaluedMap;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriInfo;

import org.jboss.resteasy.plugins.providers.multipart.InputPart;
import org.jboss.resteasy.plugins.providers.multipart.MultipartFormDataInput;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.highgui.Highgui;

import com.application.detector.RoadSignDetector;
import com.application.models.User;
import com.application.utils.MatUtils;

@Path("/upload_frame")
public class FrameProcessModule {
	public static final String UPLOADED_FILE_PATH = "C:\\temp\\";
	public static int counter = 0;
	
	@GET
	@Path("/users/{id}")
	@Produces("application/json")
	public Response getUserById(@PathParam("id") Integer id) {
		User user = new User();
		
		return Response.status(200).entity(user).build();
	}
	
	@POST
	@Path("/process")
	@Produces("application/json")
	public Response processFrame (@Context UriInfo uriInfo, String frame) {
		System.out.println("frame = " + frame);
		
		return Response.status(200).entity(true).build();
	}
	
	@POST
	@Path("/image_processing")
	@Consumes("multipart/form-data")
	public Response uploadFile(MultipartFormDataInput input) throws IOException {
		int size = 0;
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		
		//Get API input data
		Map<String, List<InputPart>> uploadForm = input.getFormDataMap();
		
		//Get file name
		//String fileName = uploadForm.get("fileName").get(0).getBodyAsString();
		
		//Get file data to save
		List<InputPart> inputParts = uploadForm.get("attachment");

		byte[] response = null;
		for (InputPart inputPart : inputParts) {
			try  {
				//Use this header for extra processing if required
				@SuppressWarnings("unused")
				MultivaluedMap<String, String> header = inputPart.getHeaders();
				
				// convert the uploaded file to inputstream
				InputStream inputStream = inputPart.getBody(InputStream.class, null);
				System.out.println("Frame uploaded successfully !!!!!");
				
				Mat mat = MatUtils.readInputStreamIntoMat(inputStream);
				
				RoadSignDetector trafficSignDetector = new RoadSignDetector();
				//trafficSignDetector.convert(mat.getNativeObjAddr());
				size = trafficSignDetector.detect(mat.getNativeObjAddr());
				
				response = MatUtils.readMatIntoByteArray(mat);
				if (size != 0) {
					Highgui.imwrite(UPLOADED_FILE_PATH + (counter + "test.jpg"), mat);
					counter++;
				}
			} 
			catch (Exception e)  {
				e.printStackTrace();
			}
		}
		
		return Response.status(200).entity(response).build();
	}
}