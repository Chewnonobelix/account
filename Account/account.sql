-- phpMyAdmin SQL Dump
-- version 4.9.1
-- https://www.phpmyadmin.net/
--
-- Hôte : localhost
-- Généré le :  mar. 07 jan. 2020 à 19:14
-- Version du serveur :  5.5.57-MariaDB
-- Version de PHP :  7.3.7

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `account`
--

-- --------------------------------------------------------

--
-- Structure de la table `account`
--

CREATE TABLE `account` (
  `ID` int(11) NOT NULL,
  `account` varchar(50) NOT NULL,
  `value` double NOT NULL,
  `date_eff` date DEFAULT NULL,
  `type` enum('income','outcome','') NOT NULL,
  `profile` varchar(200) NOT NULL DEFAULT 'Default',
  `commonExpanse` int(11) DEFAULT NULL,
  `frequencyReference` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `budget`
--

CREATE TABLE `budget` (
  `id` int(11) NOT NULL,
  `category` int(11) NOT NULL,
  `reference` date NOT NULL,
  `profile` varchar(200) NOT NULL DEFAULT 'Default',
  `account` varchar(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `categories`
--

CREATE TABLE `categories` (
  `ID` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `type` enum('income','outcome','','') DEFAULT NULL,
  `account` varchar(200) NOT NULL,
  `profile` varchar(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `commonEntry`
--

CREATE TABLE `commonEntry` (
  `idCommon` int(11) NOT NULL,
  `name` varchar(200) NOT NULL,
  `entry` int(11) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `commonExpanse`
--

CREATE TABLE `commonExpanse` (
  `id` int(11) NOT NULL,
  `begin` date NOT NULL,
  `isClose` tinyint(1) NOT NULL DEFAULT '0',
  `title` varchar(200) NOT NULL,
  `profile` varchar(200) NOT NULL DEFAULT 'Default',
  `account` varchar(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `entrymetada`
--

CREATE TABLE `entrymetada` (
  `id` int(11) NOT NULL,
  `entry` int(11) NOT NULL,
  `name` varchar(200) NOT NULL,
  `value` varchar(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `frequency`
--

CREATE TABLE `frequency` (
  `id` int(11) NOT NULL,
  `freq` int(11) NOT NULL,
  `nbGroup` int(11) NOT NULL,
  `end` date DEFAULT NULL,
  `profile` varchar(200) NOT NULL DEFAULT 'Default',
  `account` varchar(200) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `information`
--

CREATE TABLE `information` (
  `id` int(11) NOT NULL,
  `id_entry` int(11) NOT NULL,
  `info` varchar(200) NOT NULL,
  `prev` tinyint(1) NOT NULL DEFAULT '0',
  `category` int(11) DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `subbudget`
--

CREATE TABLE `subbudget` (
  `ID` int(11) NOT NULL,
  `idBudget` int(11) NOT NULL,
  `frequency` int(11) NOT NULL,
  `target` double NOT NULL,
  `fromDate` date NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Index pour les tables déchargées
--

--
-- Index pour la table `account`
--
ALTER TABLE `account`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `type` (`type`);

--
-- Index pour la table `budget`
--
ALTER TABLE `budget`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `name` (`name`,`account`,`profile`);

--
-- Index pour la table `commonEntry`
--
ALTER TABLE `commonEntry`
  ADD UNIQUE KEY `idCommon` (`idCommon`,`name`,`entry`);

--
-- Index pour la table `commonExpanse`
--
ALTER TABLE `commonExpanse`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `entrymetada`
--
ALTER TABLE `entrymetada`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `entry` (`entry`,`name`);

--
-- Index pour la table `frequency`
--
ALTER TABLE `frequency`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `information`
--
ALTER TABLE `information`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `subbudget`
--
ALTER TABLE `subbudget`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `idBudget` (`idBudget`,`fromDate`);

--
-- AUTO_INCREMENT pour les tables déchargées
--

--
-- AUTO_INCREMENT pour la table `account`
--
ALTER TABLE `account`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `budget`
--
ALTER TABLE `budget`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `categories`
--
ALTER TABLE `categories`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `commonExpanse`
--
ALTER TABLE `commonExpanse`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `entrymetada`
--
ALTER TABLE `entrymetada`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `frequency`
--
ALTER TABLE `frequency`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `information`
--
ALTER TABLE `information`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT pour la table `subbudget`
--
ALTER TABLE `subbudget`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
