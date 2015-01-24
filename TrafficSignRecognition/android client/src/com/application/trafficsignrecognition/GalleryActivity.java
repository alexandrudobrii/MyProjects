package com.application.trafficsignrecognition;

import java.io.InputStream;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.BaseAdapter;
import android.widget.Gallery;
import android.widget.ImageView;
import android.widget.Toast;

@SuppressWarnings("deprecation")
public class GalleryActivity extends Activity {
	private Gallery gallery;
	private ImageView imageView;
	private ServerConnection serverConnection;
	
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
				case LoaderCallbackInterface.SUCCESS: {
					System.loadLibrary("sign_detector");
					break;
				}
				default: {
					super.onManagerConnected(status);
					break;
				}
			}
		}
	};
	
	private Integer[] pics = { R.drawable.antartica1, R.drawable.test, R.drawable.antartica2,
			R.drawable.antartica3, R.drawable.antartica4,
			R.drawable.antartica5, R.drawable.antartica6,
			R.drawable.antartica7, R.drawable.antartica8,
			R.drawable.antartica9, R.drawable.antartica10 };
	
	public native void FindFeatures(long matAddrGr, long matAddrRgba);
	public native void ChangeColor(long matAddrGr, long matAddrRgba);
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gallery);

		serverConnection = new ServerConnection();
		
		gallery = (Gallery) findViewById(R.id.gallery1);
		gallery.setAdapter(new ImageAdapter(this));
		
		imageView = (ImageView) findViewById(R.id.processedImageView1);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_9, this, mLoaderCallback);
		
		gallery.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, final int arg2, long arg3) {
				Toast.makeText(getApplicationContext(), "pic: " + arg2, Toast.LENGTH_SHORT).show();
				
				new Thread(new Runnable() {
					@Override
					public void run() {
						String uri = serverConnection.getURI() + "upload_frame/image_processing";
						InputStream file = getResources().openRawResource(pics[arg2]);
						Utils.executeMultiPartRequest(uri, file, "test_filename.png");
					}
				}).start();
				
//				Mat mat = new Mat();
//				try {
//					mat = Utils.loadResource(GalleryActivity.this, pics[arg2], Highgui.CV_LOAD_IMAGE_COLOR);
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//				
//				FindFeatures(mat.getNativeObjAddr(), mat.getNativeObjAddr());
//				ChangeColor(mat.getNativeObjAddr(), mat.getNativeObjAddr());
//				
//				Bitmap bm = Bitmap.createBitmap(mat.cols(), mat.rows(), Bitmap.Config.ARGB_8888);
//			    Utils.matToBitmap(mat, bm);
//				
//				imageView.setImageResource(pics[arg2]);
			}
		});
	}
	
	public class ImageAdapter extends BaseAdapter {
		private Context context;
		int imageBackground;

		public ImageAdapter(Context context) {
			this.context = context;
		}

		@Override
		public int getCount() {
			return pics.length;
		}

		@Override
		public Object getItem(int arg0) {
			return arg0;
		}

		@Override
		public long getItemId(int arg0) {
			return arg0;
		}

		@Override
		public View getView(int arg0, View arg1, ViewGroup arg2) {
			ImageView imageView = new ImageView(context);
			imageView.setImageResource(pics[arg0]);
			
			return imageView;
		}
	}
}