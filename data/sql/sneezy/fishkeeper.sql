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
-- Table structure for table `fishkeeper`
--

DROP TABLE IF EXISTS `fishkeeper`;
CREATE TABLE `fishkeeper` (
  `name` varchar(80) NOT NULL default '',
  `weight` double default NULL,
  KEY `ix_fishkeeper_name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `fishkeeper`
--


/*!40000 ALTER TABLE `fishkeeper` DISABLE KEYS */;
LOCK TABLES `fishkeeper` WRITE;
INSERT INTO `fishkeeper` VALUES ('Bumali',3350.58),('Kriek',65327.2),('Dunlen',9444.93),('Stump',40.62),('Nicodemus',57342.37),('Tasha',9935.67),('Flasher',259.87),('Ceres',5673.89),('Kiayla',13.13),('Haoma',62.8),('Octogal',2960.17),('Colossus',2073.62),('Mastyr',265.55),('Kualteck',485.52),('Halucin',36966.16),('Alanna',166.52),('Wynde',214.79),('Herbals',323.4),('Loco',32),('Ragamuffin',74.72),('Feesh',289.06),('GrassHopper',6.4),('Clork',14.129999),('Disciple',157.61),('Erasamus',7.45),('testmob l1',600),('Bump',198.69),('Rouge',752.34),('Claria',132.23),('Orange',77.62),('Garissa',459.46),('Cirk',176767.350028),('Acid',198.400002),('Balrog',1203.439998),('Slim',44523.94),('Trina',848.080001),('IRS',7485.08000400001),('Lidda',46.33),('StealthAssasin',115.4),('Melisandre',341.779995),('Underpants',22.54),('Sanosuke',3872.980006),('Claw',3963.950007),('Bumblebee',842.890002),('Alucard',1359.770001),('Tyson',7540.26002100001),('Mage',243.579998),('OgreZ',393.469994),('Adoresqua',976.339999),('Quantum',710.990002),('Eeize',40.17),('Beloc',241.590003),('Magnito',66674.180045),('Pitch',10801.720002),('Leri',718.799998),('Peel',91.9),('Birch',21130.720003),('Garlic',9572.77998800001),('Isegrim',211.359997),('Grogg',10773.889989),('Shadow',2636.620007),('Arkane',120621.360173),('Thelonious',2842.59),('DarkMagic',3195.870006),('Micky',8652.900045),('Ginger',145.249999),('Aurea',21341.480058),('Gauge',247837.930191),('Shatter',466.410005),('Frobozz',45804.899968),('Deleep',1048.200002),('Laren',4067429.77180491),('Loony',3243.190006),('Trapium',2019.840009),('Thief',948.180001),('Zigzag',112484.029988),('Sephie',1135.070008),('Pirvan',136.509999),('Aenima',138.61),('Azor',2546.49),('Koala',7299.140005),('Kelranth',7030.460002),('Pobre',218.679998),('Stabby',282.530002),('Jiraiya',696.380005),('Treebeard',18.190001),('Ragnar',186.229999),('Brennar',33173.040017),('Chemosh',58.889999),('Komat',2931.939994),('Baffleflap',3334.779995),('Despair',22154.520018),('Behren',116.990001),('Millie',49.130001),('minotaur Sage switched',44.400002),('Lastime',6377.070018),('Weps',39852.1300039999),('Immorality',512.750003),('Skie',33.299999),('Pinch',9.26),('Knuckles',24383.170013),('Sidartha',6758.29999399999),('Spew',200424.800005001),('Kagato',12586.519969),('Releep',7414.57000700001),('Pragadim',91743.8800000001),('Trenaar',3412.880011),('Theef',26528.690026),('Nutter',1181.810005),('Lucivar',185274.549953),('Shandon',74137.1600499999),('Burglar',2976.219994),('Thrash',84505.5799979997),('Deceiver',400.519998),('Shabbadabba',214.189994),('Thedarkone',688.400003),('Stabassius',105.109995),('Terror',266085.840170999),('Nordic',1977.20001),('Grimm',13917.940024),('Azays',32777.179952),('Warehouse',188.460001),('Lorrheek',40113.4400280001),('Sable',20.45),('Dodgy',1002.890006),('SupaFly',393.109997),('BlackMage',83.860004),('Ecarg',217.809999),('MoFugger',2375.960019),('Sturgeon',799.530002),('Smashy',664.80001),('Freedom',231.090001),('Medic',652.770002),('Killjoy',2709.429998),('Uryll',290.599999),('minotaur Raven switched',3109.89999),('Mudbutt',69.049998),('Daffy',766.530002),('Himalay',455.990001),('Burnicus',494.649999),('KhulShalkrum',83771.3499609998),('Aiolos',6547.38001),('Raven',309.170001),('Niteshadow',10180.719994),('Micelli',3695.879994),('String',78.959999),('Petrovich',241.169999),('Aion',7659.830006),('Gon',12149.180012),('Narayan',588.830003),('Killroy',8.72),('Lichen',73.580001),('Axiom',130.059999),('Vorch',86.3),('Rekres',81.179999),('Merkaba',106.85),('Antonius',20310.37001),('Delvin',110260.57006),('Sage',4142273.37141091),('Cious',53455.730033),('Tenbutts',1335280.23038899),('OHgre',2215.160002),('Strife',5411.409992),('Rock',8053.350009),('Rhine',10483.349983),('Callon',45.03),('Delanea',38332.970005),('Zukeeni',166.520002),('Metrohep',230),('Trawl',108.499996),('Fireheart',2357.299992),('Astaldo',246.659998),('Intuition',13.32),('Cronus',175.07),('Oscar',524.490002),('Fritz',21102.040001),('Flex',4.5),('Wretch',383.990001),('Rikorosh',16.85),('Arlen',971.100001),('Spine',161.980001),('TheSaint',1153.570006),('Loa',740.360009),('Talipia',6.6),('Artur',1566.640006),('Hood',14.62),('Gaara',808.389999),('Ganondorf',1086.840004),('Nova',2327.450015),('Mole',365.520001),('Climbatize',54.820002);
UNLOCK TABLES;
/*!40000 ALTER TABLE `fishkeeper` ENABLE KEYS */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

