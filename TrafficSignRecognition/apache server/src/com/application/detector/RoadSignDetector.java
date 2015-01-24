package com.application.detector;

public class RoadSignDetector {
	//public final String PATH_TO_PROJECT = getClass().getClassLoader().getResource(".").getPath();;
	//public static final String LIB_NAME = "OpenCVApplication.dll";
	
	static {
		System.load("C:\\Users\\alexandru.dobrii\\HomeWorkspace\\AndroidApplicationServer\\shared_libs\\RoadSignDetector.dll");
	}
	
	public native int detect (long objNativeAddress);
	public native void convert (long objNativeAddress);
}
