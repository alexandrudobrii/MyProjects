package com.application.trafficsignrecognition;

import java.io.ByteArrayInputStream;
import java.io.InputStream;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;
import org.opencv.highgui.Highgui;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.WindowManager;

import com.application.detector.RoadSignDetector;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.ServerConnection;

public class CameraActivity extends Activity implements CvCameraViewListener2 {
	//private static final Object lock = new Object();
	
	private CameraBridgeViewBase    mOpenCvCameraView;
	private ServerConnection        serverConnection;
	private RoadSignDetector        roadSignDetector;
	private SharedPreferences       preferences;

	private Mat                     mRgba;
	private Mat                     mGray;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(Constants.CAMERA_TAG, "called onCreate");
		super.onCreate(savedInstanceState);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.activity_camera);
		
		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.fd_activity_surface_view);
		mOpenCvCameraView.setCvCameraViewListener(this);
		
		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		serverConnection = new ServerConnection(preferences);
		roadSignDetector = new RoadSignDetector(this, mOpenCvCameraView);
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mOpenCvCameraView != null) {
			mOpenCvCameraView.disableView();
		}
	}

	@Override
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_9, this, roadSignDetector.getBaseLoaderCallback());
	}

	public void onDestroy() {
		super.onDestroy();
		mOpenCvCameraView.disableView();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public void onCameraViewStarted(int width, int height) {
		mRgba = new Mat(height, width, CvType.CV_8UC4);
		mGray = new Mat(height, width, CvType.CV_8UC1);
	}

	public void onCameraViewStopped() {
		mRgba.release();
		mGray.release();
	}

	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();
		
		if (preferences.getBoolean(Constants.PROCESS_ON_SERVER, false) == false) {
			roadSignDetector.detect(mRgba.getNativeObjAddr(), mGray.getNativeObjAddr());
		} else {
//			final Mat copy = mRgba.clone();
//			mRgba = null;
			
			new Thread(new Runnable() {
				@Override
				public void run() {
					MatOfByte bytemat = new MatOfByte();
					Highgui.imencode(".jpg", mRgba, bytemat);
					
					InputStream file = new ByteArrayInputStream(bytemat.toArray());
					String uri = serverConnection.getURI() + "upload_frame/image_processing";
					
					AndroidUtils.executeMultiPartRequest(uri, file, "");
//					mRgba = AndroidUtils.byteArrayToMat(response);
//					
//					synchronized (lock) {
//						lock.notify();
//					}
				}
			}).start();
			
//			synchronized (lock) {
//				while (mRgba == null) {
//					try {
//						lock.wait();
//					} catch (InterruptedException e) {
//						e.printStackTrace();
//					}
//				}
//			}
		}
		
		return mRgba;
	}
}