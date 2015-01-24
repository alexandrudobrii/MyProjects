package com.application.trafficsignrecognition;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.GridView;

import com.application.adapters.GalleryGridViewAdapter;
import com.application.utils.BitmapManager;

public class GalleryGridActivity extends Fragment {
	private GridView                 gridView;
	private GalleryGridViewAdapter   customGridAdapter;
	private BitmapManager            bitmapManager;
	
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		bitmapManager = new BitmapManager(getActivity().getResources());
		customGridAdapter = new GalleryGridViewAdapter(getActivity(), R.layout.gallery_row_grid, bitmapManager.getImageItems());
		
		gridView = (GridView)getActivity().findViewById(R.id.gridView);
		gridView.setAdapter(customGridAdapter);
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		return inflater.inflate(R.layout.activity_gallery_grid, container, false);
	}
}