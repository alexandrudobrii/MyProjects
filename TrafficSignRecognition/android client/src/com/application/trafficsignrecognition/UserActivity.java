package com.application.trafficsignrecognition;

import java.util.ArrayList;
import java.util.List;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;

import com.application.adapters.LogsAdaptorListView;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;

public class UserActivity extends Fragment {
	private TextView                    tabNameTextView;
	private ListView                    logsListView;
	public static LogsAdaptorListView   logsAdaptorListView;
	
	public static List<String>       logsList;
	public static boolean            finished = false;

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		Log.d("NOTIFIER", "on Create - UserActivity");
		
		logsList = new ArrayList<String>();
		
		logsAdaptorListView = new LogsAdaptorListView(getActivity(), logsList);
		logsAdaptorListView.notifyDataSetChanged();
		
		tabNameTextView = (TextView)getActivity().findViewById(R.id.activity_tab);
		AndroidUtils.setFont(getActivity(), tabNameTextView, Constants.TEXT_FONT, Constants.USER_ACTIVITY);
		
		logsListView = (ListView)getActivity().findViewById(R.id.logs);
		logsListView.setAdapter(logsAdaptorListView);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_user, container, false);
	}
	
	public void onResume() {
		super.onResume();
		Log.d("NOTIFIER", "on Resume - UserActivity");
	}
	
	public void onStop() {
		super.onStop();
		Log.d("NOTIFIER", "on Stop - UserActivity");
	}
}