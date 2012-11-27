//TODO Remove this and place asserts in all relevant functions

/* State.H
 * 		Authors: Eric Klinger
 *
 * This class creates a state object. This object holds all the initial
 * settings for a game of Ants. It also receives information each turn
 * from the engine and stores it. It will also accept ant moves and send
 * them to the engine.
 *
 */

#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <stdint.h>

#include "Timer.H"
#include "Bug.H"
#include "Location.H"
#include "Tile.H"
#include <map>
#include <set>

//constants
const int TDIRECTIONS = 4;
const char CDIRECTIONS[4] = {'N', 'E', 'S', 'W'};
const int DIRECTIONS[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };      //{N, E, S, W}

struct State
{
	//Variables
    int rows, cols,
        turn, turns,
        noPlayers;
    double attackradius, spawnradius, viewradius;
    double loadtime, turntime;
    std::vector<double> scores;
    bool gameover;
    int64_t seed;

    struct CompLocations
    {
      // return true iff a < b
      bool operator()(const Location &a, const Location &b) {
        if(a.row < b.row)	return true;
        else if(a.col < b.col) return true;
        else
        	return false;
      }
    };

    std::vector<std::vector<Tile> > theGrid;

    //std::set<Tile> unseenTiles;		//All tile we have yet to see
    //std::set<Location, CompLocations> beingGathered;	//List of all food being gathered
    std::set<Location, CompLocations> orders;

    std::vector<Location> newVisibleFood; //All new visible food, start of turn
    std::vector<Location> myHills;		//Our live hills
    std::vector<Location> enemyHills;		//live enemy hills
    std::vector<Location> myAnts;	//All our ants that have yet to move
    std::vector<Location> enemyAnts;		//All visible emeny ants

    Timer timer;
    Bug bug;

    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);

    double distance(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &startLoc, int direction);

    void updateVisionInformation();
};

std::ostream& operator<<(std::ostream &os, const State &state);
std::istream& operator>>(std::istream &is, State &state);

#endif //STATE_H_
