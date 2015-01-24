package com.application.trafficsignrecognition;

import java.util.ArrayList;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.application.adapters.SettingsCustomListAdapter;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.NewItem;
import com.application.utils.ServerConnection;

public class SettingsActivity extends Fragment {
	
	// settings
	private static TextView             userTextView;
	private static ListView             listView;
	private static SharedPreferences    preferences;
	private ServerConnection            connection;
	
	// animations
	private Animation                   animShow;
	private Animation                   animHide;
	
	// connection
	private EditText                    serverIpView;
	private EditText                    serverPortView;
	private Button                      acceptButton;
	private Button                      dismissButton;
	private static LinearLayout         connectionPopup;
	
	// info
	private TextView                    infoTextView;
	private Button                      returnInfoButton;
	private static RelativeLayout       infoPopup;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		preferences = getActivity().getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		final ArrayList<NewItem> listSettings = getListData();
		
		connectionPopup = (LinearLayout) getActivity().findViewById(R.id.popup_window);
		connectionPopup.setVisibility(View.GONE);
		
		infoPopup = (RelativeLayout) getActivity().findViewById(R.id.info_layout);
		infoPopup.setVisibility(View.GONE);
		
		animShow = AnimationUtils.loadAnimation(getActivity(), R.anim.popup_show);
		animHide = AnimationUtils.loadAnimation(getActivity(), R.anim.popup_hide);
		
		userTextView = (TextView)getActivity().findViewById(R.id.username);
		String username = MainMenuActivity.getUsername();
		if (username != null) {
			AndroidUtils.setFont(getActivity(), userTextView, Constants.TEXT_FONT, MainMenuActivity.getUsername());
		} else {
			AndroidUtils.setFont(getActivity(), userTextView, Constants.TEXT_FONT, Constants.APP_NAME);
		}
		
		listView = (ListView)getActivity().findViewById(R.id.custom_list);
		listView.setAdapter(new SettingsCustomListAdapter(getActivity(), listSettings));
		listView.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> a, View v, int position, long id) {
				if (listSettings.get(position).getSetting().equals(Constants.SET_CONNECTION)) {
					if (connection == null) {
						setupConnectionPopup();
					}
					
					connectionPopup.setVisibility(View.VISIBLE);
					connectionPopup.startAnimation(animShow);
					userTextView.setVisibility(View.GONE);
					listView.setVisibility(View.GONE);
					serverIpView.setText("");
					serverPortView.setText("");
					
				} else if (listSettings.get(position).getSetting().equals(Constants.INFO)) {
					if (infoTextView == null) {
						setupInfoPopup();
					}
					
					infoPopup.setVisibility(View.VISIBLE);
					infoPopup.startAnimation(animShow);
					userTextView.setVisibility(View.GONE);
					listView.setVisibility(View.GONE);
					
				} else if (listSettings.get(position).getSetting().equals(Constants.CLEAR_CACHE)) {
					if (UserActivity.logsList != null) {
						UserActivity.logsList.clear();
						UserActivity.logsAdaptorListView.notifyDataSetChanged();
					}
					
					AndroidUtils.clearSharedPreferences(getActivity());
					AndroidUtils.showToast(getActivity(), Constants.CACHE_CLEARED, Toast.LENGTH_SHORT);
				} else if (listSettings.get(position).getSetting().equals(Constants.LOGOUT) ||
						   listSettings.get(position).getSetting().equals(Constants.EXIT)) {
					getActivity().finish();
				}
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_settings, container, false);
	}
	
	private static ArrayList<NewItem> getListData() {
		ArrayList<NewItem> results = new ArrayList<NewItem>();
		
		results.add(new NewItem(Constants.SKIP_LOGIN, preferences.getBoolean(Constants.SKIP_LOGIN, false), true));
		results.add(new NewItem(Constants.STORE_DETECTED_FRAMES, preferences.getBoolean(Constants.STORE_DETECTED_FRAMES, true), true));
		results.add(new NewItem(Constants.PROCESS_ON_SERVER, preferences.getBoolean(Constants.PROCESS_ON_SERVER, false), true));
		results.add(new NewItem(Constants.SET_CONNECTION, preferences.getBoolean(Constants.SET_CONNECTION, false), false));
		results.add(new NewItem(Constants.CLEAR_CACHE, preferences.getBoolean(Constants.CLEAR_CACHE, false), false));
		results.add(new NewItem(Constants.ROTATE_SCREEN, preferences.getBoolean(Constants.ROTATE_SCREEN, false), true));
		results.add(new NewItem(Constants.INFO, preferences.getBoolean(Constants.INFO, false), false));
		
		if (MainMenuActivity.isLoginPresent() == true) {
			Log.d("SETTING", "client is present");
			results.add(new NewItem(Constants.LOGOUT, preferences.getBoolean(Constants.LOGOUT, false), false));
		} else {
			Log.d("SETTING", "login skipped");
			results.add(new NewItem(Constants.EXIT, preferences.getBoolean(Constants.LOGOUT, false), false));
		}
		
		return results;
	}
	
	public void setupConnectionPopup() {
		Log.d("SETTING", "connection setup");
		connection = new ServerConnection(preferences);
		
		serverIpView = (EditText)getActivity().findViewById(R.id.server_ip);
		//serverIpView.setText(preferences.getString(Constants.SERVER_IP_TAG, connection.getIp()));
		serverPortView = (EditText)getActivity().findViewById(R.id.server_port);
		//serverPortView.setText(preferences.getString(Constants.SERVER_PORT_TAG, connection.getPort()));
		
		acceptButton = (Button)getActivity().findViewById(R.id.accept_button);
		acceptButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				if (!serverIpView.getText().toString().equals("") && !serverPortView.getText().toString().equals("")) {
					preferences.edit().putString(Constants.SERVER_IP_TAG, serverIpView.getText().toString()).commit();
					preferences.edit().putString(Constants.SERVER_PORT_TAG, serverPortView.getText().toString()).commit();
				}
				
				connectionPopup.startAnimation(animHide);
				connectionPopup.setVisibility(View.GONE);
				listView.setVisibility(View.VISIBLE);
				userTextView.setVisibility(View.VISIBLE);
			}
		});
		
		dismissButton = (Button)getActivity().findViewById(R.id.return_button);
		dismissButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				connectionPopup.startAnimation(animHide);
				connectionPopup.setVisibility(View.GONE);
				listView.setVisibility(View.VISIBLE);
				userTextView.setVisibility(View.VISIBLE);
			}
		});
	}
	
	public void setupInfoPopup() {
		Log.d("SETTING", "info setup");
		
		returnInfoButton = (Button)getActivity().findViewById(R.id.back);
		returnInfoButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				infoPopup.startAnimation(animHide);
				infoPopup.setVisibility(View.GONE);
				listView.setVisibility(View.VISIBLE);
				userTextView.setVisibility(View.VISIBLE);
			}
		});
		
		infoTextView = (TextView)getActivity().findViewById(R.id.info_message);
		AndroidUtils.setFont(getActivity(), infoTextView, Constants.TEXT_FONT, Constants.INFO_MESSAGE);
	}
}