<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>

<html>
<head>
	<meta charset="UTF-8">
	<title>Read more</title>
	<link rel="stylesheet" href="css/style.css" type="text/css">
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
		<div class="clearfix">
			<h1>${data.category}</h1>
			<div class="frame2">
				<div class="box">
					<img src="images/${data.image}" alt="Img" height="298" width="924">
				</div>
			</div>
			<p>
				${data.longNews}
			</p>
		</div>
	</div>
	
</body>
</html>