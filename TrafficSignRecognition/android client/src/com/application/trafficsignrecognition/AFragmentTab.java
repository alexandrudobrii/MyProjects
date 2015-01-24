package com.application.trafficsignrecognition;

import android.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

public class AFragmentTab extends Fragment {
	private Button startGalleryButton;

	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		getActivity().overridePendingTransition(R.anim.fragment_animation_fade_in, R.anim.fragment_animation_fade_out);
		
		startGalleryButton = (Button) getActivity().findViewById(R.id.startGallery);
		startGalleryButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(getActivity(), GalleryActivity.class);
				getActivity().startActivity(intent);
			}
		});
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,Bundle savedInstanceState) {	
		return inflater.inflate(R.layout.fragment_a, container, false);
	} 
}