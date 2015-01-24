package com.application.utils;

import java.text.SimpleDateFormat;
import java.util.Date;

public abstract class JavaUtils {
	
	public static String getCurrentDate (String dateFormat) {
		SimpleDateFormat simpleDateFormat = new SimpleDateFormat(dateFormat);
		return simpleDateFormat.format(new Date());
	}

}
