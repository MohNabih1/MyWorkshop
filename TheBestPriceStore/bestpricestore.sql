-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 18, 2024 at 10:13 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `bestpricestore`
--

-- --------------------------------------------------------

--
-- Table structure for table `contract`
--

CREATE TABLE `contract` (
  `vId` int(11) NOT NULL,
  `ctId` int(11) NOT NULL,
  `Sdate` date DEFAULT NULL,
  `Ctime` varchar(255) DEFAULT NULL,
  `Ctname` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `contract`
--

INSERT INTO `contract` (`vId`, `ctId`, `Sdate`, `Ctime`, `Ctname`) VALUES
(2, 1, '2024-06-24', '13:00:00', 'Buddies'),
(3, 1, '2024-06-25', '15:00:00', 'Greens'),
(4, 1, '2024-06-25', '16:00:00', 'Certifications'),
(5, 1, '2024-07-26', '13:00:00', 'Certifications'),
(2, 2, '2024-01-07', '13:00:00', ' Buddies'),
(4, 2, '2024-06-26', '16:00:00', 'Certifications'),
(5, 2, '2024-07-26', '14:00:00', ' Buddies'),
(2, 3, '2024-07-07', '13:00:00', 'Buddies'),
(5, 3, '2024-07-26', '15:00:00', 'Sandwiches'),
(2, 4, '2024-07-14', '14:00:00', 'Sandwiches'),
(5, 4, '2024-07-26', '16:00:00', 'Sandwiches'),
(5, 5, '2024-07-26', '17:00:00', 'Microgreens');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE `customer` (
  `cId` int(11) NOT NULL,
  `Cname` varchar(255) NOT NULL,
  `Street` varchar(255) DEFAULT NULL,
  `City` varchar(255) DEFAULT NULL,
  `StateAb` char(2) DEFAULT NULL,
  `Zipcode` varchar(255) DEFAULT NULL,
  `CtelNo` varchar(255) DEFAULT NULL,
  `Cdob` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`cId`, `Cname`, `Street`, `City`, `StateAb`, `Zipcode`, `CtelNo`, `Cdob`) VALUES
(1, 'Abed Abdi', '309 Hcounty Road', 'Abbott', 'TX', '76621-0057', '5046218927', '1998-02-01'),
(2, 'Ismail Gulg', '405 E Mesquite Street', 'Abbott', 'TX', '76621-0057', '8102929388', '1996-12-22'),
(3, 'Shakir Ali', '5000 Spectrum Street', 'Addison', 'TX', '75001-6880', '8566368749', '1992-06-07'),
(4, 'Abdur Rahman', '1000 County Road', 'Bradley', 'OK', '73011-0121', '9073854412', '1989-08-03'),
(5, 'Kalipada Ghoshal', 'Boundary Road', 'Bradley', 'OK', '73011-9600', '5135701893', '1995-04-02'),
(6, 'Manishi Dey', '450 Main Street', 'Mound City', 'KS', '66056-0001', '4195032484', '1983-09-25'),
(7, 'Nandalal Bose', 'S Metcalf Road', 'Louisburg', 'KS', '66053-0541', '7735736914', '1999-10-21'),
(8, 'Raja Ravi Varma', '2000 Forest Grove blvd', 'Allen', 'TX', '75002-8811', '4087523500', '1999-02-19'),
(9, 'Sunil Das', '4000 Woodcreek Road', 'Carrollton', 'TX', '75006-1911', '6054142147', '1982-04-01'),
(10, 'Jasper Johns', '2500 Sunset Ridge Drive', 'Rockwall', 'TX', '75032-0006', '4106558723', '1973-05-08'),
(11, 'Winslow Homer', '11300 Juniper Lane', 'Irving', 'TX', '75039-0101', '2158741229', '1968-07-18'),
(12, 'Albert Bierstadt', '400 Spruce Street', 'Leavenworth', 'KS', '66048-0001', '6313353414', '2000-11-27'),
(13, 'Edward Hopper', '1500 255th Street', 'Hillsdale', 'KS', '66036-0061', '3104985651', '1966-03-05'),
(14, 'Georgia O\'Keeffe', '3000 Weiss Lane', 'Irving', 'TX', '75039-0006', '4407808425', '1963-04-02'),
(15, 'Modupeola Fadugba', '1112 18TH Street', 'Plano', 'TX', '75086-0019', '9565376195', '2000-05-10'),
(16, 'Ekene Maduka', '15TH Street', 'Plano', 'TX', '75086-0015', '6022774385', '1994-03-21'),
(17, 'Olu Amoda', '200 Travis Street', 'Sherman', 'TX', '75090-0005', '9313139635', '1999-06-12'),
(18, 'Koki Tanaka', '5000 W Holiday Road', 'Fate', 'TX', '75087-2136', '4146619598', '1999-09-09'),
(19, 'Tatsuo Miyajima', 'Bella Ranch Drive', 'Choctaw', 'OK', '73020-0017', '3132887937', '2002-02-10'),
(20, 'Li Chen', '2090 County Road', 'Cement', 'OK', '73017-1100', '8158282147', '1963-07-08'),
(21, 'Zhan Wang', '1600 N Santa Fe', 'Edmond', 'OK', '73003-3661', '6105453615', '1984-09-10');

-- --------------------------------------------------------

--
-- Table structure for table `employee`
--

CREATE TABLE `employee` (
  `sId` int(11) NOT NULL,
  `SSN` varchar(255) NOT NULL,
  `Ename` varchar(255) DEFAULT NULL,
  `Street` varchar(255) DEFAULT NULL,
  `City` varchar(255) DEFAULT NULL,
  `StateAb` char(2) DEFAULT NULL,
  `Zipcode` varchar(255) DEFAULT NULL,
  `Etype` varchar(255) DEFAULT NULL,
  `Bdate` date DEFAULT NULL,
  `Sdate` date DEFAULT NULL,
  `Edate` date DEFAULT NULL,
  `eLevel` varchar(255) DEFAULT NULL,
  `Asalary` decimal(10,2) DEFAULT NULL,
  `Agency` varchar(255) DEFAULT NULL,
  `Hsalary` decimal(10,2) DEFAULT NULL,
  `Institute` varchar(255) DEFAULT NULL,
  `Itype` varchar(255) DEFAULT NULL,
  `TelNo` varchar(255) DEFAULT NULL,
  `Email` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `employee`
--

INSERT INTO `employee` (`sId`, `SSN`, `Ename`, `Street`, `City`, `StateAb`, `Zipcode`, `Etype`, `Bdate`, `Sdate`, `Edate`, `eLevel`, `Asalary`, `Agency`, `Hsalary`, `Institute`, `Itype`, `TelNo`, `Email`) VALUES
(1, '123456666', 'Olampiosi Pi', '100 Varsity Circle', 'Arlington', 'TX', '76019-1111', 'T', '2005-02-01', '2023-01-20', NULL, NULL, NULL, 'Top Performers', 10.00, NULL, NULL, '4145737719', 'olampiosi.pi@gmail.com'),
(1, '123456777', 'Radha Gupta', '200 Nedderman Dr', 'Arlington', 'TX', '76019-1111', 'T', '2005-01-20', '2023-01-20', NULL, NULL, 20000.00, 'AR Manpower', 10.00, NULL, NULL, '3038745160', 'radhagupta@gmail.com'),
(1, '123456780', 'Cao Peng', '12 W. Mitchell St.', 'Arlington', 'TX', '76019-1111', 'I', '2008-01-20', '2023-01-20', NULL, NULL, NULL, NULL, NULL, 'UTA', 'U', '2124629157', 'pengcao@gmil.com'),
(1, '123456781', 'Giun Seong', '12 S. Pecan St.', 'Arlington', 'TX', '76019-1111', 'I', '2007-01-01', '2023-01-20', NULL, NULL, NULL, NULL, NULL, 'Arlington High School ', 'S', '9078732882', 'giun.seong@gmail.com'),
(1, '123456788', 'Christian Cart', '20 Main Street', 'Arlington', 'TX', '76019-1111', 'F', '2001-01-02', '2023-01-20', NULL, 'Associate 1', 25000.00, NULL, NULL, NULL, NULL, '5128613814', 'christian.cart@gmail.com'),
(1, '123456789', 'Mary Durer', '11 Cooper Street', 'Arlington', 'TX', '76019-1111', 'F', '2000-01-20', '2023-01-20', NULL, 'Associate 2', 30000.00, NULL, NULL, NULL, NULL, '7329042931', 'marydurer@gmail.com');

-- --------------------------------------------------------

--
-- Table structure for table `feedback`
--

CREATE TABLE `feedback` (
  `cId` int(11) NOT NULL,
  `rId` int(11) NOT NULL,
  `Rating` int(11) DEFAULT NULL,
  `Comments` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `feedback`
--

INSERT INTO `feedback` (`cId`, `rId`, `Rating`, `Comments`) VALUES
(1, 1, 4, 'Pasta varieties are good'),
(1, 2, 5, 'Vegetables are very fresh'),
(1, 3, 2, 'Fish is very expensive and not even fresh'),
(2, 1, 3, 'Beans are not fresh'),
(2, 2, 4, 'Employees are well-behaved and most of the items are of very good quality '),
(3, 1, 5, 'Excellent Quality and varieties'),
(4, 1, 1, 'High prices and very less quantity'),
(4, 2, 2, 'Not fresh'),
(5, 1, 3, 'Fresh and Healthy fruits'),
(5, 2, 4, 'Good'),
(6, 1, 5, 'Good Quality. Will buy more.'),
(17, 1, 5, 'Fresh'),
(18, 1, 2, 'Less Variety of lentlils'),
(18, 2, 4, 'Liked Chickpeas '),
(21, 1, 5, 'Really worth it.'),
(21, 2, 4, 'Good Varieties of oils  but little expensive on Olive oil'),
(21, 3, 4, 'Definitely recommend everyone to buy.'),
(21, 4, 3, 'Need to maintain more stock on meat products');

-- --------------------------------------------------------

--
-- Table structure for table `item`
--

CREATE TABLE `item` (
  `iId` int(11) NOT NULL,
  `Iname` varchar(255) NOT NULL,
  `Sprice` decimal(10,2) DEFAULT NULL,
  `Idescription` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `item`
--

INSERT INTO `item` (`iId`, `Iname`, `Sprice`, `Idescription`) VALUES
(1, 'Broccoli ', 3.00, ' Vitamin A,C,K'),
(2, 'Kale ', 3.00, ' Vitamin A,C,K'),
(3, 'Alfalfa', 3.00, 'Vitamin A,C,K'),
(4, 'Mung', 4.00, 'Vitamin C,K'),
(5, 'Chickpeas', 4.00, 'Vitamin C,K'),
(6, 'Onion ', 4.00, ' Vitamin C,K'),
(7, 'Soyabean', 5.00, 'Vitamin C,K'),
(8, 'Clover', 3.00, 'Vitamin A,C,K'),
(9, 'Kidney Beans', 4.00, 'Vitamin C,K'),
(10, 'Adzuki Bean ', 4.00, 'Vitamin A,C,K'),
(11, 'Beet', 5.00, 'Vitamin A,C'),
(12, 'Green Pea', 5.00, 'Vitamin A,C,K'),
(13, 'Radish', 6.00, 'Vitamin A,C,K'),
(14, 'Lentil', 4.00, 'Vitamin A,C,K');

-- --------------------------------------------------------

--
-- Table structure for table `orders`
--

CREATE TABLE `orders` (
  `oId` int(11) NOT NULL,
  `sId` int(11) NOT NULL,
  `Odate` date DEFAULT NULL,
  `Ddate` date DEFAULT NULL,
  `Amount` decimal(10,2) DEFAULT NULL,
  `cId` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `orders`
--

INSERT INTO `orders` (`oId`, `sId`, `Odate`, `Ddate`, `Amount`, `cId`) VALUES
(1, 1, '2023-01-20', '2023-01-20', 2023.00, 3),
(2, 1, '2023-01-20', '2023-01-20', 2023.00, 3),
(3, 1, '2023-01-20', '2023-01-20', 2023.00, 4),
(4, 1, '2023-01-20', '2023-01-20', 2023.00, 12),
(5, 1, '2023-01-20', '2023-01-20', 2023.00, 4),
(6, 1, '2023-01-20', '2023-01-20', 2023.00, 27),
(7, 1, '2023-01-20', '2023-01-20', 2023.00, 4),
(8, 1, '2023-01-20', '2023-01-20', 2023.00, 5),
(9, 1, '2023-01-20', '2023-01-20', 2023.00, 5),
(10, 1, '2023-01-20', '2023-01-20', 2023.00, 5),
(11, 1, '2023-01-20', '2023-01-20', 2023.00, 3),
(12, 1, '2023-01-25', '2023-01-25', 2023.00, 4),
(13, 1, '2023-01-25', '2023-01-25', 2023.00, 4),
(14, 1, '2023-01-25', '2023-01-25', 2023.00, 4),
(15, 1, '2023-01-25', '2023-01-25', 2023.00, 22),
(16, 1, '2023-01-25', '2023-01-25', 2023.00, 5),
(17, 1, '2023-01-25', '2023-01-25', 2023.00, 6),
(18, 1, '2023-01-30', '2023-01-30', 2023.00, 6),
(19, 1, '2023-01-30', '2023-01-30', 2023.00, 3),
(20, 1, '2023-01-30', '2023-01-30', 2023.00, 3),
(21, 1, '2023-01-30', '2023-01-30', 2023.00, 4),
(22, 1, '2023-01-30', '2023-01-30', 2023.00, 4),
(23, 1, '2023-02-05', '2023-02-05', 2023.00, 4),
(24, 1, '2023-02-05', '2023-02-05', 2023.00, 4),
(25, 1, '2023-02-05', '2023-02-05', 2023.00, 4),
(26, 1, '2023-02-10', '2023-02-10', 2023.00, 4),
(27, 1, '2023-02-12', '2023-02-12', 2023.00, 4);

-- --------------------------------------------------------

--
-- Table structure for table `order_item`
--

CREATE TABLE `order_item` (
  `oId` int(11) NOT NULL,
  `sId` int(11) NOT NULL,
  `iId` int(11) NOT NULL,
  `Icount` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `order_item`
--

INSERT INTO `order_item` (`oId`, `sId`, `iId`, `Icount`) VALUES
(1, 1, 1, 1),
(2, 1, 2, 1),
(3, 1, 4, 1),
(4, 1, 5, 1),
(4, 1, 6, 2),
(5, 1, 5, 1),
(6, 1, 5, 1),
(6, 1, 6, 2),
(6, 1, 7, 3),
(7, 1, 6, 1),
(8, 1, 7, 1),
(9, 1, 7, 1),
(10, 1, 7, 1),
(11, 1, 8, 1),
(12, 1, 9, 1),
(13, 1, 10, 1),
(14, 1, 10, 1),
(15, 1, 4, 1),
(15, 1, 5, 1),
(15, 1, 6, 1),
(15, 1, 7, 2),
(16, 1, 12, 1),
(17, 1, 13, 1),
(18, 1, 13, 1),
(19, 1, 1, 1),
(20, 1, 2, 1),
(21, 1, 4, 1),
(22, 1, 4, 1),
(23, 1, 4, 1),
(24, 1, 4, 1),
(25, 1, 5, 1),
(26, 1, 5, 1),
(27, 1, 14, 1);

-- --------------------------------------------------------

--
-- Table structure for table `store`
--

CREATE TABLE `store` (
  `sId` int(11) NOT NULL,
  `Sname` varchar(255) NOT NULL,
  `Street` varchar(255) DEFAULT NULL,
  `City` varchar(255) DEFAULT NULL,
  `StateAb` char(2) DEFAULT NULL,
  `Zipcode` varchar(255) DEFAULT NULL,
  `Sdate` date DEFAULT NULL,
  `Telno` varchar(255) DEFAULT NULL,
  `URL` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `store`
--

INSERT INTO `store` (`sId`, `Sname`, `Street`, `City`, `StateAb`, `Zipcode`, `Sdate`, `Telno`, `URL`) VALUES
(1, 'Best Price Grocery-1', '1 Cooper Street', 'Arlington', 'TX', '76019-0012', '2023-10-01', '8172721111', 'www.BestPriceGrocery-1.com');

-- --------------------------------------------------------

--
-- Table structure for table `store_customer`
--

CREATE TABLE `store_customer` (
  `sId` int(11) NOT NULL,
  `cId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `store_customer`
--

INSERT INTO `store_customer` (`sId`, `cId`) VALUES
(1, 1),
(1, 2),
(1, 3),
(1, 4),
(1, 5),
(1, 6),
(1, 7),
(1, 8),
(1, 9),
(1, 10),
(1, 11),
(1, 12),
(1, 13),
(1, 14),
(1, 15),
(1, 16),
(1, 17),
(1, 18),
(1, 19),
(1, 20),
(1, 21);

-- --------------------------------------------------------

--
-- Table structure for table `store_item`
--

CREATE TABLE `store_item` (
  `sId` int(11) NOT NULL,
  `iId` int(11) NOT NULL,
  `Scount` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `store_item`
--

INSERT INTO `store_item` (`sId`, `iId`, `Scount`) VALUES
(1, 1, 10),
(1, 2, 10),
(1, 3, 10),
(1, 4, 10),
(1, 5, 10),
(1, 6, 10),
(1, 7, 10),
(1, 8, 10),
(1, 9, 10),
(1, 10, 12),
(1, 11, 13),
(1, 12, 10),
(1, 13, 16),
(1, 14, 15);

-- --------------------------------------------------------

--
-- Table structure for table `vendor`
--

CREATE TABLE `vendor` (
  `vId` int(11) NOT NULL,
  `Vname` varchar(255) NOT NULL,
  `Street` varchar(255) DEFAULT NULL,
  `City` varchar(255) DEFAULT NULL,
  `StateAb` char(2) DEFAULT NULL,
  `Zipcode` varchar(255) DEFAULT NULL,
  `Vemail` varchar(255) DEFAULT NULL,
  `VTelNo` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `vendor`
--

INSERT INTO `vendor` (`vId`, `Vname`, `Street`, `City`, `StateAb`, `Zipcode`, `Vemail`, `VTelNo`) VALUES
(1, 'Organic Nature', '1500 E Exchange Road', 'Allen', 'TX', '75002-4504', 'organic.nature@gmail.com', '9127731567'),
(2, 'Green Valley', '1405 Julian Street', 'Addison', 'TX', '75001-4633', 'green.valley@gmail.com', '8659432879'),
(3, 'Green Mountains', '800 Preston Road', 'Gunter', 'TX', '75058-0001', 'green.mountains@gmail.com', '4695308000'),
(4, 'Whole Foods', '1010 Spicewood Drive', 'Garland', 'TX', '75044-2569', 'whole.foods@gmail.com', '3129874562'),
(5, 'LA Queen', '500 Angi Road', 'Minden', 'LA', '71055-1004', 'la.queen@gmail.com', '5429873904'),
(6, 'Castor Sprouts', '100 New Ramah Road', 'Castor', 'LA', '71016-2402', 'castor.sprouts@gmail.com', '6700042715');

-- --------------------------------------------------------

--
-- Table structure for table `vendor_item`
--

CREATE TABLE `vendor_item` (
  `vId` int(11) NOT NULL,
  `iId` int(11) NOT NULL,
  `Vprice` decimal(10,2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `vendor_item`
--

INSERT INTO `vendor_item` (`vId`, `iId`, `Vprice`) VALUES
(1, 1, 2.00),
(1, 2, 2.00),
(1, 3, 1.00),
(1, 14, 1.00),
(2, 4, 3.00),
(2, 5, 2.00),
(2, 6, 2.00),
(2, 13, 4.00),
(3, 7, 3.00),
(3, 8, 2.00),
(3, 9, 2.00),
(3, 10, 2.00),
(4, 11, 3.00),
(4, 12, 3.00),
(4, 13, 2.00),
(4, 14, 2.00),
(5, 7, 3.00),
(5, 11, 2.00),
(5, 13, 5.00),
(5, 14, 3.00),
(6, 7, 2.00),
(6, 8, 1.00),
(6, 9, 3.00),
(6, 10, 4.00);

-- --------------------------------------------------------

--
-- Table structure for table `vendor_store`
--

CREATE TABLE `vendor_store` (
  `vId` int(11) NOT NULL,
  `sId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `vendor_store`
--

INSERT INTO `vendor_store` (`vId`, `sId`) VALUES
(1, 1),
(2, 1),
(3, 1),
(4, 1),
(5, 1),
(6, 1);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `contract`
--
ALTER TABLE `contract`
  ADD PRIMARY KEY (`ctId`,`vId`),
  ADD KEY `vId` (`vId`);

--
-- Indexes for table `customer`
--
ALTER TABLE `customer`
  ADD PRIMARY KEY (`cId`);

--
-- Indexes for table `employee`
--
ALTER TABLE `employee`
  ADD PRIMARY KEY (`SSN`),
  ADD KEY `sId` (`sId`);

--
-- Indexes for table `feedback`
--
ALTER TABLE `feedback`
  ADD PRIMARY KEY (`cId`,`rId`);

--
-- Indexes for table `item`
--
ALTER TABLE `item`
  ADD PRIMARY KEY (`iId`);

--
-- Indexes for table `orders`
--
ALTER TABLE `orders`
  ADD PRIMARY KEY (`oId`,`sId`),
  ADD KEY `sId` (`sId`);

--
-- Indexes for table `order_item`
--
ALTER TABLE `order_item`
  ADD PRIMARY KEY (`oId`,`sId`,`iId`),
  ADD KEY `sId` (`sId`),
  ADD KEY `iId` (`iId`);

--
-- Indexes for table `store`
--
ALTER TABLE `store`
  ADD PRIMARY KEY (`sId`);

--
-- Indexes for table `store_customer`
--
ALTER TABLE `store_customer`
  ADD PRIMARY KEY (`sId`,`cId`),
  ADD KEY `cId` (`cId`);

--
-- Indexes for table `store_item`
--
ALTER TABLE `store_item`
  ADD PRIMARY KEY (`sId`,`iId`),
  ADD KEY `iId` (`iId`);

--
-- Indexes for table `vendor`
--
ALTER TABLE `vendor`
  ADD PRIMARY KEY (`vId`);

--
-- Indexes for table `vendor_item`
--
ALTER TABLE `vendor_item`
  ADD PRIMARY KEY (`vId`,`iId`),
  ADD KEY `iId` (`iId`);

--
-- Indexes for table `vendor_store`
--
ALTER TABLE `vendor_store`
  ADD PRIMARY KEY (`vId`,`sId`),
  ADD KEY `sId` (`sId`);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `contract`
--
ALTER TABLE `contract`
  ADD CONSTRAINT `contract_ibfk_1` FOREIGN KEY (`vId`) REFERENCES `vendor` (`vId`);

--
-- Constraints for table `employee`
--
ALTER TABLE `employee`
  ADD CONSTRAINT `employee_ibfk_1` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`);

--
-- Constraints for table `feedback`
--
ALTER TABLE `feedback`
  ADD CONSTRAINT `feedback_ibfk_1` FOREIGN KEY (`cId`) REFERENCES `customer` (`cId`);

--
-- Constraints for table `orders`
--
ALTER TABLE `orders`
  ADD CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`);

--
-- Constraints for table `order_item`
--
ALTER TABLE `order_item`
  ADD CONSTRAINT `order_item_ibfk_1` FOREIGN KEY (`oId`) REFERENCES `orders` (`oId`),
  ADD CONSTRAINT `order_item_ibfk_2` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`),
  ADD CONSTRAINT `order_item_ibfk_3` FOREIGN KEY (`iId`) REFERENCES `item` (`iId`);

--
-- Constraints for table `store_customer`
--
ALTER TABLE `store_customer`
  ADD CONSTRAINT `store_customer_ibfk_1` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`),
  ADD CONSTRAINT `store_customer_ibfk_2` FOREIGN KEY (`cId`) REFERENCES `customer` (`cId`);

--
-- Constraints for table `store_item`
--
ALTER TABLE `store_item`
  ADD CONSTRAINT `store_item_ibfk_1` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`),
  ADD CONSTRAINT `store_item_ibfk_2` FOREIGN KEY (`iId`) REFERENCES `item` (`iId`);

--
-- Constraints for table `vendor_item`
--
ALTER TABLE `vendor_item`
  ADD CONSTRAINT `vendor_item_ibfk_1` FOREIGN KEY (`vId`) REFERENCES `vendor` (`vId`),
  ADD CONSTRAINT `vendor_item_ibfk_2` FOREIGN KEY (`iId`) REFERENCES `item` (`iId`);

--
-- Constraints for table `vendor_store`
--
ALTER TABLE `vendor_store`
  ADD CONSTRAINT `vendor_store_ibfk_1` FOREIGN KEY (`vId`) REFERENCES `vendor` (`vId`),
  ADD CONSTRAINT `vendor_store_ibfk_2` FOREIGN KEY (`sId`) REFERENCES `store` (`sId`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
