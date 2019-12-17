// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: June 2, 2019 <v2.0>

#include "game.hpp"


// Game Constructor

Game::Game(std::string gid, Team ht, Team at) {
	gameID = gid;

	homeTeam = ht;
	awayTeam = at;

	period = 1;

	lastPossession = Event();
	lastPossessionTeam = Team();
}

// Get Game Variables

std::string Game::getGameID() {
	return gameID;
}

Team Game::getHomeTeam() {
	return homeTeam;
}

Team Game::getAwayTeam() {
	return awayTeam;
}

std::vector<Event> Game::getGameEvents() {
	return events;
}

//	Game Functions

void Game::addEvent(Event ev) {
	events.push_back(ev);
}

void Game::addStarters(std::vector<Player> s) {
	starters.push_back(s);
}

void Game::sortEvents() {
	std::sort(events.begin(), events.end());
}

void Game::updateRosters() {
	homeTeam.updateRoster();
	awayTeam.updateRoster();
}

void Game::homeScore(int points) {
	homeTeam.score(points);
	awayTeam.scoredOn(points);
}

void Game::awayScore(int points) {
	awayTeam.score(points);
	homeTeam.scoredOn(points);
}

void Game::endOfHomePossession() {
	homeTeam.offPossession();
	awayTeam.defPossession();

	lastPossessionTeam = homeTeam;
}

void Game::endOfAwayPossession() {
	homeTeam.defPossession();
	awayTeam.offPossession();

	lastPossessionTeam = awayTeam;
}

void Game::subPossession(Player player) {

	if (homeTeam.hasPlayer(player)) {
		if (lastPossessionTeam == homeTeam) {
			homeTeam.playerDefPossession(player);
		}
		else if (lastPossessionTeam == awayTeam) {
			homeTeam.playerOffPossesion(player);
		}
	}
	else if (awayTeam.hasPlayer(player)) {
		if (lastPossessionTeam == awayTeam) {
			awayTeam.playerDefPossession(player);
		}
		else if (lastPossessionTeam == homeTeam) {
			awayTeam.playerOffPossesion(player);
		}
	}
}

void Game::homeSubstitution(Player out, Player in) {
	homeTeam.substitute(out, in);
}

void Game::awaySubstitution(Player out, Player in) {
	awayTeam.substitute(out, in);
}

void Game::addToSubBuffer(Player out, Player in) {
	subBufferOut.push_back(out);
	subBufferIn.push_back(in);
}

void Game::pushSubBuffer() {
	for (int i = 0; i < subBufferOut.size(); i++) {
		if (homeTeam.hasPlayer(subBufferOut[i])) {
			homeSubstitution(subBufferOut[i], subBufferIn[i]);
		}
		else if (awayTeam.hasPlayer(subBufferOut[i])) {
			awaySubstitution(subBufferOut[i], subBufferIn[i]);
		}
	}

	subBufferIn.clear();
	subBufferOut.clear();
}

void Game::updateStarters() {

	if (period >= starters.size()) {
		return;
	}

	for (Player player : homeTeam.getCourt()) {
		homeTeam.addToBench(player);
	}
	homeTeam.clearCourt();

	for (Player player : awayTeam.getCourt()) {
		awayTeam.addToBench(player);
	}
	awayTeam.clearCourt();

	for (Player player : starters[period]) {
		if (homeTeam.hasPlayer(player)) {
			for (Player benchPlayer : homeTeam.getBench()) {
				if (player == benchPlayer) {
					homeTeam.addToCourt(benchPlayer);
					homeTeam.removeFromBench(benchPlayer);
					break;
				}
			}
		}
		else {
			for (Player benchPlayer : awayTeam.getBench()) {
				if (player == benchPlayer) {
					awayTeam.addToCourt(benchPlayer);
					awayTeam.removeFromBench(benchPlayer);
					break;
				}
			}
		}
	}
}

void Game::handleMadeShot(Event ev) {

	if (ev.isMadeShot()) {
		if (homeTeam.hasPlayer(ev.getPlayer1())) {
			homeScore(ev.getOption());

			endOfHomePossession();

			lastPossession = ev;
		}
		else if (awayTeam.hasPlayer(ev.getPlayer1())) {
			awayScore(ev.getOption());

			endOfAwayPossession();

			lastPossession = ev;
		}
	}
}

void Game::handleFreeThrow(Event ev) {


	if (ev.isMadeFreeThrow()) {

		if (homeTeam.hasPlayer(ev.getPlayer1())) {
			homeScore(MADE_FREE_THROW);
		}
		else if (awayTeam.hasPlayer(ev.getPlayer1())) {
			awayScore(MADE_FREE_THROW);
		}
	}
}

