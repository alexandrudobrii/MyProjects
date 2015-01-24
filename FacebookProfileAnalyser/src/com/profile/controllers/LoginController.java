package com.profile.controllers;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.social.facebook.api.Facebook;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import com.profile.dao.NewsDAO;

@Controller
public class LoginController {
	private Facebook facebook;
	
	@Autowired
    public void setFacebook (Facebook facebook) {
    	this.facebook = facebook;
    }
	
	@RequestMapping(value={"/login"}, method=RequestMethod.GET)
	public ModelAndView showMusic() {
		if (facebook.isAuthorized()) {
			return new ModelAndView("login", "", "");
		}
		
		return new ModelAndView("home", "data", NewsDAO.getAllNews());
	}
	
}