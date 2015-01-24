package com.application.trafficsignrecognition;

import java.io.InputStream;

import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import com.application.detector.RoadSignDetector;
import com.application.utils.AndroidUtils;
import com.application.utils.BitmapManager;
import com.application.utils.Constants;
import com.application.utils.ServerConnection;

public class ResultActivity extends Activity {
	private static final Object lock   = new Object();
	static boolean init_opencv         = false; 
	
    private ImageView                  processedImageView;
	//private ImageView                  initialImageView;
	private View                       progress;
	private RoadSignDetector           roadSignDetector;
	private SharedPreferences          preferences;
	//private ViewAnimator               viewAnimator;
	private BitmapManager              bitmapManager;
	private ServerConnection           serverConnection;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_result);

		preferences = getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		AndroidUtils.setRotation(this, preferences.getBoolean(Constants.ROTATE_SCREEN, false));
		
		serverConnection = new ServerConnection(preferences);
		roadSignDetector = new RoadSignDetector(this, null);
		
		bitmapManager = new BitmapManager(getResources());
		progress = findViewById(R.id.login_status);
		
//		viewAnimator = (ViewAnimator)this.findViewById(R.id.viewFlipper);
//		viewAnimator.setOnClickListener(new OnClickListener() { 
//			@Override
//			public void onClick(View v) { 
//				//AnimationFactory.flipTransition(viewAnimator, FlipDirection.LEFT_RIGHT);
//			}
//        });
//		
//		this.findViewById(R.id.imageView2).setOnClickListener(new OnClickListener() { 
//			@Override
//			public void onClick(View v) { 
//				//AnimationFactory.flipTransition(viewAnimator, FlipDirection.LEFT_RIGHT);
//			}
//        });
//		
//		this.findViewById(R.id.imageView1).setOnClickListener(new OnClickListener() { 
//			@Override
//			public void onClick(View v) { 
//				//AnimationFactory.flipTransition(viewAnimator, FlipDirection.LEFT_RIGHT);
//			}
//        });
		
		processedImageView = (ImageView)findViewById(R.id.result);
		//initialImageView = (ImageView)findViewById(R.id.imageView1);
		showProgress(true);
	
		int position = getIntent().getIntExtra(Constants.POSITION_TAG, -1);
		ImageProcessing imageProcessing = new ImageProcessing(position);
		imageProcessing.execute();
	}

	private void showProgress(final boolean show) {
		progress.setVisibility(show ? View.VISIBLE : View.GONE);
		processedImageView.setVisibility(show ? View.GONE : View.VISIBLE);
		//initialImageView.setVisibility(show ? View.GONE : View.VISIBLE);
		//viewAnimator.setEnabled(!show);
	}

	@Override
	public void onResume() {
		super.onResume();
		init_opencv = OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_9, this, roadSignDetector.getBaseLoaderCallback());
		
		synchronized (lock) {
			lock.notify();
		}
	}

	class ImageProcessing extends AsyncTask<Integer, Void, Bitmap> {
		private int position;
		
		public ImageProcessing (int position) {
			this.position = position;
		}
		
		@Override
		protected Bitmap doInBackground (Integer... params) {
			try {
				Thread.sleep(2000);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
			
			if (preferences.getBoolean(Constants.PROCESS_ON_SERVER, false) == false) {
				Bitmap image = bitmapManager.getImageItems().get(position).getImage();
				Mat mat = AndroidUtils.bitmapToMat(image);
				
				synchronized (lock) {
					while (init_opencv == false) {
						try {
							lock.wait();
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
					}
				}
				
				//roadSignDetector.findFeatures(mat.getNativeObjAddr(), mat.getNativeObjAddr());
				roadSignDetector.detect(mat.getNativeObjAddr(), mat.getNativeObjAddr());
				
				return AndroidUtils.matToBitmap(mat);
			} else {
				String uri = serverConnection.getURI() + "upload_frame/image_processing";
				InputStream file = getResources().openRawResource(bitmapManager.getResIds().get(position));
				byte[] response = AndroidUtils.executeMultiPartRequest(uri, file, "test_filename.png");
				
				return AndroidUtils.byteArrayToBitmap(response);
			}
		}
		
		@Override
		protected void onPostExecute (final Bitmap bitmap) {
			//processedImageView.setImageBitmap(BitmapManager.scaleBitmap(bitmap, 250, 250));
			processedImageView.setImageBitmap(BitmapManager.rotateBitmap(bitmap, 270));
			//initialImageView.setImageBitmap(bitmapManager.getImageItems().get(position).getImage());
			
			showProgress(false);
		}

		@Override
		protected void onCancelled() {
			showProgress(false);
		}
	}
}