/* NBA Basketball Project */
// Author: Kevin M. Smith <kmsmith3@vt.edu>
// Version: May 30, 2019 <v2.0>

// This Project was developed using CMake 3.13.3 and Windows
// Visual Studio 2017

// Version 2.0 reads all Play_by_Play.txt data into a vector of Events 
// for each Game, which will then be sequentially evaluated
//
// Player plus/minus should all be accurate in v2.0

#include "event.hpp"
#include "game.hpp"
#include "player.hpp"
#include "team.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#define BBALL_NULL	-1.0

// Values in Off Rtg & Def Rtg calculation
#define ONE_HUNDRED_POSSESSIONS	100.0
#define ZERO					0.0

// Periods in Game File
#define PERIOD_NULL		"0"
#define FIRST_PERIOD	"1"

// Number of Players per period in Game File
#define FULL_COURT	10

// Game File Token Values
#define GAME_GAME_ID	0
#define GAME_PERIOD		1
#define GAME_PLAYER_ID	2
#define GAME_TEAM_ID	3
#define STATUS			4

// Play File Token Values
#define PLAY_GAME_ID	0
#define EVENT_NUMBER	1
#define EVENT_TYPE		2
#define PLAY_PERIOD		3
#define WC_TIME			4
#define PC_TIME			5
#define ACTION_TYPE		6
#define OPTION1			7
#define OPTION2			8
#define OPTION3			9
#define PLAY_TEAM_ID	10
#define PLAY_PERSON1	11
#define PLAY_PERSON2	12
#define PLAY_PERSON3	13
#define PLAY_TEAM_TYPE	14
#define PERSON1_TYPE	15
#define PERSON2_TYPE	16
#define PERSON3_TYPE	17


using namespace std;


/* Clean double quotes from data */
std::string cleanString(std::string str) {

	std::string cleaned;

	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '"') cleaned.push_back(str[i]);
	}

	return cleaned;
}

/* Checks if File line contains header data */
bool isValidLine(std::string line) {

	std::size_t check = line.find("Game");

	return check == std::string::npos;
}

/* Simulate vector of Games */
std::vector<Game> simulateGames(std::vector<Game> gamesToPlay) {

	std::vector<Game> playedGames;

	for (Game currGame : gamesToPlay) {

		currGame.simulateGame();

		currGame.updateRosters();

		currGame.printRatings();

		playedGames.push_back(currGame);
	}

	return playedGames;
}

/* Create an Event from Play line data */
Event makeEvent(std::string playLine) {

	std::stringstream strStream(playLine);

	std::string playData;

	std::vector<std::string> playTokens;

	while (getline(strStream, playData, '\t')) {
		playTokens.push_back(cleanString(playData));
	}

	int eventNumber = std::stoi(playTokens[EVENT_NUMBER]);
	int eventType = std::stoi(playTokens[EVENT_TYPE]);
	int period = std::stoi(playTokens[PLAY_PERIOD]);
	int actionType = std::stoi(playTokens[ACTION_TYPE]);
	int wcTime = std::stoi(playTokens[WC_TIME]);
	int pcTime = std::stoi(playTokens[PC_TIME]);
	int option = std::stoi(playTokens[OPTION1]);

	std::string gameID = playTokens[PLAY_GAME_ID];
	std::string player1ID = playTokens[PLAY_PERSON1];
	std::string player2ID = playTokens[PLAY_PERSON2];
	std::string player3ID = playTokens[PLAY_PERSON3];
	std::string teamID = playTokens[PLAY_TEAM_ID];

	playTokens.clear();

	return Event(eventNumber, eventType, period, actionType, wcTime, pcTime,
		option, Player(player1ID), Player(player2ID), Player(player3ID),
		Team(teamID));
}

/* Make vector of Games with all Play Events */
std::vector<Game> getGameEvents(std::vector<Game> gamesToMake,
	std::ifstream *playStream)
{

	std::vector<Game> madeGames;

	for (Game game : gamesToMake) {

		std::string playLine;

		while (getline(*playStream, playLine)) {

			if (isValidLine(playLine)) {

				// Turn Play Data line into Event
				Event currEvent = makeEvent(playLine);

				game.addEvent(currEvent);

				if (currEvent.isGameCompleted()) {
					game.sortEvents();
					madeGames.push_back(game);
					break;
				}
			}
		}

	}

	return madeGames;
}

/* Bench Players not on Team court */
Team benchPlayers(Team team) {
	for (Player player : team.getRoster()) {
		if (!team.isOnCourt(player)) team.addToBench(player);
	}

	return team;
}

/* Tokenize Game data from Game line */
std::vector<std::string> getGameData(std::string gameLine) {

	std::stringstream strStream(gameLine);

	std::string gameData;

	std::vector<std::string> gameTokens;

	while (getline(strStream, gameData, '\t')) {
		gameTokens.push_back(cleanString(gameData));
	}

	return gameTokens;
}

/* Make Game from Game ID, Teams, and vector of starters */
Game makeGame(std::string gameID, Team homeTeam, Team awayTeam,
	std::vector<std::vector<Player>> starters) {

	homeTeam = benchPlayers(homeTeam);
	awayTeam = benchPlayers(awayTeam);

	Game game = Game(gameID, homeTeam, awayTeam);

	for (int i = 0; i < starters.size(); i++) {
		game.addStarters(starters[i]);
	}

	return game;
}

/* Make Player from Player ID and set if active */
Player makePlayer(std::vector<std::string> gameTokens) {

	Player player(gameTokens[GAME_PLAYER_ID]);

	if (gameTokens[STATUS] == "A") player.activate();

	return player;
}

