package com.application.adapters;

import java.util.List;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.application.notifications.NotificationsModel;
import com.application.social.maps.R;

public class NotificationsListAdapter extends BaseAdapter {
	private List<NotificationsModel> 	notificationsValues;
	private LayoutInflater				layoutInflater;
	
	public NotificationsListAdapter (Activity context, List<NotificationsModel> notificationsValues) {
		this.notificationsValues = notificationsValues;
		this.layoutInflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
	@Override
	public int getCount() {
		return notificationsValues.size();
	}

	@Override
	public NotificationsModel getItem (int position) {
		return notificationsValues.get(position);
	}

	@Override
	public long getItemId (int position) {
		return position;
	}

	@SuppressLint("InflateParams") @Override
	public View getView (int position, View convertView, ViewGroup parent) {
		 View view = convertView;
		 SettingsHolder viewHolder;
         if (convertView == null) {
        	 view = layoutInflater.inflate(R.layout.notifications_list_layout, null);
        	 viewHolder = new SettingsHolder(view);
        	 view.setTag(viewHolder);
         } else {
              viewHolder = (SettingsHolder) view.getTag();
         } 
         viewHolder.userImageView.setImageResource(R.drawable.icon_map);
         viewHolder.notificationView.setText(notificationsValues.get(position).getNotification());
         viewHolder.dateView.setText(notificationsValues.get(position).getDate());
         return view;
	}
	
	static class SettingsHolder {
		public ImageView userImageView;
		public TextView notificationView;
		public TextView dateView;
		
		public SettingsHolder (View view) {
			userImageView = (ImageView)view.findViewById(R.id.user_image_view);
			notificationView = (TextView)view.findViewById(R.id.notification);
			dateView = (TextView)view.findViewById(R.id.date);
		}
	}
}