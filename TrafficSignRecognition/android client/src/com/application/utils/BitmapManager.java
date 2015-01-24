package com.application.utils;

import java.util.ArrayList;
import java.util.List;

import org.opencv.android.Utils;
import org.opencv.core.Mat;

import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;

import com.application.trafficsignrecognition.R;

public class BitmapManager {
	private static List<ImageItem>    imageItems = null;
	private static List<Integer>      resIds = null;

	public BitmapManager (Resources resources) {
		if (imageItems == null) {
			imageItems = new ArrayList<ImageItem>();
			resIds = new ArrayList<Integer>();
			setDataImages(resources);
		}
	}
	
	public List<ImageItem> getImageItems() {
		return imageItems;
	}
	
	public List<Integer> getResIds() {
		return resIds;
	}
	
	public static int calculateInSampleSize (BitmapFactory.Options options, int reqWidth, int reqHeight) {
		// Raw height and width of image
		final int height = options.outHeight;
		final int width = options.outWidth;
		int inSampleSize = 1;

		if (height > reqHeight || width > reqWidth) {
			final int halfHeight = height / 2;
			final int halfWidth = width / 2;

			// Calculate the largest inSampleSize value that is a power of 2 and
			// keeps both
			// height and width larger than the requested height and width.
			while ((halfHeight / inSampleSize) > reqHeight && (halfWidth / inSampleSize) > reqWidth) {
				inSampleSize *= 2;
			}
		}

		return inSampleSize;
	}

	public static Bitmap decodeResource (Resources res, int resId, int reqWidth, int reqHeight) {
		
		// First decode with inJustDecodeBounds=true to check dimensions
	    final BitmapFactory.Options options = new BitmapFactory.Options();
	    options.inJustDecodeBounds = true;
	    BitmapFactory.decodeResource(res, resId, options);

	    // Calculate inSampleSize
	    options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight);

	    // Decode bitmap with inSampleSize set
	    options.inJustDecodeBounds = false;
	    return BitmapFactory.decodeResource(res, resId, options);
	}
	
	public void setDataImages (Resources resources) {
		TypedArray imgs = resources.obtainTypedArray(R.array.image_ids);
		for (int i = 0; i < imgs.length(); i++) {
			Bitmap bitmap = decodeResource(resources, imgs.getResourceId(i, -1), 100, 100);
			imageItems.add(new ImageItem(bitmap, "Image#" + i));
			resIds.add(imgs.getResourceId(i, -1));
		}

		if (imgs != null) {
			imgs.recycle();
			imgs = null;
		}
	}

	public Bitmap matToBitmap (Mat src) {
		Bitmap bdst = Bitmap.createBitmap(src.cols(), src.rows(), Bitmap.Config.ARGB_8888); 
		Utils.matToBitmap(src, bdst); 
		
		return bdst;  
	}
	
	public void free (Bitmap bitmap) {
		if(bitmap != null) {
			bitmap.recycle();
			bitmap = null;
		}
	}
	
	public static Bitmap scaleBitmap (Bitmap source, int width, int height) {
		return Bitmap.createScaledBitmap(source, width, height, true);
	}
	
	public static Bitmap rotateBitmap (Bitmap source, float angle) {
	      Matrix matrix = new Matrix();
	      matrix.postRotate(angle);
	     
	      return Bitmap.createBitmap(source, 0, 0, source.getWidth(), source.getHeight(), matrix, true);
	}
}