void Game::handleFinalFreeThrow(Event ev) {

	if (ev.isMadeFreeThrow()) {

		if (homeTeam.hasPlayer(ev.getPlayer1())) {

			homeScore(MADE_FREE_THROW);

			// Not a technical, flagrant, or clear path
			if (ev.isNormalFreeThrow()) {
				endOfHomePossession();
				lastPossession = ev;
			}
		}
		else if (awayTeam.hasPlayer(ev.getPlayer1())) {

			awayScore(MADE_FREE_THROW);

			// Not a technical, flagrant, or clear path
			if (ev.isNormalFreeThrow()) {
				endOfAwayPossession();
				lastPossession = ev;
			}
		}
	}
	// Misses handled by rebound handler
}

void Game::handleRebound(Event ev, Event lastEv) {

	if (homeTeam.hasPlayer(lastEv.getPlayer1())) {

		endOfHomePossession();

		lastPossession = ev;
	}
	else if (awayTeam.hasPlayer(lastEv.getPlayer1())) {

		endOfAwayPossession();

		lastPossession = ev;
	}
}

void Game::handleTurnover(Event ev) {

	if (ev.isTurnover()) {
		if (homeTeam.hasPlayer(ev.getPlayer1())) {

			endOfHomePossession();

			lastPossession = ev;
		}
		else if (awayTeam.hasPlayer(ev.getPlayer1())) {

			endOfAwayPossession();

			lastPossession = ev;
		}
		else {
			if (ev.getTeam().getTeamID() == homeTeam.getTeamID()) {
				endOfHomePossession();

				lastPossession = ev;
			}
			else if (ev.getTeam().getTeamID() == awayTeam.getTeamID()) {
				endOfAwayPossession();

				lastPossession = ev;
			}
		}
	}
}

void Game::handleEndPeriod(Event ev, Event lastEv, int count) {

	if (lastEv.isMadeShot() || lastEv.isMadeFreeThrow()) { // Buzzer Beater 

		updateStarters();
		period++;
		return;
	}

	// Iteratre thru events to determine last possession
	while (count > 0) {

		Event currEv = events[count];

		if (homeTeam.hasPlayer(currEv.getPlayer1())) {
			endOfHomePossession();

			lastPossession = ev;

			updateStarters();
			period++;
			break;
		}
		else if (awayTeam.hasPlayer(events[count].getPlayer1())) {
			endOfAwayPossession();

			lastPossession = ev;

			updateStarters();
			period++;
			break;
		}
		count--;
	}
}

/* Helper to determine if event validates possession
   after an unknown rebound */
bool unknownReboundCheck(Event ev) {
	return !ev.isSubstitution() && !ev.isFoul() && !ev.isViolation();
}

void Game::handleUnknownRebound(Event ev, Event lastEv, Event nextEv,
	int i) {

	if (nextEv.isShotclockViolation()) return;

	bool homeShooter = homeTeam.hasPlayer(lastEv.getPlayer1());

	// Iteratre thru events to determine possession
	while (i < events.size()) {

		Event currEv = events[i];
		Player player1 = currEv.getPlayer1();

		if (currEv.isEndPeriod()) { // Wait to find poss after end of period
			return;
		}
		// Home rebounder
		if (homeTeam.hasPlayer(player1) && unknownReboundCheck(currEv)) {
			// Check if defensive rebound
			if (!homeShooter) {

				endOfAwayPossession();

				lastPossession = ev;
			}
			return; // Offensive rebound, do nothing
		}
		// Away rebounder
		else if (awayTeam.hasPlayer(player1) && unknownReboundCheck(currEv)) {
			// Check if defensive rebound
			if (homeShooter) {

				endOfHomePossession();

				lastPossession = ev;
			}
			return; // Offensive rebound, do nothing
		}
		i++;
	}
}

bool Game::handleSubstitution(Event ev, Event lastEv, Event nextEv, int i,
	bool wait) {

	// Check for a "block" of substitutions
	if (nextEv.isSubstitution()) {

		int checkI = i + 1;

		while (checkI < events.size()) {
			checkI++;

			if (events[checkI].isSubstitution()) continue;
			else if (events[checkI].isFreeThrow()) {

				addToSubBuffer(ev.getPlayer1(), ev.getPlayer2());

				return true;
			}
			else break;
		}
	}

	// Instances for Sub Buffer
	if (wait || (lastEv.isFreeThrow() && !lastEv.isFinalFreeThrow())
		|| nextEv.isFreeThrow()) {

		addToSubBuffer(ev.getPlayer1(), ev.getPlayer2());

		return true; // Did use Sub Buffer
	}
	// Regular Substitution
	else {

		if (ev.getPCTime() != lastPossession.getPCTime()) {
			subPossession(ev.getPlayer1());
		}

		if (homeTeam.hasPlayer(ev.getPlayer1())) {

			homeSubstitution(ev.getPlayer1(), ev.getPlayer2());

		}
		else if (awayTeam.hasPlayer(ev.getPlayer1())) {

			awaySubstitution(ev.getPlayer1(), ev.getPlayer2());
		}

		return false; // Did NOT use Sub Buffer
	}
}

