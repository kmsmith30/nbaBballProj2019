// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 30, 2019 <v2.0>

#include "event.hpp"

// Event Constructor

Event::Event(int en, int et, int p, int at, int wc, int pc, int op, Player p1,
	Player p2, Player p3, Team t) {

	eventNumber = en;
	eventType = et;
	period = p;
	actionType = at;
	wcTime = wc;
	pcTime = pc;
	option = op;

	player1 = p1;
	player2 = p2;
	player3 = p3;

	team = t;
}

// Event Variable Getters

int Event::getEventNumber() {
	return eventNumber;
}

int Event::getPCTime() {
	return pcTime;
}

int Event::getOption() {
	return option;
}

Player Event::getPlayer1() {
	return player1;
}

Player Event::getPlayer2() {
	return player2;
}

Player Event::getPlayer3() {
	return player3;
}

Team Event::getTeam() {
	return team;
}

// Event Type Checks

bool Event::isMadeShot() {
	return eventType == MADE_SHOT;
}

bool Event::isMissedShot() {
	return eventType == MISSED_SHOT;
}

bool Event::isFreeThrow() {
	return eventType == FREE_THROW;
}

bool Event::isRebound() {
	return eventType == REBOUND;
}

bool Event::isTurnover() {
	return eventType == TURNOVER;
}

bool Event::isFoul() {
	return eventType == FOUL;
}

bool Event::isViolation() {
	return eventType == VIOLATION;
}

bool Event::isSubstitution() {
	return eventType == SUBSTITUTION;
}

bool Event::isTimeout() {
	return eventType == TIMEOUT;
}

bool Event::isJumpBall() {
	return eventType == JUMP_BALL;
}

bool Event::isEjection() {
	return eventType == EJECTION;
}

bool Event::isStartPeriod() {
	return eventType == START_PERIOD;
}

bool Event::isEndPeriod() {
	return eventType == END_PERIOD;
}

// Special Event Checks

bool Event::isGameCompleted() {
	return eventNumber == GAME_COMPLETE;
}

bool Event::isInitialJump() {
	return isJumpBall() && eventNumber == INITIAL_JUMP_BALL;
}

bool Event::isShootingFoul() {
	return isFoul() && actionType == SHOOTING_FOUL;
}

bool Event::isTechnicalFoul() {
	return isFoul() && actionType == TECHNICAL_FOUL;
}

bool Event::isFlagrantFoul() {
	return isFoul() && (actionType == FLAGRANT_FOUL_A ||
		actionType == FLAGRANT_FOUL_B);
}

bool Event::isMadeFreeThrow() {
	return isFreeThrow() && option == MADE_FREE_THROW;
}

bool Event::isFinalFreeThrow() {
	return isFreeThrow() &&
		(actionType == FINAL_FREE_THROW_A ||
			actionType == FINAL_FREE_THROW_B ||
			actionType == FINAL_FREE_THROW_C ||
			actionType == TECHNICAL_FREE_THROW ||
			actionType == FLAGRANT_FREE_THROW_A ||
			actionType == FLAGRANT_FREE_THROW_B ||
			actionType == CLEAR_PATH_FREE_THROW);
}

bool Event::isTechnicalFreeThrow() {
	return isFreeThrow() && actionType == TECHNICAL_FREE_THROW;
}

bool Event::isFlagrantFreeThrow() {
	return isFreeThrow() &&
		(actionType == FLAGRANT_FREE_THROW_A ||
			actionType == FLAGRANT_FREE_THROW_B);
}

bool Event::isClearPathFreeThrow() {
	return isFreeThrow() && actionType == CLEAR_PATH_FREE_THROW;
}

bool Event::isNormalFreeThrow() {
	return isFreeThrow() && !isTechnicalFreeThrow() && !isFlagrantFreeThrow()
		&& !isClearPathFreeThrow();
}

bool Event::isShotclockViolation() {
	return isTurnover() && actionType == SHOTCLOCK_VIOLATION;
}

bool Event::isUnknownRebound(Team home, Team away) {
	if (isRebound()) {
		if (home.hasPlayer(player1)) return false;
		if (away.hasPlayer(player1)) return false;

		return true;
	}
	return false;
}

bool Event::isEndPossession(Event lastEvent, Team home, Team away) {

	// Made Shot, Turnover, or End of Period
	if (isMadeShot() || isTurnover() || isEndPeriod()) {
		return true;
	}
	// Made Final Free Throw
	else if (isFinalFreeThrow() && option == MADE_FREE_THROW) {
		return true;
	}
	// Defensive Rebound from Missed Shot
	else if (isRebound()) {

		// Missed Final FT or Shot
		if ((lastEvent.isFinalFreeThrow() && lastEvent.option != MADE_FREE_THROW)
			|| lastEvent.isMissedShot()) {

			// Find Rebounder
			bool homeBoarder = false;

			for (Player p : home.getRoster()) {

				if (p == player1) {
					homeBoarder = true;
					break;
				}
			}

			// Find Shooter
			bool homeShooter = false;

			for (Player p : home.getRoster()) {

				if (p == lastEvent.player1) {
					homeShooter = true;
					break;
				}
			}

			// Can't be on same team, else same possession
			if (homeBoarder != homeShooter) {
				return true;
			}

		}
	}

	// Event does not switch possession
	return false;
}

// Event Operators

/* Evaluate Period, PC Time, WC Time, then Event Number */
bool Event::operator<(Event e) {

	return (period < e.period) ||
		(period == e.period && pcTime > e.pcTime) ||
		(period == e.period && pcTime == e.pcTime && wcTime < e.wcTime) ||
		(period == e.period && pcTime == e.pcTime && wcTime == e.wcTime &&
			eventNumber < e.eventNumber);
}