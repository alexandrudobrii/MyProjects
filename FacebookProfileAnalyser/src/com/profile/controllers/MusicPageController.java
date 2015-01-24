package com.profile.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import com.profile.dao.NewsDAO;

@Controller
public class MusicPageController {
	
	@RequestMapping("/music")
	public ModelAndView showMusic() {
		return new ModelAndView("music", "data", NewsDAO.getMusicNews());
	}
}