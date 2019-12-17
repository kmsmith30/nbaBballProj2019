/* Game Header */
// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: June 2, 2019 <v2.0>

#ifndef GAME_H_
#define GAME_H_

#include "event.hpp"
#include "team.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


#define ONE_HUNDRED_POSSESSIONS	100.0

/* Represents Game with two Teams */
class Game {

public:

	Game() {}; // Default

	/// Construct Game with ID and Home and Away Teams
	Game(std::string gid, Team ht, Team at);

	/// Game Getters

	std::string getGameID();

	Team getHomeTeam();
	Team getAwayTeam();

	std::vector<Event> getGameEvents();

	/// Game Functions

	/// Add Event to Events vector
	void addEvent(Event ev);
	/// Add starting Players vector to starters vector
	void addStarters(std::vector<Player> s);

	/// Sort Events by period, PC Time, WC Time, then number
	void sortEvents();

	/// Add Players to roster with calculated data
	void updateRosters();

	void homeScore(int points);
	void awayScore(int points);

	void endOfHomePossession();
	void endOfAwayPossession();

	/// Substitute and increment Player possessions
	void subPossession(Player player);

	void homeSubstitution(Player in, Player out);
	void awaySubstitution(Player in, Player out);

	/// Add Players to sub buffer
	void addToSubBuffer(Player in, Player out);
	/// Add buffered Players to Game
	void pushSubBuffer();

	/// Place period starting Players into Game
	void updateStarters();

	/// Simulate Events in Game
	void simulateGame();

	/// Print Player Data for Off and Def Rtg
	void printRatings();

	/* Game Event Handlers */

	// Increase points and end possession
	void handleMadeShot(Event ev);
	
	// Free Throw Handlers

	// Increment points if made FT
	void handleFreeThrow(Event ev);
	// Increment points and end possession if made
	void handleFinalFreeThrow(Event ev);

	// End possession
	void handleTurnover(Event ev);

	// End possession if Defensive
	void handleRebound(Event ev, Event lastEv);

	// End possession and update starting Players for next period
	void handleEndPeriod(Event ev, Event lastEv, int i);

	// Determine if unknown rebound is Offensive or Defensive
	void handleUnknownRebound(Event ev, Event lastEv, Event nextEv, int i);

	// Substitute court/bench players, use sub buffer if FT is occuring
	bool handleSubstitution(Event ev, Event lastEv, Event nextEv, int i, 
		bool wait);

	// Increase points, wait for FT to end possession
	void handleAndOne(Event ev);

	// Determine which handler to evalute possession
	void handleEndPossession(Event ev, Event lastEv, Event nextEv, int i);

private:

	std::string gameID;	// Unique ID for each Game

	Team homeTeam;		// Home Team (first team in Game File)
	Team awayTeam;		// Away Team

	std::vector<Event> events;	// Vector of all Game Events

	// Vector of Vectors with starting Players for each period
	std::vector<std::vector<Player>> starters;

	int period;	// Current period of Game

	std::vector<Player> subBufferOut;	// Holds subs to leave Game after FTs
	std::vector<Player> subBufferIn;	// Holds subs to enter Game after FTs

	Event lastPossession;		// Last Event that ended possession
	Team lastPossessionTeam;	// Last Team to have an offensive possession
};

#endif // GAME_H_
