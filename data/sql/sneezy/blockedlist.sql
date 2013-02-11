-- MySQL dump 10.10
--
-- Host: db.sneezymud.com    Database: sneezy
-- ------------------------------------------------------
-- Server version	5.0.24a-standard

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `blockedlist`
--

DROP TABLE IF EXISTS `blockedlist`;
CREATE TABLE `blockedlist` (
  `player_id` int(11) default NULL,
  `blocked` varchar(33) default NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `blockedlist`
--


/*!40000 ALTER TABLE `blockedlist` DISABLE KEYS */;
LOCK TABLES `blockedlist` WRITE;
INSERT INTO `blockedlist` VALUES (3096,'~dopeman'),(3096,'~eternal'),(5605,'~dyehead'),(5605,'~eternal'),(5548,'waut'),(50015,'ac'),(3096,'~thirdrail'),(116300,'ri'),(3096,'~afterlife'),(3096,'~crucial'),(99214,'ren'),(3096,'~afterlife'),(3096,'~crucial'),(3096,'~dopeman'),(3096,'~eternal');
UNLOCK TABLES;
/*!40000 ALTER TABLE `blockedlist` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

