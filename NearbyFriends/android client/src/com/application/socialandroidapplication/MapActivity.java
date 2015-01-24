package com.application.socialandroidapplication;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.application.facebook.controller.FacebookMainFragment;
import com.application.google.maps.SocialMapActivity;
import com.application.social.maps.R;
import com.application.utils.Constants;
import com.pkmmte.circularimageview.CircularImageView;

public class MapActivity extends Fragment {
	private SharedPreferences    		preferences;
	
	private CircularImageView	userImageView;
	private Button 				showMapButton;
	public static TextView		statusTextView;
	
	@SuppressLint("ClickableViewAccessibility") @Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		preferences = getActivity().getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		statusTextView = (TextView)getActivity().findViewById(R.id.status);
		if (preferences.getBoolean(Constants.ENABLE_PRIVACY, true) == true) {
			statusTextView.setText(Constants.USER_OFFLINE);
			statusTextView.setTextColor(Color.RED);
		}  else {
			statusTextView.setText(Constants.USER_ONLINE);
			statusTextView.setTextColor(Color.GREEN);
		}
		
		userImageView = (CircularImageView)getActivity().findViewById(R.id.user_image);
		if (FacebookMainFragment.myPhoto == null) {
			userImageView.setImageResource(R.drawable.user_man);
		} else {
			userImageView.setImageBitmap(FacebookMainFragment.myPhoto);
		}
	
		userImageView.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch (View v, MotionEvent event) {
				if (userImageView.isSelected()) {
					Intent intent = new Intent(getActivity(), SocialMapActivity.class);
					intent.putExtra(Constants.USER_PROFILE_TAG, Constants.CURRENT_USER_ID);
					getActivity().startActivity(intent);
					return true;
				}
				
				return false;
			}
		});
		
		showMapButton = (Button)getActivity().findViewById(R.id.show_map);
		showMapButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(getActivity(), SocialMapActivity.class);
				intent.putExtra(Constants.USER_PROFILE_TAG, Constants.CURRENT_USER_ID);
				getActivity().startActivity(intent);
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_map, container, false);
	}
}