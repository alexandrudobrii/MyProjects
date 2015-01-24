package com.application.adapters;

import java.util.List;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.TextView;

import com.application.social.maps.R;
import com.application.socialandroidapplication.MapActivity;
import com.application.utils.Constants;
import com.application.utils.SettingModel;

public class SettingsListAdapter extends BaseAdapter {
	private List<SettingModel> 		settingsValues;
	private LayoutInflater			layoutInflater;
	private SharedPreferences		preferences;
	
	public SettingsListAdapter (Activity context, List<SettingModel> settingsValues) {
		this.settingsValues = settingsValues;
		this.layoutInflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		this.preferences = context.getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
	}
	
	@Override
	public int getCount() {
		return settingsValues.size();
	}

	@Override
	public SettingModel getItem (int position) {
		return settingsValues.get(position);
	}

	@Override
	public long getItemId (int position) {
		return position;
	}

	@SuppressLint("InflateParams") @Override
	public View getView (int position, View convertView, ViewGroup parent) {
		 View view = convertView;
		 final SettingsHolder viewHolder;
         if (convertView == null) {
        	 view = layoutInflater.inflate(R.layout.settings_list_layout, null);
        	 viewHolder = new SettingsHolder(view);
        	 view.setTag(viewHolder);
         } else {
              viewHolder = (SettingsHolder) view.getTag();
         }
         
         final String setting = settingsValues.get(position).getSetting();
         viewHolder.settingView.setText(setting);
         viewHolder.checkboxView.setChecked(settingsValues.get(position).isSelected());
         viewHolder.checkboxView.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Log.d(Constants.SETTINGS_TAG, setting + " is " + viewHolder.checkboxView.isChecked());
				preferences.edit().putBoolean(setting, viewHolder.checkboxView.isChecked()).commit();
				
				if (viewHolder.checkboxView.isChecked()) {
					MapActivity.statusTextView.setText(Constants.USER_OFFLINE);
					MapActivity.statusTextView.setTextColor(Color.RED);
				} else {
					MapActivity.statusTextView.setText(Constants.USER_ONLINE);
					MapActivity.statusTextView.setTextColor(Color.GREEN);
				}
			}
		});
         
         if (settingsValues.get(position).isVisible() == false) {
        	 viewHolder.checkboxView.setVisibility(View.GONE);
         } else {
        	 viewHolder.checkboxView.setVisibility(View.VISIBLE);
         }
         
         return view;
	}
	
	static class SettingsHolder {
		public TextView settingView;
		public CheckBox checkboxView;
		
		public SettingsHolder (View view) {
			settingView = (TextView)view.findViewById(R.id.setting);
			checkboxView = (CheckBox)view.findViewById(R.id.checkbox);
		}
	}
}