package com.application.trafficsignrecognition;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

public class CameraPreviewActivity extends Fragment {
	private Button startCameraButton;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		
		startCameraButton = (Button) getActivity().findViewById(R.id.startGallery);
		//startCameraButton.getBackground().setColorFilter(Color.GREEN, Mode.MULTIPLY);
		startCameraButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(getActivity(), CameraActivity.class);
				getActivity().startActivity(intent);
			}
		});
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_camera_preview, container, false);
	}
}