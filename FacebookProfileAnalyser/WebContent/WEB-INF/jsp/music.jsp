<%@ page language="java" contentType="text/html; charset=ISO-8859-1" pageEncoding="ISO-8859-1"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
	<title>News Feed</title>
	<link rel="stylesheet" href="css/style.css" />
</head>
<body>
	
	<div id="header">
		<div class="clearfix">
			<div class="logo">
				<a href="index.html"></a>
			</div>
			<ul class="navigation">
				<li>
					<a href="index.html">Home</a>
				</li>
				<li class="active">
					<a href="music.html">Music</a>
				</li>
				<li>
					<a href="movies.html">Movies</a>
				</li>
				<li>
					<a href="sports.html">Sports</a>
				</li>
				<li>
					<a href="locations.html">Locations</a>
				</li>
			</ul>
		</div>
	</div>
	<div id="contents">
		<div class="featured">
			<h2>Music</h2>
			<ul class="clearfix">
				<c:forEach var="p" items="${data}" varStatus="loop">
					<c:if test="${loop.index < 4}">
            		<li>
						<div class="frame1">
							<div class="box">
								<img src="<c:url value="images/${p.image}" />" alt="Img"height="130" width="197" />
							</div>
						</div>
						<p>
							${p.shortNews}
						</p>
						<a href="read_more.html?newsId=${p.id}" class="more"> Read More </a>
					</li>
					</c:if>
				</c:forEach>
			</ul>
		</div>
	</div>
</body>
</html>