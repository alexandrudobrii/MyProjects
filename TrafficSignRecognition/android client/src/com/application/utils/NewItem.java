package com.application.utils;

public class NewItem {
	private String setting;
	private boolean isSelected;
	private boolean isVisible;
	
	public NewItem (String setting, boolean isSelected, boolean isVisible) {
		this.setting = setting;
		this.isSelected = isSelected;
		this.isVisible = isVisible;
	}
	
	public String getSetting() {
		return setting;
	}
	
	public void setSetting(String setting) {
		this.setting = setting;
	}
	
	public boolean isSelected() {
		return isSelected;
	}

	public void setSelected(boolean isSelected) {
		this.isSelected = isSelected;
	}

	public boolean isVisible() {
		return isVisible;
	}

	public void setVisible(boolean isVisible) {
		this.isVisible = isVisible;
	}
}
