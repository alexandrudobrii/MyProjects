package com.application.adapters;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import com.application.socialandroidapplication.MapActivity;
import com.application.socialandroidapplication.SettingsActivity;
import com.application.socialandroidapplication.UsersActivity;

public class TabsPagerAdapter extends FragmentPagerAdapter {

	public TabsPagerAdapter(FragmentManager fm) {
		super(fm);
	}

	@Override
	public Fragment getItem(int index) {
		switch (index) {
			case 0:
				return new MapActivity();
			case 1:
				return new UsersActivity();
			case 2:
				return new SettingsActivity();
		}
		
		return null;
	}

	@Override
	public int getCount() {
		return 3;
	}
}