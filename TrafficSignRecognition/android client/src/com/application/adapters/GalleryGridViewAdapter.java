package com.application.adapters;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.application.trafficsignrecognition.R;
import com.application.trafficsignrecognition.ResultActivity;
import com.application.trafficsignrecognition.UserActivity;
import com.application.utils.Constants;
import com.application.utils.ImageItem;

public class GalleryGridViewAdapter extends ArrayAdapter<ImageItem> {
	private int layoutResourceId;
	private List<ImageItem> data = new ArrayList<ImageItem>();
	private LayoutInflater inflater;
	private Context context;

	public GalleryGridViewAdapter(Context context, int layoutResourceId, List<ImageItem> data) {
		super(context, layoutResourceId, data);
		this.layoutResourceId = layoutResourceId;
		this.data = data;
		this.context = context;
		this.inflater = LayoutInflater.from(context);
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View row = convertView;
		ViewHolder holder = null;

		if (row == null) {
			row = inflater.inflate(layoutResourceId, parent, false);
			holder = new ViewHolder();
			holder.imageTitle = (TextView) row.findViewById(R.id.text);
			holder.image = (ImageView) row.findViewById(R.id.image);
			holder.image.setOnClickListener(new ImageViewAdapter(position));
			
			row.setTag(holder);
		} else {
			holder = (ViewHolder) row.getTag();
			holder.image.setOnClickListener(new ImageViewAdapter(position));
		}

		ImageItem item = data.get(position);
		holder.imageTitle.setText(item.getTitle());
		holder.image.setImageBitmap(item.getImage());
		return row;
	}

	static class ViewHolder {
		TextView imageTitle;
		ImageView image;
	}
	
	class ImageViewAdapter implements View.OnClickListener {
		private int position;
		
		public ImageViewAdapter (int position) {
			this.position = position;
		}

		@Override
		public void onClick(View v) {
			Log.d("USER", "Image " + position + " was processed!");
			
			if (UserActivity.logsList != null) {
				UserActivity.logsList.add("Image " + position + " was processed!");
				UserActivity.logsAdaptorListView.notifyDataSetChanged();
			}
			
			Intent intent = new Intent(context, ResultActivity.class);
			intent.putExtra(Constants.POSITION_TAG, position);
			context.startActivity(intent);
		}
	}
}