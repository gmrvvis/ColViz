--Copyright (c) 2017 CCS/GMRV/UPM/URJC.
--
--Author: Juan P. Brito <juanpedro.brito@upm.es>
--
--This library is free software; you can redistribute it and/or modify it under
--the terms of the GNU Lesser General Public License version 3.0 as published
--by the Free Software Foundation.
--
--This library is distributed in the hope that it will be useful, but WITHOUT
--ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
--FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
--details.
--
--You should have received a copy of the GNU Lesser General Public License
--along with this library; if not, write to the Free Software Foundation, Inc.,
--51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


-- phpMyAdmin SQL Dump
-- version 4.6.6deb5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Mar 05, 2018 at 05:59 PM
-- Server version: 10.1.26-MariaDB-1
-- PHP Version: 7.1.8-1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `ColVizDB`
--

-- --------------------------------------------------------

--
-- Table structure for table `DATAEXPERIMENT`
--

CREATE TABLE `DATAEXPERIMENT` (
  `ID` int(11) NOT NULL,
  `EXPERIMENTID` int(11) NOT NULL,
  `NOTEEXPERIMENT` varchar(3000) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `DIALOGS`
--

CREATE TABLE `DIALOGS` (
  `ID` int(11) NOT NULL,
  `MESSAGE` varchar(3000) DEFAULT NULL,
  `USERID` int(11) NOT NULL,
  `SESSIONID` int(11) NOT NULL,
  `MESSAGETIME` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `DIALOGS`
--

INSERT INTO `DIALOGS` (`ID`, `MESSAGE`, `USERID`, `SESSIONID`, `MESSAGETIME`) VALUES
(1, 'This is a collaborative visualization test', 1, 1, '2018-03-05 16:24:47'),
(2, 'Ok, this looks nice', 2, 1, '2018-03-05 16:24:47'),
(3, 'Of course :)', 1, 1, '2018-03-05 16:24:47'),
(4, 'Lets do some work', 2, 1, '2018-03-05 16:24:47'),
(5, 'Ok, please select the piramidal neurons and send me the selection', 1, 1, '2018-03-05 16:24:47'),
(6, 'Done!', 2, 1, '2018-03-05 16:24:47'),
(7, 'Thanks', 1, 1, '2018-03-05 16:24:47');

-- --------------------------------------------------------

--
-- Table structure for table `EXPERIMENTS`
--

CREATE TABLE `EXPERIMENTS` (
  `ID` int(11) NOT NULL,
  `DESCRIPTION` varchar(3000) DEFAULT NULL,
  `TYPESOFEXPERIMENTSID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `EXPERIMENTS`
--

INSERT INTO `EXPERIMENTS` (`ID`, `DESCRIPTION`, `TYPESOFEXPERIMENTSID`) VALUES
(1, 'Inters and Pirams excitation', 2),
(2, 'Apical neurite analysis', 1);

-- --------------------------------------------------------

--
-- Table structure for table `SESSIONS`
--

CREATE TABLE `SESSIONS` (
  `ID` int(11) NOT NULL,
  `DESCRIPTION` text,
  `USERID` int(11) NOT NULL,
  `EXPERIMENTID` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `SESSIONS`
--

INSERT INTO `SESSIONS` (`ID`, `DESCRIPTION`, `USERID`, `EXPERIMENTID`) VALUES
(1, 'Simulation of inters and pirams', 1, 1),
(2, 'Apical morpho analysis', 1, 2);

-- --------------------------------------------------------

--
-- Table structure for table `TYPESOFEXPERIMENTS`
--

CREATE TABLE `TYPESOFEXPERIMENTS` (
  `ID` int(11) NOT NULL,
  `TYPEOFEXPERIMENT` varchar(300) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `TYPESOFEXPERIMENTS`
--

INSERT INTO `TYPESOFEXPERIMENTS` (`ID`, `TYPEOFEXPERIMENT`) VALUES
(1, 'Morphological'),
(2, 'Simulation');

-- --------------------------------------------------------

--
-- Table structure for table `USERS`
--

CREATE TABLE `USERS` (
  `ID` int(11) NOT NULL,
  `NAME` varchar(100) DEFAULT NULL,
  `SURNAME` varchar(100) DEFAULT NULL,
  `EMAIL` varchar(100) DEFAULT NULL,
  `USERPASSWORD` varchar(100) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `USERS`
--

INSERT INTO `USERS` (`ID`, `NAME`, `SURNAME`, `EMAIL`, `USERPASSWORD`) VALUES
(1, 'juan', 'BM', 'juanbm@something.com', '1234'),
(2, 'pepe', 'wert', 'pepe@something.com', '1234'),
(3, 'antonio', 'asdfg', 'anto@something.com', '1234');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `DATAEXPERIMENT`
--
ALTER TABLE `DATAEXPERIMENT`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `EXPERIMENTID` (`EXPERIMENTID`);

--
-- Indexes for table `DIALOGS`
--
ALTER TABLE `DIALOGS`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `USERID` (`USERID`),
  ADD KEY `SESSIONID` (`SESSIONID`);

--
-- Indexes for table `EXPERIMENTS`
--
ALTER TABLE `EXPERIMENTS`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `SESSIONS`
--
ALTER TABLE `SESSIONS`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `EXPERIMENTID` (`EXPERIMENTID`);

--
-- Indexes for table `TYPESOFEXPERIMENTS`
--
ALTER TABLE `TYPESOFEXPERIMENTS`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `USERS`
--
ALTER TABLE `USERS`
  ADD PRIMARY KEY (`ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `DATAEXPERIMENT`
--
ALTER TABLE `DATAEXPERIMENT`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `DIALOGS`
--
ALTER TABLE `DIALOGS`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT for table `EXPERIMENTS`
--
ALTER TABLE `EXPERIMENTS`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `SESSIONS`
--
ALTER TABLE `SESSIONS`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `TYPESOFEXPERIMENTS`
--
ALTER TABLE `TYPESOFEXPERIMENTS`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT for table `USERS`
--
ALTER TABLE `USERS`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
--
-- Constraints for dumped tables
--

--
-- Constraints for table `DATAEXPERIMENT`
--
ALTER TABLE `DATAEXPERIMENT`
  ADD CONSTRAINT `DATAEXPERIMENT_ibfk_1` FOREIGN KEY (`EXPERIMENTID`) REFERENCES `EXPERIMENTS` (`ID`);

--
-- Constraints for table `DIALOGS`
--
ALTER TABLE `DIALOGS`
  ADD CONSTRAINT `DIALOGS_ibfk_1` FOREIGN KEY (`USERID`) REFERENCES `USERS` (`ID`),
  ADD CONSTRAINT `DIALOGS_ibfk_2` FOREIGN KEY (`SESSIONID`) REFERENCES `SESSIONS` (`ID`);

--
-- Constraints for table `SESSIONS`
--
ALTER TABLE `SESSIONS`
  ADD CONSTRAINT `SESSIONS_ibfk_1` FOREIGN KEY (`EXPERIMENTID`) REFERENCES `EXPERIMENTS` (`ID`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
