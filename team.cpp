// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 31, 2019 <v2.0>

#include "team.hpp"

// Team Constructor

Team::Team(std::string tid) {
	teamID = tid;
	gameScore = 0;

	offPossessions = 0;
	defPossessions = 0;
}

// Get Team Variables

std::string Team::getTeamID() {
	return teamID;
}

std::vector<Player> Team::getRoster() {
	return roster;
}

std::vector<Player> Team::getCourt() {
	return court;
}

std::vector<Player> Team::getBench() {
	return bench;
}

// Team Functions

int Team::getTeamSize() {
	return roster.size();
}

int Team::getScore() {
	return gameScore;
}

int Team::getOffPossessions() {
	return offPossessions;
}

int Team::getDefPossessions() {
	return defPossessions;
}

// Add Player p to Team roster
void Team::addPlayer(Player p) {
	roster.push_back(p);
}

void Team::addToCourt(Player p) {

	for (Player player : roster) {
		if (player == p) {
			court.push_back(p);
		}
	}
}

void Team::addToBench(Player p) {
	bench.push_back(p);
}

void Team::removeFromBench(Player p) {

	std::vector<Player> newBench;

	for (Player player : bench) {
		if (!(player == p)) {
			newBench.push_back(player);;
		}
	}

	bench = newBench;
}


void Team::clearCourt() {
	court.clear();
}

void Team::score(int points) {
	std::vector<Player> scoredCourt;

	gameScore += points;

	for (Player player : court) {
		player.score(points);
		scoredCourt.push_back(player);
	}

	court = scoredCourt;
}

void Team::scoredOn(int points) {
	std::vector<Player> scoredCourt;

	for (Player player : court) {
		player.scoredOn(points);
		scoredCourt.push_back(player);
	}

	court = scoredCourt;
}

void Team::offPossession() {
	std::vector<Player> posCourt;

	for (Player player : court) {
		player.offPossession();
		posCourt.push_back(player);
	}

	court = posCourt;

	offPossessions++;
}

void Team::defPossession() {
	std::vector<Player> posCourt;

	for (Player player : court) {
		player.defPossession();
		posCourt.push_back(player);
	}

	court = posCourt;

	defPossessions++;
}

void Team::playerOffPossesion(Player p) {

	std::vector<Player> posCourt;

	for (Player player : court) {
		if (p == player) {
			player.offPossession();
			posCourt.push_back(player);
		}
		else posCourt.push_back(player);
	}

	court = posCourt;
}

void Team::playerDefPossession(Player p) {

	std::vector<Player> posCourt;

	for (Player player : court) {
		if (p == player) {
			player.defPossession();
			posCourt.push_back(player);
		}
		else posCourt.push_back(player);
	}

	court = posCourt;
}

void Team::substitute(Player out, Player in) {

	std::vector<Player> newCourt;
	std::vector<Player> newBench;

	Player pOut;
	Player pIn;


	// Find player to sub out
	for (Player player : court) {
		if (!(player == out)) {
			newCourt.push_back(player);
		}
		else {
			pOut = player;
		}
	}

	// Find player to sub in
	for (Player player : bench) {
		if (!(player == in)) {
			newBench.push_back(player);
		}
		else {
			pIn = player;
		}
	}

	newBench.push_back(pOut);
	newCourt.push_back(pIn);

	bench = newBench;
	court = newCourt;
}

// Team Checks

bool Team::hasPlayer(Player p) {
	for (Player player : roster) {
		if (player == p) {
			return true;
		}
	}

	return false;
}

bool Team::isOnCourt(Player p) {
	for (Player player : court) {
		if (player == p) {
			return true;
		}
	}

	return false;
}

bool Team::isOnBench(Player p) {
	for (Player player : bench) {
		if (player == p) {
			return true;
		}
	}

	return false;
}

void Team::updateRoster() {
	std::vector<Player> newRoster;
	std::vector<Player> didNotPlay;

	for (Player player : court) {
		newRoster.push_back(player);
	}

	for (Player player : bench) {

		if (player.getOffPossessions() == 0
			&& player.getDefPossessions() == 0) {
			didNotPlay.push_back(player);
			continue;
		}

		newRoster.push_back(player);
	}

	for (Player player : didNotPlay) {
		newRoster.push_back(player);
	}

	roster = newRoster;
}

// Team Operators

bool Team::operator==(Team t) {
	return teamID == t.teamID;
}