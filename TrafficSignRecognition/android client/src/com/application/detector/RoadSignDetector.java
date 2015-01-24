package com.application.detector;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;

import com.application.utils.Constants;

import android.app.Activity;
import android.util.Log;

public class RoadSignDetector {
	private Activity activity;
	private CameraBridgeViewBase mOpenCvCameraView;
	
	public native void findFeatures (long matAddrGr, long matAddrRgba);
	public native void detect (long matAddrGr, long matAddrRgba); 
	
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(activity) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
				case LoaderCallbackInterface.SUCCESS: {
					Log.i(Constants.CAMERA_TAG, "OpenCV loaded successfully");
					System.loadLibrary("sign_detector");
					
					if (mOpenCvCameraView != null) {
						mOpenCvCameraView.enableView();
					}
					
					break;
				}
				default: {
					super.onManagerConnected(status);
					break;
				}
			}
		}
	};
	
	public RoadSignDetector (Activity activity, CameraBridgeViewBase mOpenCvCameraView) {
		this.activity = activity;
		this.mOpenCvCameraView = mOpenCvCameraView;
	}
	
	public BaseLoaderCallback getBaseLoaderCallback() {
		return mLoaderCallback;
	}
}