void Game::handleAndOne(Event ev) {

	if (homeTeam.hasPlayer(ev.getPlayer1())) {
		homeScore(ev.getOption());
	}
	else if (awayTeam.hasPlayer(ev.getPlayer1())) {
		awayScore(ev.getOption());
	}
}

/* Heleper to determine if a shooting foul occured when shot was made */
bool andOneCheck(Event ev, Event nextEv) {
	return ev.isMadeShot() && nextEv.isShootingFoul()
		&& ev.getPCTime() == nextEv.getPCTime();
}

void Game::handleEndPossession(Event ev, Event lastEv, Event nextEv, int i) {
	if (andOneCheck(ev, nextEv)) {
		handleAndOne(ev);
	}
	else if (ev.isMadeShot()) {
		handleMadeShot(ev);
	}
	else if (ev.isFinalFreeThrow()) {
		handleFinalFreeThrow(ev);
	}
	else if (ev.isRebound()) {
		handleRebound(ev, lastEv);
	}
	else if (ev.isTurnover()) {
		handleTurnover(ev);
	}
	else if (ev.isEndPeriod()) {
		handleEndPeriod(ev, lastEv, i - 1);
	}
}

void Game::simulateGame() {

	bool waitToSub = false, waitForRebound = false;

	Event currEvent, lastEvent, nextEvent;

	for (int i = 0; i < events.size(); i++) {

		currEvent = events[i];

		if (i > 0) lastEvent = events[i - 1];
		else lastEvent = Event();

		if (i + 1 < events.size()) nextEvent = events[i + 1];
		else nextEvent = Event();

		if (currEvent.isUnknownRebound(homeTeam, awayTeam) &&
			lastEvent.isMissedShot()) {

			handleUnknownRebound(currEvent, lastEvent, nextEvent, i + 1);
			continue;
		}

		if (currEvent.isEndPossession(lastEvent, homeTeam, awayTeam)) {
			handleEndPossession(currEvent, lastEvent, nextEvent, i);
		}

		if (currEvent.isFreeThrow() && !currEvent.isFinalFreeThrow()) {
			handleFreeThrow(currEvent);
		}

		if (waitForRebound && currEvent.isRebound()) {
			waitToSub = false;
			waitForRebound = false;

			pushSubBuffer();
		}

		if (waitToSub && currEvent.isFinalFreeThrow()) {
			// Add Player to court after made FT
			if (currEvent.isMadeFreeThrow()) {
				waitToSub = false;

				pushSubBuffer();
			}
			else waitForRebound = true;
		}

		if (currEvent.isSubstitution()) {
			// Sub Handle returns if sub buffer is used
			waitToSub = handleSubstitution(currEvent, lastEvent, nextEvent,
				i, waitToSub);
		}

	}
}

/* Helper to generate stats for Player */
void printPlayerStats(Player player) {

	if (player.getOffPossessions() == 0 && player.getDefPossessions() == 0) {
		std::cout << player.getPlayerID() << ": Did Not Play" << std::endl;
		return;
	}

	double oRating = player.getPointsFor() /
		(double)player.getOffPossessions();
	double dRating = player.getPointsAgainst() /
		(double)player.getDefPossessions();

	int plusMinus = player.getPointsFor() - player.getPointsAgainst();

	std::cout << player.getPlayerID() << ": OR " <<
		(oRating * ONE_HUNDRED_POSSESSIONS) << ", DR " <<
		(dRating * ONE_HUNDRED_POSSESSIONS) << "; +/-: " << plusMinus
		<< " Off Pos: " << player.getOffPossessions() << "; Def Pos: "
		<< player.getDefPossessions() << std::endl;
}

void Game::printRatings() {

	std::cout << "Home Stats: " << std::endl << std::endl;
	for (Player player : homeTeam.getRoster()) {
		printPlayerStats(player);
	}

	std::cout << std::endl << "Away Stats: " << std::endl << std::endl;
	for (Player player : awayTeam.getRoster()) {
		printPlayerStats(player);
	}

	std::cout << std::endl;
}