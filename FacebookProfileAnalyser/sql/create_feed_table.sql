CREATE TABLE `feed` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `short_news` varchar(250) DEFAULT NULL,
  `long_news` varchar(2000) DEFAULT NULL,
  `image` varchar(25) DEFAULT NULL,
  `description` varchar(45) DEFAULT NULL,
  `category` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=latin1;
