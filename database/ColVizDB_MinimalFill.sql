-- Copyright (c) 2017 CCS/GMRV/UPM/URJC.
-- 
-- Author: Juan P. Brito <juanpedro.brito@upm.es>
-- 
-- This library is free software; you can redistribute it and/or modify it under
-- the terms of the GNU Lesser General Public License version 3.0 as published
-- by the Free Software Foundation.
-- 
-- This library is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
-- FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
-- details.
-- 
-- You should have received a copy of the GNU Lesser General Public License
-- along with this library; if not, write to the Free Software Foundation, Inc.,
-- 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

USE `ColVizDB`;

--
-- Test content for tables
--

INSERT INTO `USERS` (`ID`, `NAME`, `SURNAME`, `EMAIL`, `USERPASSWORD`) VALUES
(1, 'juan', 'asdf', 'juan@something.com', '1234'),
(2, 'pepe', 'qwert', 'pepe@something.com', '1234'),
(3, 'antonio', 'zxcvb', 'anto@something.com', '1234');

-- --------------------------------------------------------

INSERT INTO `TYPESOFEXPERIMENTS` (`ID`, `TYPEOFEXPERIMENT`) VALUES
(1, 'Morphological'),
(2, 'Simulation');

-- --------------------------------------------------------

INSERT INTO `EXPERIMENTS` (`ID`, `DESCRIPTION`, `TYPESOFEXPERIMENTSID`) VALUES
(1, 'Inters and Pirams excitation', 2),
(2, 'Apical neurite analysis', 1);

-- --------------------------------------------------------

INSERT INTO `SESSIONS` (`ID`, `DESCRIPTION`, `USERID`, `EXPERIMENTID`) VALUES
(1, 'Simulation of inters and pirams', 1, 1),
(2, 'Apical morpho analysis', 1, 2);

-- --------------------------------------------------------

INSERT INTO `DIALOGS` (`ID`, `MESSAGE`, `USERID`, `SESSIONID`, `MESSAGETIME`) VALUES 
(1, 'This is a collaborative visualization test', 1, 1, '2018-03-05 16:24:47'), 
(2, 'Ok, this looks nice', 2, 1, '2018-03-05 16:24:47'), 
(3, 'Of course :)', 1, 1, '2018-03-05 16:24:47'), 
(4, 'Lets do some work', 2, 1, '2018-03-05 16:24:47'), 
(5, 'Ok, please select the piramidal neurons and send me the selection', 1, 1, '2018-03-05 16:24:47'), 
(6, 'Done!', 2, 1, '2018-03-05 16:24:47'), 
(7, 'Thanks', 1, 1, '2018-03-05 16:24:47');
