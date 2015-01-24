package com.profile.controllers;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;

import com.profile.dao.NewsDAO;

@Controller
public class ReadMorePageController {
	
	@RequestMapping(value="/read_more", method=RequestMethod.GET)
	public ModelAndView readMore (@RequestParam int newsId) {
		return new ModelAndView("read_more", "data", NewsDAO.getNewsFeed(newsId));
	}
}