package com.application.adapters;

import java.util.List;

import android.app.Activity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.application.trafficsignrecognition.R;

public class LogsAdaptorListView extends BaseAdapter {
	private List<String>         logsList;
	private LayoutInflater       layoutInflater;
	
	public LogsAdaptorListView (Activity context, List<String> logsList) {
		this.logsList = logsList;
		layoutInflater = (LayoutInflater)context.getLayoutInflater();
	}
	
	@Override
	public int getCount() {
		return logsList.size();
	}

	@Override
	public Object getItem(int position) {
		return logsList.get(position);
	}

	@Override
	public long getItemId(int position) {
		return 0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		View row = convertView;
		ViewHolder holder = null;
		String action = logsList.get(position);
		
		if (row == null) {
			row = layoutInflater.inflate(R.layout.activity_logs_even, parent, false);
		    
			holder = new ViewHolder();
			holder.coordonatesView = (TextView)row.findViewById(R.id.action);
			holder.coordonatesView.setText(action);
	        
			row.setTag(holder);
		} else {
			holder = (ViewHolder) row.getTag();
			holder.coordonatesView.setText(action);
		}

		return row;
	}
	
	static class ViewHolder {
		TextView coordonatesView;
		TextView dateView;
		ImageView imageView;
	}
}
