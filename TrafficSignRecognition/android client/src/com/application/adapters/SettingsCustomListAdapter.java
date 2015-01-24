package com.application.adapters;

import java.util.ArrayList;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.TextView;

import com.application.trafficsignrecognition.R;
import com.application.utils.Constants;
import com.application.utils.NewItem;

public class SettingsCustomListAdapter extends BaseAdapter {	
	private ArrayList<NewItem> listData;
	private LayoutInflater layoutInflater;
	private SharedPreferences preferences;
	
	public SettingsCustomListAdapter (Context context, ArrayList<NewItem> listData) {
		this.listData = listData;
		layoutInflater = LayoutInflater.from(context);
		preferences = context.getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
	}

	@Override
	public int getCount() {
		return listData.size();
	}

	@Override
	public Object getItem(int position) {
		return listData.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	public View getView(int position, View convertView, ViewGroup parent) {
		convertView = layoutInflater.inflate(R.layout.settings_list_layout, null);
			
		ViewHolder holder = new ViewHolder();
		holder.settingView = (TextView) convertView.findViewById(R.id.setting);
		holder.checkboxView = (CheckBox) convertView.findViewById(R.id.checkbox);
		holder.checkboxView.setOnClickListener(new CheckBoxAdapter(listData.get(position).getSetting(), holder.checkboxView));
			
		holder.settingView.setText(listData.get(position).getSetting());
		holder.checkboxView.setChecked(listData.get(position).isSelected());
		if (listData.get(position).isVisible() == false) {
			holder.checkboxView.setVisibility(View.GONE);
		}
			
		return convertView;
	}
	
	static class ViewHolder {
		TextView settingView;
		CheckBox checkboxView;
	}
	
	class CheckBoxAdapter implements View.OnClickListener {
		private String setting;
		private CheckBox checkBox;
		
		public CheckBoxAdapter (String setting, CheckBox checkBox) {
			this.setting = setting;
			this.checkBox = checkBox;
		}

		@Override
		public void onClick(View v) {
			preferences.edit().putBoolean(setting, checkBox.isChecked()).commit();
			Log.d("SETTING", setting + " = " + checkBox.isChecked());
		}
	}
}