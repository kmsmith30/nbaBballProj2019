/* Player Header */
// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 31, 2019 <v2.0>

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>

/* Represents Player data and ID */
class Player {

public:

	Player() {}; // Default

	/// Construct Player with ID
	Player(std::string pid);

	/// Player Getters

	std::string getPlayerID();

	int getPointsFor();
	int getPointsAgainst();

	int getOffPossessions();
	int getDefPossessions();

	/// Player Functions

	bool isActive();

	void score(int points);
	void scoredOn(int points);

	void offPossession();
	void defPossession();

	void decOffPossession();
	void decDefPossession();

	void activate();
	void deactivate();

	/// Player Operators

	bool operator==(Player p);

private:

	std::string playerID;	// Unique ID for each Player

	int pointsFor;			// Points scored while on court
	int pointsAgainst;		// Points scored on while on court

	int offPossessions;		// Offensive possessions while on court
	int defPossessions;		// Defensive possessions while on court

	bool active;			// Active in Game

};

#endif // PLAYER_H_