/* Return Team with new Player added to roster */
Team addPlayerToRoster(Team team, std::vector<std::string> gameTokens) {

	Player player = makePlayer(gameTokens);

	team.addPlayer(player);

	return team;
}

/* Make Team from Game ID and add first Player */
Team makeTeam(std::vector<std::string> gameTokens) {

	Team team(gameTokens[GAME_TEAM_ID]);

	team = addPlayerToRoster(team, gameTokens);

	return team;
}

/* Make Game rosters for both teams */
// - First Team listed in Game File is "Home" Team by default
std::vector<Game> makeRosters(std::ifstream *gameStream) {

	std::vector<Game> games;

	Game game;

	Team homeTeam, awayTeam;

	std::vector<std::vector<Player>> allStarters;
	std::vector<Player> starters;

	std::string gameID = "", homeTeamID = "", awayTeamID = "", playerID = "";

	std::vector<std::string> gameTokens;

	std::string gameLine;

	while (getline(*gameStream, gameLine)) {

		if (isValidLine(gameLine)) {

			gameTokens = getGameData(gameLine);

			if (gameID == "") gameID = gameTokens[GAME_GAME_ID];
			// Make new Game if GameIDs dont match
			else if (gameID != gameTokens[GAME_GAME_ID]) {

				game = makeGame(gameID, homeTeam, awayTeam, allStarters);

				allStarters.clear();

				games.push_back(game);
				gameID = gameTokens[GAME_GAME_ID];
				homeTeamID = "", awayTeamID = "";
			}

			// Add all Players to respective roster
			if (gameTokens[GAME_PERIOD] == PERIOD_NULL) {

				if (homeTeamID == "") {
					homeTeamID = gameTokens[GAME_TEAM_ID];
					homeTeam = makeTeam(gameTokens);
				}
				else if (homeTeamID == gameTokens[GAME_TEAM_ID]) {
					homeTeam = addPlayerToRoster(homeTeam, gameTokens);
				}
				else if (awayTeamID == "") {
					awayTeamID = gameTokens[GAME_TEAM_ID];
					awayTeam = makeTeam(gameTokens);
				}
				else if (awayTeamID == gameTokens[GAME_TEAM_ID]) {
					awayTeam = addPlayerToRoster(awayTeam, gameTokens);
				}
			}
			// Add starters to starter vector and add to court if first period
			else {
				if (homeTeamID == gameTokens[GAME_TEAM_ID]) {
					if (gameTokens[GAME_PERIOD] == FIRST_PERIOD) {
						homeTeam.addToCourt(Player(gameTokens[GAME_PLAYER_ID]));
					}
					starters.push_back(Player(gameTokens[GAME_PLAYER_ID]));
				}
				else if (awayTeamID == gameTokens[GAME_TEAM_ID]) {
					if (gameTokens[GAME_PERIOD] == FIRST_PERIOD) {
						awayTeam.addToCourt(Player(gameTokens[GAME_PLAYER_ID]));
					}
					starters.push_back(Player(gameTokens[GAME_PLAYER_ID]));
				}

				if (starters.size() == FULL_COURT) {
					allStarters.push_back(starters);
					starters.clear();
				}
			}
			gameTokens.clear();
		}
	}

	// Add last Game after all Game lines read
	game = makeGame(gameID, homeTeam, awayTeam, allStarters);

	games.push_back(game);

	return games;
}

/* Write Player Off Rtg and Def Rtg to Data File */
void writePlayerData(Player player, std::string gameID,
	std::ofstream *dataStream) {

	std::string gid = "\"" + gameID + "\"";
	std::string pid = "\"" + player.getPlayerID() + "\"";

	int pointsFor = player.getPointsFor();
	int pointsAgainst = player.getPointsAgainst();

	int offPoss = player.getOffPossessions();
	int defPoss = player.getDefPossessions();

	double oRating = BBALL_NULL;
	double dRating = BBALL_NULL;

	if (offPoss == ZERO) oRating = ZERO;
	else oRating = (pointsFor / (double)offPoss) * ONE_HUNDRED_POSSESSIONS;

	if (defPoss == ZERO) dRating = ZERO;
	else dRating = (pointsAgainst / (double)defPoss) * ONE_HUNDRED_POSSESSIONS;

	*dataStream << std::fixed << std::setprecision(1) << gid << ","
		<< pid << "," << oRating << "," << dRating << std::endl;
}

/* Write Game Off Rtg & Def Rtg stats to Data File */
void writeToDataFile(std::vector<Game> games, std::ofstream *dataStream) {

	*dataStream << "\"Game_id\",\"Person_id\",\"OffRtg\",\"DefRtg\""
		<< std::endl;

	for (Game game : games) {
		for (Player player : game.getHomeTeam().getRoster()) {
			writePlayerData(player, game.getGameID(), dataStream);
		}
		for (Player player : game.getAwayTeam().getRoster()) {
			writePlayerData(player, game.getGameID(), dataStream);
		}
	}
}

/* BBall Main Function */
int main(int argc, char **argv) {

	std::vector<Game> games;

	std::ifstream gameFile("Game_Lineup.txt");
	std::ifstream playFile("Play_by_Play.txt");

	std::ofstream dataFile("Kevin_M_Smith_Q1_BBALL.csv");

	if (gameFile.is_open()) {
		games = makeRosters(&gameFile);

		if (playFile.is_open()) {

			games = getGameEvents(games, &playFile);
			games = simulateGames(games);

			if (dataFile.is_open()) {
				writeToDataFile(games, &dataFile);
			}

			std::cout << std::endl << "**Done**" << std::endl;
		}
	}
}