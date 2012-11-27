//TODO Remove this and place asserts in all relevant functions

/* Bot.C
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This is the source file for the Bot class. This class creates a Bot object
 * that plays a game of ANTS. This Bot will set up the state of the game,
 * reads in the state each turn and submits the moves to the state to be
 * sent to the engine.
 */

#include "Bot.H"

using namespace std;


//constructor
Bot::Bot(){
//	myFile.open("output.txt");
};

//Destructor
Bot::~Bot(){

}

//plays a single game of Ants.
void Bot::playGame(){
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();
    //continues making moves while the game is not over
    while(cin >> state){
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
}

Location Bot::BFS(Tile &start){
	//Initialization
	Location goal = Location(-1,-1);	// the return of location for BFS
	// pointer to start tile, current tile and neighbor tile
	Tile *sTile;
	Tile *cTile;
	Tile *nTile;
	list<Tile*> openTiles;			// list of all tiles to consider next
	list<Tile*> changedTiles;		// list of times we have looked at
	//Body
	sTile = &start;	//initialize the start tile
	sTile->setReached();	//mark start as being reached already
	changedTiles.push_back(sTile);
	openTiles.push_front(sTile);	//add the start tile to be used in search
	//Loop for bfs, go through each level and try to find the goal
	while(!openTiles.empty()){
		//if we are at level 10 we have gone far enough, return
		//get the next tile to consider
		cTile = openTiles.front(); //currentTile is not a pointer!!
		openTiles.pop_front();	//removes first element from the openList
		if(cTile->getDistance() > 5) break;
		for(int i = 0; i<4; i++){
			nTile = cTile->neighbors[i];
			if(nTile->isReached()) continue;
			if(nTile->isWater()) continue;
			if(nTile->isFood()){
			//if(!nTile->isVisible()){
				//Get the location and return it
				goal.col = nTile->getCol();
				goal.row = nTile->getRow();
				goto done;
				}
			nTile->setReached();
			nTile->setDistance(cTile->getDistance() + 1);
			changedTiles.push_back(nTile);
			openTiles.push_back(nTile);
		}
	}//end of while
done:;
	list<Tile*>::iterator it = changedTiles.begin(), end = changedTiles.end();
	for( ; it != end; ++it){
		(*it)->notReached();
		(*it)->setDistance(0);
	}
	changedTiles.clear();
	openTiles.clear();
	return goal;
}

void Bot::aStar(Location start, Location goal, Ant& ant){
	ant.route.clear();
}

//makes the bots moves for the turn
void Bot::makeMoves(){
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

    //picks out moves for each ant
    //TODO use a static cast here
    for(int ant=0; ant<(int)state.myAnts.size(); ant++){
    	Location goal = Location(-1,-1);
    	Location next, bestNext;
    	int distance = 100; int tempDistance = 0;
    	int move = -1;
    	Tile start = state.theGrid[state.myAnts[ant].row][state.myAnts[ant].col];
    	//This code checks if the ant is currently on a hill. If so move off it if possible
    	if(start.getHillPlayer() >= 0){
    		for(int i=0; i<4; i++){
    			if(!start.neighbors[i]->isWater() && !start.neighbors[i]->isOccupied()){
    				state.orders.insert(state.getLocation(state.myAnts[ant], i));
    				start.unOccupied();
    				state.makeMove(state.myAnts[ant], i);
    				goto NextAnt;
    			}
    		}
    	}

    	goal = BFS(start);
    	if(goal.row == -1){
    		//need to explore rather than try and find food
    		//for not dont move this ant and move to the next one
    		continue;
    	}
    	//Check north
    		next = state.getLocation(state.myAnts[ant], 0);
    		tempDistance = state.distance(next, goal);
    		if(tempDistance <= distance){
                if(!state.theGrid[next.row][next.col].isWater()
                		&& !state.theGrid[next.row][next.col].isOccupied()
                		&& state.orders.find(next) == state.orders.end()){
    			distance = tempDistance;
    			move = 0;
    			bestNext = next;
                }
    		}
    	//East
			next = state.getLocation(state.myAnts[ant], 1);
			tempDistance = state.distance(next, goal);
    		if(tempDistance <= distance){
                if(!state.theGrid[next.row][next.col].isWater()
                		&& !state.theGrid[next.row][next.col].isOccupied()
                		&& state.orders.find(next) == state.orders.end()){
    			distance = tempDistance;
    			move = 1;
    			bestNext = next;
                }
    		}
    	//South
			next = state.getLocation(state.myAnts[ant], 2);
			tempDistance = state.distance(next, goal);
    		if(tempDistance <= distance){
                if(!state.theGrid[next.row][next.col].isWater()
                		&& !state.theGrid[next.row][next.col].isOccupied()
                		&& state.orders.find(next) == state.orders.end()){
    			distance = tempDistance;
    			move = 2;
    			bestNext = next;
                }
    		}
    	//West
			next = state.getLocation(state.myAnts[ant], 3);
			tempDistance = state.distance(next, goal);
    		if(tempDistance <= distance){
                if(!state.theGrid[next.row][next.col].isWater()
                		&& !state.theGrid[next.row][next.col].isOccupied()
                		&& state.orders.find(next) == state.orders.end()){
    			distance = tempDistance;
    			move = 3;
    			bestNext = next;
                }
    		}
    		//If the move == -1 then no move is possible
    		if(move == -1){
    			continue;
    		}
    		else{
    		//add the location the ant will be into orders
			state.orders.insert(bestNext);
			//this spot that ant is current is will be unoccupied
			start.unOccupied();
			//send this move to the engine
			state.makeMove(state.myAnts[ant], move);
    		}
NextAnt:;
    }
    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn(){
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};







    /////////////////////////////////////////

// Return next ant in battleZone (friends first, foes second)
	Tile* Bot::battleZone::nextAnt(){
		//TODO: get in index, start with friendly

		// friendly ant
		if(antIndex < friends.size()){
//			return Tile ant is on
		} else if(antIndex < (friends.size()+foes.size())){
//			return Tile ant is on
		} else{
			return inValid;
		}
		return inValid;
	}

	// return true iff moving from start towards direction is valid
	bool Bot::battleZone::validStep(Tile& start, int direction){
		Tile* next = start.neighbors[direction];
		list<Tile*>::iterator it = tiles.begin();
		list<Tile*>::iterator end = tiles.end();

		// O(n) is not good
		for(;it != end;it++){
			if(*it == next){
				break;
			}
		}
		// retreating or water or occupied //todo: switch to "will be occupied"
		if(it == end || next->isWater() || next->isOccupied()){
			return false;
		}
		return true;
	}

	// simulate step
	void Bot::battleZone::takeStep(Location start, int direction){
		// Ant at start location makes path = direction;
		//TODO: make step happen
	}

	// undo last step
	void Bot::battleZone::undoStep(Location start, int direction){
		// Move ant back... find a way to remember path...
		//TODO: path...
	}

	int Bot::battleZone::evaluate(){
		int value = 0;


		return value;
	}
/*

	// His implementation concept
	void max(int antLoc, Bot::battleZone bz){
		if(antLoc < 5 numFriendAnts){
			for(int i=0;i< 5 size;i++){
//				if(bz.validStep(antLoc,i)){
//					bz.takeStep(antLoc, i);
//					max(bz.nextAnt(), bz);
//					bz.undoStep(antLoc, bz);
//				}
			}
		} else{
			int value = min(0, bz);
			if(value < bz.bestValue){
				bz.bestValue = value;
				// Save current state (as []? as un-hashable value?)

			}
		}
	}

	int min(int antLoc, Bot::battleZone bz){
		if(antLoc < 5 numEnemyAnts){
			int alpha = 1000; //infinity
			for(int i = 0; i< 5 numSteps;i++){
	//			if(bz.validStep(antLoc, i);
	//				bz.takeStep(antLoc, i);
	//				int value = min(bz.nextAnt(), bz);
	//				bz.undoStep(antLoc, bz);
	//				if(value < bz.bestValue){ return -1000;} //-Infinity
//					if(value < alpha){
//						alpha == value;
//						return value;
//					}
	//			}
			}
		}

		return bz.evaluate();

	}
*/

	// My implementation concept
    int Bot::multiAB(int depth, battleZone &battleZone, int alpha, int beta, Tile &startLocation){

    	int currentValue = -1000; // -INFINITY
    	// Leaf node. Return value
/*    	if(startLocation == battleZone.inValid || depth == battleZone.maxDepth){ // C++11 null_ptr?
    //		return battleZone.evaluate();
    	}*/

    	for(int i = 0; i< 5/*startLocation.neighbors->size*/;i++){ //TODO: neighbors.size
    		/*
    		if(battleZone.validMove(startLocation, i)){
    			battleZone.takeStep(startLocation, i); // simulate step

				double hashValue = battleZone.makeHashValue(); //TODO: find a way to hash a Bz to a value
    			if(knownZones.contains(hashValue){
    				currentValue = knownZones.get(hashValue(battleZones).second);
					undoStep();
    				return currentValue;
    			}

    			Tile* nextLoc = battleZone.nextAnt();
    			nextValue = multiAB(depth+1, battleZone, alpha, beta, nextLoc);

    			if(battleZone.antIndex < battleZone.friends.size(){
					if(nextLocation.friendlyAnt(){
						// can't update beta or beta cut?
						if(nextValue > alpha) { alpha = nextValue;}
						if(nextValue > currentValue) {currentValue = nextValue;}
					} else { // friend to foe in tree
						if(nextValue < beta) { beta = nextValue; } //TODO: this doesn't feel right
					}
    			}
    			else { // isEnemy
    				if(nextValue <= alpha) {
    				    undoStep();
    				    return nextValue;
    				} // alpha cut //TODO: still doesn't feel right
    				if(nextValue < beta) { beta = nextValue; } // bring down upper bound
    				if(nextValue < value) { currentValue = nextValue;} // MIN current value
    			}
    			// TODO: store hash value
    			undoStep();
    		}
*/
    	}
    	return currentValue;
    }

