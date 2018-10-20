SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;


CREATE TABLE IF NOT EXISTS `iis_kancelar` (
  `roomID` varchar(7) NOT NULL,
  `el_zasuvky` int(2) NOT NULL,
  `eth_zasuvky` int(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `iis_kancelar` (`roomID`, `el_zasuvky`, `eth_zasuvky`) VALUES
('B218', 12, 6),
('C306', 10, 5),
('F111', 16, 8),
('O112', 8, 4);

CREATE TABLE IF NOT EXISTS `iis_mistnost` (
  `roomID` varchar(7) NOT NULL,
  `oddeleni` int(4) NOT NULL,
  `lokace` varchar(30) DEFAULT NULL,
  `sirka` int(2) DEFAULT NULL,
  `delka` int(2) DEFAULT NULL,
  `vyska` int(2) DEFAULT NULL,
  `smazan` tinyint(4) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `iis_mistnost` (`roomID`, `oddeleni`, `lokace`, `sirka`, `delka`, `vyska`, `smazan`) VALUES
('B218', 3, 'Budova 1 p.2', 6, 5, 3, 0),
('C306', 1, 'Budova 2 p.2', 25, 25, 10, 0),
('D0207', 5, '', 12, 32, 12, 1),
('D105', 5, 'Budova 2 p.1', 5, 4, 3, 0),
('E105', 3, 'Budova 2 p.3', 3, 4, 3, 0),
('E112', 7, 'Budova 1 p.4', 8, 5, 3, 0),
('F111', 2, 'Budova 1 p.3', 6, 5, 3, 0),
('O112', 4, 'Budova 1 p.1', 8, 5, 3, 0);

CREATE TABLE IF NOT EXISTS `iis_oddeleni` (
  `oddID` int(4) NOT NULL,
  `nazev` varchar(50) DEFAULT NULL,
  `popis` varchar(255) DEFAULT NULL,
  `smazan` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

INSERT INTO `iis_oddeleni` (`oddID`, `nazev`, `popis`, `smazan`) VALUES
(1, 'Informační systémy', 'Formální modely, databáze, management SW', 0),
(2, 'Počítačové systémy', 'Diagnostika, architerktury, akcelerování', 0),
(3, 'Inteligentní systémy', 'Biometrické systémy, robotika, Petriho sítě', 0),
(4, 'Grafika a multimédia', 'Grafika, zvuk, obraz, medicína', 0),
(5, 'Děkanát', 'Vedení fakulty', 0),
(6, 'CVT', 'Centrum vypočetní techniky', 0),
(7, 'Vézkumné centrum IT', 'IT4Innovations', 0);

CREATE TABLE IF NOT EXISTS `iis_porucha` (
  `poruchaID` int(5) NOT NULL,
  `k_oprave` int(5) NOT NULL,
  `technik` int(5) DEFAULT NULL,
  `oznamujici` int(5) NOT NULL,
  `popis` varchar(255) DEFAULT NULL,
  `cas_oznam` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `zprava_opravy` varchar(255) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

INSERT INTO `iis_porucha` (`poruchaID`, `k_oprave`, `technik`, `oznamujici`, `popis`, `cas_oznam`, `zprava_opravy`) VALUES
(1, 1, NULL, 4, 'Vadny konektor napajeni', '2015-12-05 10:38:29', NULL),
(2, 3, NULL, 2, 'Spatna baterie', '2015-12-05 10:38:29', NULL),
(3, 3, NULL, 2, 'Nereaguje touchpad', '2015-12-05 10:38:29', NULL),
(4, 1, NULL, 4, 'Nedekuje pam. kartu', '2015-12-05 10:38:29', NULL),
(5, 5, 1, 4, 'Vadne pixely', '2015-12-05 10:38:29', NULL),
(6, 1, 1, 1, 'hjkhkjhk', '2015-12-05 11:05:23', NULL);

CREATE TABLE IF NOT EXISTS `iis_prava` (
  `pravaID` int(2) NOT NULL,
  `popis` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `iis_prava` (`pravaID`, `popis`) VALUES
(1, 'Zaměstnanec'),
(2, 'Materiálový koordinátor'),
(3, 'Personalista'),
(4, 'Technik'),
(5, 'Admin');

CREATE TABLE IF NOT EXISTS `iis_presun` (
  `presunID` int(5) NOT NULL,
  `zarizeni` int(5) NOT NULL,
  `zadatel` int(5) NOT NULL,
  `duvod` varchar(255) DEFAULT NULL,
  `provedeno` timestamp NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP,
  `cas_zadosti` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `odkud` varchar(7) NOT NULL,
  `kam` varchar(7) NOT NULL,
  `schvalil` int(7) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

INSERT INTO `iis_presun` (`presunID`, `zarizeni`, `zadatel`, `duvod`, `provedeno`, `cas_zadosti`, `odkud`, `kam`, `schvalil`) VALUES
(1, 1, 4, 'Nepoužívám', '2015-12-07 15:07:20', '2015-12-05 10:38:29', 'C306', 'E112', 1),
(2, 2, 4, 'Zbytecne velka', NULL, '2015-12-05 10:38:29', 'E112', 'C306', NULL),
(4, 3, 4, 'Nedostatečný výkon pro vyvíjené aplikace', NULL, '2015-12-06 23:01:22', 'B218', 'F111', NULL),
(6, 1, 1, 'Malý rozměr a nedostatečné rozlišení pro práci s grafikou', NULL, '2015-12-07 07:31:03', 'E112', 'E105', NULL);

CREATE TABLE IF NOT EXISTS `iis_ucebna` (
  `roomID` varchar(7) NOT NULL,
  `mist_k_sezeni` int(3) NOT NULL,
  `pocet_rad` int(2) DEFAULT NULL,
  `mist_na_radu` int(3) DEFAULT NULL,
  `bloky` int(2) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `iis_ucebna` (`roomID`, `mist_k_sezeni`, `pocet_rad`, `mist_na_radu`, `bloky`) VALUES
('D0207', 20, 0, 0, 0),
('D105', 152, 8, 19, 3),
('E105', 20, 4, 5, 1),
('E112', 20, 4, 5, 1);

CREATE TABLE IF NOT EXISTS `iis_zamestnanec` (
  `zamID` int(5) NOT NULL,
  `pracoviste` varchar(7) NOT NULL,
  `prava` int(2) NOT NULL,
  `login` varchar(10) DEFAULT NULL,
  `jmeno` varchar(20) NOT NULL,
  `prijmeni` varchar(20) NOT NULL,
  `password` varchar(60) DEFAULT NULL,
  `datum_narozeni` date DEFAULT NULL,
  `mobil` int(9) DEFAULT NULL,
  `telefon_kancelar` int(9) DEFAULT NULL,
  `mail` varchar(255) DEFAULT NULL,
  `aktivni` tinyint(1) NOT NULL DEFAULT '1',
  `smazan` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

INSERT INTO `iis_zamestnanec` (`zamID`, `pracoviste`, `prava`, `login`, `jmeno`, `prijmeni`, `password`, `datum_narozeni`, `mobil`, `telefon_kancelar`, `mail`, `aktivni`, `smazan`) VALUES
(1, 'C306', 5, 'xadmin00', 'Neznamý', 'NERD', '$2a$10$AYw5dbDErKZhh0pimErzvOFOfqIYedY8ULg.303ouzyPxzcy1p6bq', NULL, NULL, NULL, NULL, 1, 0),
(2, 'B218', 3, 'xnovak90', 'Jan', 'Novák', '$2a$10$VS0PVnYV3c30hu/X86emmOf35LpddDrO0VG6u4sFYW8l5eYiBhDf2', NULL, NULL, NULL, NULL, 1, 0),
(3, 'C306', 4, 'xvecer18', 'Vojtěch', 'Večeřa', '$2a$10$nusGJ9Bb8ZcqwLZ8./Dfw.6YeRy7UCMoOEJRdk5F7VgSw.69quEq2', NULL, NULL, NULL, NULL, 1, 0),
(4, 'O112', 2, 'xpalka26', 'David', 'Pálka', '$2a$10$iBBK3pSBASRWHSQIFCh2Z.Ud2H7v9Bz4tb2EiLHaJjG3AJWkMUPaC', NULL, NULL, NULL, NULL, 1, 0),
(5, 'F111', 1, 'xvesel19', 'Marek', 'Veselý', '$2a$10$ZkEQQoined9DCTkQD6HieurHiAE0CicdLUgHeZq3JsxaT8O9F/hcO', NULL, NULL, NULL, NULL, 1, 0),
(6, 'C306', 5, 'sokac', 'Tomas', 'Sokac', '$2y$10$E5XIbh3ZLl7x4opMx7holu7yIsmnh/2D.AEaBbMwqdfsubYiPgzRy', '0000-00-00', 0, 0, '', 0, 1),
(7, 'F111', 4, 'xtestt00', 'Test', 'Test', '$2y$10$5mXm25MX.pJf//luVwXQbOMSY5ljO.J2A0OMyvmpMa65XDAQ7hdUC', '0000-00-00', 0, 0, '', 0, 1),
(8, 'F111', 4, 'xtechn02', 'Technik', 'Technicky', '$2y$10$pQ0nnRuPxDzhz75UKWq.muE2teRGsfM2bGn.aW23hjMiblhpitHpW', '0000-00-00', 0, 0, '', 1, 0);

CREATE TABLE IF NOT EXISTS `iis_zarizeni` (
  `zarID` int(5) NOT NULL,
  `umisteno` varchar(7) NOT NULL,
  `zodpovida` int(5) NOT NULL,
  `znacka` varchar(30) DEFAULT NULL,
  `model` varchar(30) DEFAULT NULL,
  `cena` int(10) DEFAULT NULL,
  `specifikace` varchar(255) DEFAULT NULL,
  `stav` varchar(100) DEFAULT NULL,
  `smazan` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;

INSERT INTO `iis_zarizeni` (`zarID`, `umisteno`, `zodpovida`, `znacka`, `model`, `cena`, `specifikace`, `stav`, `smazan`) VALUES
(1, 'E112', 4, 'Acer', 'abc-567', 5999, 'tablet', 'Vadný konektor', 0),
(2, 'C306', 2, 'HP', 'LaserJet1235', 3000, 'tiskarna', 'Ok', 0),
(3, 'B218', 2, 'Lenovo', 'Thinkpad T50', 20000, 'notebook', 'Å patnÃ¡ baterie', 0),
(4, 'F111', 2, 'LG', 'SuperModel-Mk2', 8000, 'mobil', 'Ok', 0),
(5, 'F111', 4, 'Samsung', 'SmartTV2456', 14000, 'monitor', 'Ok', 0),
(6, 'E105', 2, 'Genius', 'Silver-5894', 5999, 'reproduktory', 'Ok', 0);

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL,
  `login` varchar(10) NOT NULL,
  `password` varchar(60) NOT NULL,
  `prava` int(11) DEFAULT NULL
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

INSERT INTO `users` (`id`, `login`, `password`, `prava`) VALUES
(1, 'xadmin00', '$2a$10$AYw5dbDErKZhh0pimErzvOFOfqIYedY8ULg.303ouzyPxzcy1p6bq', 5),
(2, 'xnovak90', '$2a$10$VS0PVnYV3c30hu/X86emmOf35LpddDrO0VG6u4sFYW8l5eYiBhDf2', 3),
(3, 'xvecer18', '$2a$10$nusGJ9Bb8ZcqwLZ8./Dfw.6YeRy7UCMoOEJRdk5F7VgSw.69quEq2', 4),
(4, 'xpalka26', '$2a$10$iBBK3pSBASRWHSQIFCh2Z.Ud2H7v9Bz4tb2EiLHaJjG3AJWkMUPaC', 2),
(5, 'xvesel19', '$2a$10$ZkEQQoined9DCTkQD6HieurHiAE0CicdLUgHeZq3JsxaT8O9F/hcO', 1),
(8, 'xtechn02', '$2y$10$90I8UVy2D15koeg.FexFS.PNIwlbC2hGRLC/5yzNNNzhj/vaWKfKi', 4);


ALTER TABLE `iis_kancelar`
  ADD PRIMARY KEY (`roomID`);

ALTER TABLE `iis_mistnost`
  ADD PRIMARY KEY (`roomID`),
  ADD KEY `patrila` (`oddeleni`);

ALTER TABLE `iis_oddeleni`
  ADD PRIMARY KEY (`oddID`);

ALTER TABLE `iis_porucha`
  ADD PRIMARY KEY (`poruchaID`),
  ADD KEY `poskozeno` (`k_oprave`),
  ADD KEY `opravil` (`technik`),
  ADD KEY `oznamil` (`oznamujici`);

ALTER TABLE `iis_prava`
  ADD PRIMARY KEY (`pravaID`);

ALTER TABLE `iis_presun`
  ADD PRIMARY KEY (`presunID`),
  ADD KEY `presunuto` (`zarizeni`),
  ADD KEY `zadal` (`zadatel`);

ALTER TABLE `iis_ucebna`
  ADD PRIMARY KEY (`roomID`);

ALTER TABLE `iis_zamestnanec`
  ADD PRIMARY KEY (`zamID`),
  ADD UNIQUE KEY `login` (`login`),
  ADD KEY `mel` (`prava`),
  ADD KEY `sidlil` (`pracoviste`);

ALTER TABLE `iis_zarizeni`
  ADD PRIMARY KEY (`zarID`),
  ADD KEY `vlastnil` (`zodpovida`),
  ADD KEY `obsahovala` (`umisteno`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD KEY `duplicita` (`login`);


ALTER TABLE `iis_oddeleni`
  MODIFY `oddID` int(4) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=9;
ALTER TABLE `iis_porucha`
  MODIFY `poruchaID` int(5) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=7;
ALTER TABLE `iis_presun`
  MODIFY `presunID` int(5) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=8;
ALTER TABLE `iis_zamestnanec`
  MODIFY `zamID` int(5) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=9;
ALTER TABLE `iis_zarizeni`
  MODIFY `zarID` int(5) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=8;
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=9;

ALTER TABLE `iis_kancelar`
  ADD CONSTRAINT `FKKancelar` FOREIGN KEY (`roomID`) REFERENCES `iis_mistnost` (`roomID`);

ALTER TABLE `iis_mistnost`
  ADD CONSTRAINT `patrila` FOREIGN KEY (`oddeleni`) REFERENCES `iis_oddeleni` (`oddID`);

ALTER TABLE `iis_porucha`
  ADD CONSTRAINT `opravil` FOREIGN KEY (`technik`) REFERENCES `iis_zamestnanec` (`zamID`),
  ADD CONSTRAINT `oznamil` FOREIGN KEY (`oznamujici`) REFERENCES `iis_zamestnanec` (`zamID`),
  ADD CONSTRAINT `poskozeno` FOREIGN KEY (`k_oprave`) REFERENCES `iis_zarizeni` (`zarID`);

ALTER TABLE `iis_presun`
  ADD CONSTRAINT `presunuto` FOREIGN KEY (`zarizeni`) REFERENCES `iis_zarizeni` (`zarID`),
  ADD CONSTRAINT `zadal` FOREIGN KEY (`zadatel`) REFERENCES `iis_zamestnanec` (`zamID`);

ALTER TABLE `iis_ucebna`
  ADD CONSTRAINT `FKUcebna` FOREIGN KEY (`roomID`) REFERENCES `iis_mistnost` (`roomID`);

ALTER TABLE `iis_zamestnanec`
  ADD CONSTRAINT `mel` FOREIGN KEY (`prava`) REFERENCES `iis_prava` (`pravaID`),
  ADD CONSTRAINT `sidlil` FOREIGN KEY (`pracoviste`) REFERENCES `iis_mistnost` (`roomID`);

ALTER TABLE `iis_zarizeni`
  ADD CONSTRAINT `obsahovala` FOREIGN KEY (`umisteno`) REFERENCES `iis_mistnost` (`roomID`),
  ADD CONSTRAINT `vlastnil` FOREIGN KEY (`zodpovida`) REFERENCES `iis_zamestnanec` (`zamID`);

ALTER TABLE `users`
  ADD CONSTRAINT `duplicita` FOREIGN KEY (`login`) REFERENCES `iis_zamestnanec` (`login`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
