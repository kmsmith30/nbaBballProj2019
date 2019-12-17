// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 31, 2019 <v2.0>

#include "player.hpp"

// Player Constructor

Player::Player(std::string pid) {
	playerID = pid;

	pointsFor = 0;
	pointsAgainst = 0;

	offPossessions = 0;
	defPossessions = 0;

	active = false;
}

// Get Player Variables

std::string Player::getPlayerID() {
	return playerID;
}

int Player::getPointsFor() {
	return pointsFor;
}

int Player::getPointsAgainst() {
	return pointsAgainst;
}

int Player::getOffPossessions() {
	return offPossessions;
}

int Player::getDefPossessions() {
	return defPossessions;
}

bool Player::isActive() {
	return active;
}

// Player Functions

void Player::score(int points) {
	pointsFor += points;
}

void Player::scoredOn(int points) {
	pointsAgainst += points;
}

void Player::offPossession() {
	offPossessions++;
}

void Player::defPossession() {
	defPossessions++;
}

void Player::decOffPossession() {
	offPossessions--;
}

void Player::decDefPossession() {
	defPossessions--;
}

void Player::activate() {
	active = true;
}

void Player::deactivate() {
	active = false;
}

// Player Operators

bool Player::operator==(Player p) {
	return playerID == p.playerID;
}