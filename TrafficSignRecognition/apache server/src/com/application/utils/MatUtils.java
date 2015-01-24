package com.application.utils;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import org.apache.commons.io.IOUtils;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;
import org.opencv.highgui.Highgui;

public abstract class MatUtils {

	public static void writeFile (byte[] content, String filename) throws IOException {
		File file = new File(filename);
		if (!file.exists()) {
			file.createNewFile();
		}
		FileOutputStream fop = new FileOutputStream(file);
		fop.write(content);
		fop.flush();
		fop.close();
	}
	
	public static Mat byteArrayToMat (byte[] array) {
		return Highgui.imdecode(new MatOfByte(array), Highgui.IMREAD_COLOR);
	}
	
	public static InputStream byteArrayToInputstream (byte[] array) {
		return new ByteArrayInputStream(array);
	}

	public static Mat readInputStreamIntoMat (InputStream inputStream) throws IOException {
		byte[] temporaryImageInMemory = IOUtils.toByteArray(inputStream);
		
		// Decode into mat. Use any IMREAD_ option that describes your image appropriately
		Mat outputImage = Highgui.imdecode(new MatOfByte(temporaryImageInMemory), Highgui.IMREAD_COLOR);

		return outputImage;
	}
	
	public static byte[] readMatIntoByteArray (Mat mat) {
		MatOfByte bytemat = new MatOfByte();
		Highgui.imencode(".jpg", mat, bytemat);
		//InputStream in = new ByteArrayInputStream(bytes);
		
		return bytemat.toArray();
	}
	
	public static int safeLongToInt(long l) {
	    if (l < Integer.MIN_VALUE || l > Integer.MAX_VALUE) {
	        throw new IllegalArgumentException
	            (l + " cannot be cast to int without changing its value.");
	    }
	    return (int) l;
	}
}