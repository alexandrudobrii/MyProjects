package com.application.trafficsignrecognition;

import java.io.InputStream;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.mime.MultipartEntity;
import org.apache.http.entity.mime.content.InputStreamBody;
import org.apache.http.entity.mime.content.StringBody;
import org.apache.http.impl.client.DefaultHttpClient;

import android.util.Log;

public abstract class Utils {

	public static void executeMultiPartRequest(String urlString, InputStream file, String fileName) {
    	HttpClient client = new DefaultHttpClient();
        HttpPost postRequest = new HttpPost(urlString);
        
        try {
        	//Set various attributes 
            MultipartEntity multiPartEntity = new MultipartEntity();
            multiPartEntity.addPart("fileName", new StringBody(fileName));
 
            // prepare payload
            InputStreamBody fileBody = new InputStreamBody(file, "application/octect-stream");
            multiPartEntity.addPart("attachment", fileBody);
 
            //Set to request body
            postRequest.setEntity(multiPartEntity);
            
           //Send request
            HttpResponse response = client.execute(postRequest);
            
            //Verify response if any
            if (response != null) {
                Log.d("REST", response.getStatusLine().getStatusCode()+"");
            }
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}