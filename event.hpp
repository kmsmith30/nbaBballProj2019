/* Event Header */
// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 30, 2019 <v2.0>

#ifndef EVENT_H_
#define EVENT_H_

#include "player.hpp"
#include "team.hpp"

#include <cmath>


// Event Time Values
#define TENTHS	10
#define SECONDS	60

// Special Event Numbers
#define	GAME_COMPLETE		0
#define INITIAL_JUMP_BALL	4

// Event Types
#define MADE_SHOT		1
#define MISSED_SHOT		2
#define FREE_THROW		3
#define REBOUND			4
#define TURNOVER		5
#define FOUL			6
#define VIOLATION		7
#define SUBSTITUTION	8
#define TIMEOUT			9
#define JUMP_BALL		10
#define EJECTION		11
#define START_PERIOD	12
#define END_PERIOD		13

// Free Throw Values
#define FINAL_FREE_THROW_A			10
#define FINAL_FREE_THROW_B			12
#define FINAL_FREE_THROW_C			15
#define TECHNICAL_FREE_THROW		16 
#define FLAGRANT_FREE_THROW_A		19
#define FLAGRANT_FREE_THROW_B		20
#define CLEAR_PATH_FREE_THROW		26

// Point Values
#define MADE_FREE_THROW		1
#define TWO_POINTS			2
#define THREE_POINTS		3

// Foul Values
#define SHOOTING_FOUL		2
#define TECHNICAL_FOUL		11
#define FLAGRANT_FOUL_A		14	
#define FLAGRANT_FOUL_B		15

// Turnover Values
#define SHOTCLOCK_VIOLATION		11

/* Represents Event that occurs in Game */
class Event {

public:

	Event() {}; // Default Event

	/// Construct Event with Event number, type, period, action type, WC Time,
	/// PC Time, option, Players 1-3 (From Play line data), and Team
	Event(int en, int et, int p, int at, int wc, int pc, int op, Player p1,
		Player p2, Player p3, Team t);

	/// Event Getters

	int getEventNumber();
	int getPCTime();
	int getOption();

	Player getPlayer1();
	Player getPlayer2();
	Player getPlayer3();

	Team getTeam();

	/// Event Checks

	/// Main Event Checks

	bool isMadeShot();
	bool isMissedShot();
	bool isFreeThrow();
	bool isRebound();
	bool isTurnover();
	bool isFoul();
	bool isViolation();
	bool isSubstitution();
	bool isTimeout();
	bool isJumpBall();
	bool isEjection();
	bool isStartPeriod();
	bool isEndPeriod();

	/// Special Event Checks

	bool isGameCompleted();

	bool isInitialJump();

	bool isShootingFoul();
	bool isFlagrantFoul();
	bool isTechnicalFoul();

	bool isMadeFreeThrow();
	bool isFinalFreeThrow();
	bool isTechnicalFreeThrow();
	bool isFlagrantFreeThrow();
	bool isClearPathFreeThrow();
	bool isNormalFreeThrow();

	bool isShotclockViolation();

	bool isUnknownRebound(Team home, Team away);

	bool isEndPossession(Event lastEvent, Team home, Team away);

	/// Event Operators

	bool operator<(Event ge);

private:

	int eventNumber;	// Number given to Event (mostly in order)
	int eventType;		// Event Type Number [1-13]
	int period;			// Period Event occured [1-5]
	int actionType;		// Action Type for Event
	int wcTime;			// Actual Time (in tenth sec)
	int pcTime;			// Game Clock Time (in tenth sec)
	int option;			// Point Values for FGs, FTs

	Player player1; // For subs, player1 is coming out
	Player player2; // For subs, player2 is coming in
	Player player3; // This person gains possession of jump

	Team team;		// Team associated w/ Event (not always accurate)

};

#endif // EVENT_H_