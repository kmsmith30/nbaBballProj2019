/* Team Header */
// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 31, 2019 <v2.0>

#ifndef TEAM_H_
#define TEAM_H_

#include "player.hpp"

#include <string>
#include <vector>

/* Represents Team with Players */
class Team {

public:

	Team() {}; // Default

	/// Construct Team with ID
	Team(std::string tid);

	/// Team Getters

	std::string getTeamID();

	int getTeamSize();

	int getScore();

	int getOffPossessions();
	int getDefPossessions();

	std::vector<Player> getRoster();
	std::vector<Player> getCourt();
	std::vector<Player> getBench();

	/// Team Functions

	void addPlayer(Player p);

	void addToCourt(Player p);
	void addToBench(Player p);

	void removeFromBench(Player p);

	void clearCourt();

	void score(int points);
	void scoredOn(int points);

	void offPossession();
	void defPossession();

	void playerOffPossesion(Player p);
	void playerDefPossession(Player p);

	void substitute(Player in, Player out);

	bool hasPlayer(Player p);

	bool isOnCourt(Player p);
	bool isOnBench(Player p);

	void updateRoster();

	/// Team Operators

	bool operator==(Team t);

private:

	std::string teamID;			// Unique ID for each Team

	int gameScore;				// Score in Game

	int offPossessions;			// Net Offensive Possessions in Game
	int defPossessions;			// Net Defensive Possessions in Game

	std::vector<Player> roster;	// Vector of all Players 

	std::vector<Player> court;	// Vector of Players in Game
	std::vector<Player> bench;	// Vector of Non-playing Players in Game

};

#endif // TEAM_H_