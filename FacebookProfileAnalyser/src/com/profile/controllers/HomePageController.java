package com.profile.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

import com.profile.dao.NewsDAO;

@Controller
public class HomePageController {
	
	@RequestMapping(value={"/home"}, method=RequestMethod.GET)
	public ModelAndView showUsers() {
		return new ModelAndView("home", "data", NewsDAO.getAllNews());
	}
	
}