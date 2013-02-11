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
-- Table structure for table `ship_destinations`
--

DROP TABLE IF EXISTS `ship_destinations`;
CREATE TABLE `ship_destinations` (
  `vnum` int(11) default NULL,
  `name` varchar(32) default NULL,
  `room` int(11) default NULL,
  KEY `ix1__ship_destinations` (`vnum`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `ship_destinations`
--


/*!40000 ALTER TABLE `ship_destinations` DISABLE KEYS */;
LOCK TABLES `ship_destinations` WRITE;
INSERT INTO `ship_destinations` VALUES (19000,'Fishingshack',15150),(19000,'Mansion',26869),(19000,'Garbageisland',12623),(19000,'Citadel',10847),(19000,'Gnath',2400),(19000,'Whirlpool',12814),(19000,'WhirlpoolVortex',12781),(19000,'StrangeIsland',13142),(19000,'NorthEastMyrian',12545),(19000,'creed',2495),(19000,'poachers',15294),(19000,'WestGrimhaven',5417),(19000,'JungleBeach',12643),(19000,'SpiritBeach',14114),(19000,'Yola',12889),(19000,'siren',12874),(19000,'LazyGuy',13106),(19000,'Benjamin',12854),(19000,'Xanesla',6301),(19000,'sunkenship',13111),(19000,'lotsofcrap',12607),(19000,'smalltropicalisland',12802),(15375,'cardac',2471),(15375,'garbage',12623),(15375,'kalysia',14117),(15375,'fishmaster',15150),(15375,'Neghya',27274),(15375,'Brazzed',2492),(15375,'DD',13899),(15375,'Waterfall',15286),(15375,'Ranger',5408),(15375,'Xanesla',6301),(15375,'fog',13280);
UNLOCK TABLES;
/*!40000 ALTER TABLE `ship_destinations` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

