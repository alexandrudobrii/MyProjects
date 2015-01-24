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

import com.application.adapters.UsersListAdapter;
import com.application.google.maps.SocialMapActivity;
import com.application.social.maps.R;
import com.application.utils.AndroidUtils;
import com.application.utils.Constants;
import com.application.utils.UserDetails;

public class UsersActivity extends Fragment {
	//private SharedPreferences    		preferences;
	private ListView 					usersListView;
	private TextView 					usersTextView;
	public static UsersListAdapter 		usersListAdapter;
	private View  						progress;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		progress = getActivity().findViewById(R.id.login_status);
		usersListView = (ListView)getActivity().findViewById(R.id.users_list);
		usersTextView = (TextView)getActivity().findViewById(R.id.users_title);
		
		// showProgress(true);
		// preferences = getActivity().getSharedPreferences(Constants.PREFERENCES_FILE, Context.MODE_PRIVATE);
		
		AndroidUtils.setFont(getActivity(), usersTextView, Constants.TEXT_FONT, "Users");
		
		usersListAdapter = new UsersListAdapter(getActivity(), HomeActivity.usersList);
		usersListView.setAdapter(usersListAdapter);
		usersListView.setOnItemClickListener(new OnItemClickListener() {	
			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				UserDetails userDetails = (UserDetails)parent.getItemAtPosition(position);
				String status = userDetails.getStatus();
				if (status.equals(Constants.USER_OFFLINE) || status.equals(Constants.USER_NOT_IN_RANGE)) {
					AndroidUtils.showToast(getActivity(), "Selected user is " + status, Toast.LENGTH_SHORT);
					Log.d(Constants.MAP_TAG, userDetails.getName() + " is " + status);
				} else {
					Intent intent = new Intent(getActivity(), SocialMapActivity.class);
					intent.putExtra(Constants.USER_PROFILE_TAG, position);
					getActivity().startActivity(intent);
				}
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_users, container, false);
	}
	
	public void showProgress(final boolean show) {
		progress.setVisibility(show ? View.VISIBLE : View.GONE);
		usersListView.setVisibility(show ? View.GONE : View.VISIBLE);
		usersTextView.setVisibility(show ? View.GONE : View.VISIBLE);
	}
}