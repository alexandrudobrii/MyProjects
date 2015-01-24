package com.application.adapters;

import com.application.trafficsignrecognition.CameraPreviewActivity;
import com.application.trafficsignrecognition.GalleryGridActivity;
import com.application.trafficsignrecognition.MainMenuActivity;
import com.application.trafficsignrecognition.UserActivity;
import com.application.trafficsignrecognition.SettingsActivity;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class TabsPagerAdapter extends FragmentPagerAdapter {

	public TabsPagerAdapter(FragmentManager fm) {
		super(fm);
	}

	@Override
	public Fragment getItem(int index) {
		if (MainMenuActivity.getUsername() != null) {
			switch (index) {
				case 0:
					return new CameraPreviewActivity();
				case 1:
					return new GalleryGridActivity();
				case 2:
					return new UserActivity();
				case 3:
					return new SettingsActivity();
			}
		}
		else {
			switch (index) {
				case 0:
					return new CameraPreviewActivity();
				case 1:
					return new GalleryGridActivity();
				case 2:
					return new SettingsActivity();
			}
		}
		
		return null;
	}

	@Override
	public int getCount() {
		if (MainMenuActivity.getUsername() != null) {
			return 4;
		}
		
		return 3;
	}
}
