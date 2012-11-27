//TODO Remove this and place asserts in all relevant functions

/* Bot.H
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This is the header file for the Bot class. This class creates a bot object
 * that plays a game of ANTS. This Bot will set up the state of the game,
 * reads in the state each turn and submits the moves to the state to be
 * sent to the engine.
 */

#ifndef BOT_H_
#define BOT_H_

#include "State.H"
#include "Ant.H"
#include "Location.H"
#include "Tile.H"
#include <list>
#include <iostream>
#include <fstream>

struct Bot{
    State state;
    std::ofstream myFile;

    Bot();
    ~Bot();

    void playGame();    //plays a single game of Ants

    Location BFS(Tile &start);

    void aStar(Location start, Location goal, Ant& ant);

    void printToFile(int i1, int i2);

    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves

    /////////////////////////////////

    class battleZone{
    public:
    	Tile* inValid;
    	std::list<Tile*> tiles;
    	std::list<Ant> friends;
    	std::list<Ant> foes;

    	int maxDepth;
    	int bestValue;
//    	int alpha;
//    	int beta;
    	int antIndex;
    	// hash_map<battleZone hash value, battleZone evaluated value>
    	// hash_map<double, double> knownZones; //TODO: implement?

    	battleZone(Location border){/* Add Tiles to tiles if ant from that position can move onto to enter battle*/}

    	Tile* nextAnt(); // return Tile of next ant (friends first, then foes)
    	bool validStep(Tile& start, int direction); // return true iff next step is valid
    	void takeStep(Location start, int direction); // move ant and mark as moved ... store move in path?
    	void undoStep(Location start, int direction); // reverse takeStep
    	int evaluate(); // return (kills - deaths)

    	double makeHashValue(); // return unique double representing battleZone //TODO: magic
    };
    // Simulate a battle at the border to predict best ant moves
    void doBattle(Tile* border); //TODO: make moves or return moves to be made?
    // Return (kills - losses) by starting to move with indicated ant (startLocation indicates antToMove)
    int multiAB(int depth, battleZone &battleZone, int alpha, int beta, Tile &startLocation); // battleZone stores best moves
    // return true iff friendly and on l
    bool isFriendly(Location l); //TODO: Find a better way to do this? Related to depth?
    // determine value of battleZone (kills - losses)
    int evaluate(Tile** battleZone);

};

#endif //BOT_H_

// BFS
// ============================================================================
//	LinkedList<Tile> openList = new LinkedList<Tile>();
//	LinkedList<Tile> changedTiles = new LinkedList<Tile>();
//	openList.add(foodTile);
//	foodTile.dist = 0;
//	foodTile.isReached = true;
//	changedTiles.add(foodTile);
//	while (!openList.isEmpty()) {
//	    Tile tile = openList.removeFirst();
//	    if (tile.dist >= 10) break;
//	    for (Tile n : tile.neighbors) {
//	            if (n.isReached) continue;
//	            if (n.type == Type.MY_ANT) {
//	                    // found one of my ants at the tile n
//	            }
//	            n.isReached = true;
//	            n.dist = tile.dist + 1;
//	            changedTiles.add(n);
//	            openList.add(n);
//	    }
//	}
//	for (Tile tile : changedTiles) tile.isReached = false;
// ============================================================================

// ============================================================================
/* Psuedo code to be implemented for basic functionality
 *
 * Avoiding Collisions
 * Create a method to check a move against Water, and the moves of other ants
 * - Need to create a structure to hold all move for the turn
 * - Create a method to check the move
 * - it will take in the ants current location and a direction
 * - if the direction will take it to an occupied square, or one that will be
 * occupied next turn then return false. If both are passed return true
 * - add that location to the set of moves to be performed next turn
 *
 * Gathering food
 * - When we have found a new food, need to find the closest ant
 * - Once a food is being collected, prevent others from going after it as well
 * - each food will be collected by the closest ant, any ants left over after that
 * should now default to exploring
 * - The path to the food should be done with A*
 *
 * Not Blocking hills
 * - Check if an ant is currently on the hill, is so move off
 * - If a move will take an ant onto our hill prevent this
 * - even if the move would make a more optimal path, disregard it
 *
 * Exploring the map
 * - Get a list of all the tiles we have not seen yet
 * - when an ant is not collecting food, go to explore these
 * - Each new unseen tile will be reached by A*
 *
 * Attacking the Enemy hills
 * - For starters, regardless of numbers once an enemy hill is found move all
 * ants to it and try to take it out
 *
 * RUN AWAY
 * - Create a method for checking if, an enemy is visible, can it make a move
 * to kill one of our ants in its current location. If so move our ant in a
 * direction that will move it out of harms way
 *
 */
// ============================================================================
