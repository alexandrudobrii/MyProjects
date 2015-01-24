package com.application.socialandroidapplication;

import java.util.ArrayList;
import java.util.List;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import com.application.adapters.SettingsListAdapter;
import com.application.facebook.controller.FacebookLoginActivity;
import com.application.google.maps.SocialMapActivity;
import com.application.social.maps.R;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.SettingModel;
import com.facebook.Session;

public class SettingsActivity extends Fragment {
	private SharedPreferences    	preferences;
	private SettingsListAdapter 	settingsListAdapter;
	private ListView 				settingsListView;
	private TextView 				userTextView;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		userTextView = (TextView)getActivity().findViewById(R.id.username);
		AndroidUtils.setFont(getActivity(), userTextView, Constants.TEXT_FONT, "Settings");
		
		preferences = getActivity().getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		final List<SettingModel> settingsModel = getSettingsValues();
		settingsListAdapter = new SettingsListAdapter(getActivity(), settingsModel);
		
		settingsListView = (ListView)getActivity().findViewById(R.id.settings_list);
		settingsListView.setAdapter(settingsListAdapter);
		settingsListView.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> a, View v, int position, long id) {
				if (settingsModel.get(position).getSetting().equals(Constants.CLEAR_CACHE)) {
					AndroidUtils.clearSharedPreferences(getActivity());
					settingsListAdapter.notifyDataSetChanged();
					AndroidUtils.showToast(getActivity(), Constants.CACHE_CLEARED, Toast.LENGTH_SHORT);
				} else if (settingsModel.get(position).getSetting().equals(Constants.CLEAR_NOTIFICATIONS)) {
					AndroidUtils.showToast(getActivity(), Constants.NOTIFICATIONS_CLEARED, Toast.LENGTH_SHORT);
				} else if (settingsModel.get(position).getSetting().equals(Constants.LOGOUT)) {
					if (Session.getActiveSession() != null) {
						Session.getActiveSession().closeAndClearTokenInformation();
						Session.setActiveSession(null);
						HomeActivity.stopTimer();
						SocialMapActivity.stopTimer();
					}
					Intent intent = new Intent(getActivity(), FacebookLoginActivity.class);
					getActivity().startActivity(intent);
				} else if (settingsModel.get(position).getSetting().equals(Constants.SET_RADIUS)) {
					AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
					builder.setTitle("Select radius");
					final int radiusIndex = preferences.getInt(Constants.RADIUS_INDEX, 0);
					builder.setSingleChoiceItems(Constants.selectionRadiusList, radiusIndex, new DialogInterface.OnClickListener() {
					    public void onClick(DialogInterface dialog, int item) {
					    	String selection = Constants.selectionRadiusList[item].toString();
					    	preferences.edit().putFloat(Constants.SET_RADIUS, getRadius(selection)).commit();
					    	preferences.edit().putInt(Constants.RADIUS_INDEX, item).commit();
					    	dialog.dismiss();
					    }
					});
					builder.create().show();
				}
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_settings, container, false);
	}
	
	private float getRadius (String selection) {
		if (selection.equals(Constants.ALL_MAP_VISIBILITY)) {
			return 0.0f;
		}
		String[] radius = selection.split(" ");
		if (radius[1].contains("kilometer")) {
			return Float.parseFloat(radius[0].trim())*1000.0f;
		} else if (radius[1].contains("meter")) {
			return Float.parseFloat(radius[0].trim());
		}
		
		return 0.0f;
	}
	
	private List<SettingModel> getSettingsValues() {
		List<SettingModel> results = new ArrayList<SettingModel>();
		
		results.add(new SettingModel(Constants.ENABLE_PRIVACY, preferences.getBoolean(Constants.ENABLE_PRIVACY, true), true));
		results.add(new SettingModel(Constants.SET_RADIUS, false, false));
		results.add(new SettingModel(Constants.CLEAR_NOTIFICATIONS, preferences.getBoolean(Constants.CLEAR_NOTIFICATIONS, false), false));
		results.add(new SettingModel(Constants.ROTATE_SCREEN, preferences.getBoolean(Constants.ROTATE_SCREEN, false), true));
		results.add(new SettingModel(Constants.CLEAR_CACHE, preferences.getBoolean(Constants.CLEAR_CACHE, false), false));
		results.add(new SettingModel(Constants.LOGOUT, preferences.getBoolean(Constants.LOGOUT, false), false));
		
		return results;
	}
}