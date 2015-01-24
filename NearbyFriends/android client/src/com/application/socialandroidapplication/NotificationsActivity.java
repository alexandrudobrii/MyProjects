package com.application.socialandroidapplication;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.application.adapters.NotificationsListAdapter;
import com.application.google.maps.SocialMapActivity;
import com.application.social.maps.R;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.UserDetails;

public class NotificationsActivity extends Fragment {
	public static NotificationsListAdapter 		notificationsListAdapter;
	
	private ListView 							notificationsListView;
	private TextView 							notificationsTitleView;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		notificationsTitleView = (TextView)getActivity().findViewById(R.id.notifications_title);
		AndroidUtils.setFont(getActivity(), notificationsTitleView, Constants.TEXT_FONT, "Notifications");
		
		notificationsListAdapter = new NotificationsListAdapter(getActivity(), null);
		
		notificationsListView = (ListView)getActivity().findViewById(R.id.notifications_list);
		notificationsListView.setAdapter(notificationsListAdapter);
		notificationsListView.setOnItemClickListener(new OnItemClickListener() {	
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				UserDetails userDetails = (UserDetails)parent.getItemAtPosition(position);
				if (userDetails.isEnabled()) {
					Intent intent = new Intent(getActivity(), SocialMapActivity.class);
					intent.putExtra(Constants.USER_PROFILE_TAG, position);
					getActivity().startActivity(intent);
				} else {
					AndroidUtils.showToast(getActivity(), "Selected user is offine", Toast.LENGTH_SHORT);
					Log.d(Constants.MAP_TAG, userDetails.getName() + " is offline");
				}
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_notifications, container, false);
	}
}