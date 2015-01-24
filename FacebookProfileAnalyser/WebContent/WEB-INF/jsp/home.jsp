<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib prefix="fn" uri="http://java.sun.com/jsp/jstl/functions" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">

<html>
<head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
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
				<li class="active">
					<a href="index.html">Home</a>
				</li>
				<li>
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
				<%for (int step = 0; step <= 3; step++) { %>
					<c:set var="index" value="<%=step%>"/>
					<li>
						<div class="frame1">
							<div class="box">
								<img src="<c:url value="images/${data[index].image}" />" alt="Img" height="130" width="197" />
							</div>
						</div>
						<p> <c:out value="${data[index].shortNews}"/>  </p>
						<a href="read_more.html?newsId=${data[index].id}" class="more"> Read More </a>
					</li>
				<%}%>
			</ul>
		</div>
	</div>
	
	<div id="contents">
		<div class="featured">
			<h2>Movies</h2>
			<ul class="clearfix">
				<%for (int step = 5; step <= 8; step++) { %>
					<c:set var="index" value="<%=step%>"/>
					<li>
						<div class="frame1">
							<div class="box">
								<img src="<c:url value="images/${data[index].image}" />" alt="Img" height="130" width="197" />
							</div>
						</div>
						<p> <c:out value="${data[index].shortNews}"/>  </p>
						<a href="read_more.html?newsId=${data[index].id}" class="more"> Read More </a>
					</li>
				<%}%>
			</ul>
		</div>
	</div>
	
	<div id="contents">
		<div class="featured">
			<h2>Sports</h2>
			<ul class="clearfix">
				<%for (int step = 10; step <= 13; step++) { %>
					<c:set var="index" value="<%=step%>"/>
					<li>
						<div class="frame1">
							<div class="box">
								<img src="<c:url value="images/${data[index].image}" />" alt="Img" height="130" width="197" />
							</div>
						</div>
						<p> <c:out value="${data[index].shortNews}"/>  </p>
						<a href="read_more.html?newsId=${data[index].id}" class="more"> Read More </a>
					</li>
				<%}%>
			</ul>
		</div>
	</div>
	
	<div id="contents">
		<div class="featured">
			<h2>Locations</h2>
			<ul class="clearfix">
				<%for (int step = 15; step <= 18; step++) { %>
					<c:set var="index" value="<%=step%>"/>
					<li>
						<div class="frame1">
							<div class="box">
								<img src="<c:url value="images/${data[index].image}" />" alt="Img" height="130" width="197" />
							</div>
						</div>
						<p> <c:out value="${data[index].shortNews}"/>  </p>
						<a href="read_more.html?newsId=${data[index].id}" class="more"> Read More </a>
					</li>
				<%}%>
			</ul>
		</div>
	</div>
	
</body>
</html>