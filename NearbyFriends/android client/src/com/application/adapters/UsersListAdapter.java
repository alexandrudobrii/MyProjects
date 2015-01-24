package com.application.adapters;

import java.util.List;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.application.social.maps.R;
import com.application.socialandroidapplication.HomeActivity;
import com.application.utils.Constants;
import com.application.utils.UserDetails;
import com.pkmmte.circularimageview.CircularImageView;

public class UsersListAdapter extends BaseAdapter {
	private List<UserDetails> 		usersValues;
	private LayoutInflater			layoutInflater;
	
	public UsersListAdapter (Activity context, List<UserDetails> usersValues) {
		this.usersValues = usersValues;
		this.layoutInflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
	}
	
	@Override
	public int getCount() {
		return usersValues.size();
	}

	@Override
	public UserDetails getItem (int position) {
		return usersValues.get(position);
	}

	@Override
	public long getItemId (int position) {
		return Long.parseLong(usersValues.get(position).getId());
	}

	@SuppressLint("InflateParams") @Override
	public View getView (int position, View convertView, ViewGroup parent) {
		 View view = convertView;
		 SettingsHolder viewHolder;
         if (convertView == null) {
        	 view = layoutInflater.inflate(R.layout.users_list_layout, null);
        	 viewHolder = new SettingsHolder(view);
        	 view.setTag(viewHolder);
         } else {
              viewHolder = (SettingsHolder) view.getTag();
         } 
         
         UserDetails user = usersValues.get(position);
         
         Bitmap userPhoto = HomeActivity.userPhoto.get(user.getId());
         if (userPhoto == null) {
        	 viewHolder.userPictureView.setImageResource(R.drawable.user_man);
         } else {
        	 viewHolder.userPictureView.setImageBitmap(userPhoto);
         }
        	 
         viewHolder.usernameView.setText(user.getName());
         
         if (user.getStatus() != null && !user.getStatus().equals("")) {
        	 if (user.getStatus().equals(Constants.USER_ONLINE)) {
        		 viewHolder.userStatusView.setText(Constants.USER_ONLINE);
        		 viewHolder.userStatusView.setTextColor(Color.GREEN);
        	 } else if (user.getStatus().equals(Constants.USER_OFFLINE)) {
        		 viewHolder.userStatusView.setText(Constants.USER_OFFLINE);
        		 viewHolder.userStatusView.setTextColor(Color.RED);
        	 } else if (user.getStatus().equals(Constants.USER_NOT_IN_RANGE)) {
        		 viewHolder.userStatusView.setText(Constants.USER_NOT_IN_RANGE);
        		 viewHolder.userStatusView.setTextColor(Color.YELLOW);
        	 }
         } else {
        	 viewHolder.userStatusView.setText(Constants.USER_ONLINE);
    		 viewHolder.userStatusView.setTextColor(Color.BLUE);
         }
         
         return view;
	}
	
	static class SettingsHolder {
		public CircularImageView userPictureView;
		public TextView usernameView;
		public TextView userStatusView;
		
		public SettingsHolder (View view) {
			userPictureView = (CircularImageView)view.findViewById(R.id.user_picture);
			usernameView = (TextView)view.findViewById(R.id.user_name);
			userStatusView = (TextView)view.findViewById(R.id.user_status);
		}
	}
}