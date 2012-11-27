//TODO Remove this and place asserts in all relevant functions

/* State.C
 * 		Authors: Eric Klinger
 *
 * This class creates a state object. This object holds all the initial
 * settings for a game of Ants. It also receives information each turn
 * from the engine and stores it. It will also accept ant moves and send
 * them to the engine.
 *
 */

#include "State.H"

using namespace std;

//constructor
State::State(){
    gameover = 0;
    turn = 0;
    bug.open("./debug.txt");
};

//destructor
State::~State(){
    bug.close();
};

//sets the state up
void State::setup(){
    theGrid = vector<vector<Tile> >(rows, vector<Tile>(cols, Tile()));

    //TODO need to add a loop to initialize all of the north, east, south, west pointers
    // I am hoping that the &theGrid... is the right way to have the pointers work in this case
	for(int row = 0; row < rows; row++){
		//theGrid[row] = new Tile[cols];
		for(int col = 0; col < cols; col++){
    		theGrid[row][col].setRow(row);
    		theGrid[row][col].setCol(col);
    		//Need to set up all the cardinal pointers in the tiles
    		theGrid[row][col].neighbors = new Tile*[4];
    		theGrid[row][col].neighbors[0] = &theGrid[(row-1)%rows][col];	//N
    		theGrid[row][col].neighbors[2] = &theGrid[(row+1)%rows][col];	//S
    		theGrid[row][col].neighbors[3] = &theGrid[row][(col-1)%cols];	//W
    		theGrid[row][col].neighbors[1] = &theGrid[row][(col+1)%cols];	//E
		}
    }
    //TODO add any more pre-calculations in here that we may want, we have time=3000 to work with
};

//resets all non-water squares to land and clears the bots ant vector
void State::reset(){
    myAnts.clear();
    enemyAnts.clear();
    myHills.clear();
    enemyHills.clear();
    newVisibleFood.clear();
    orders.clear();
    //TODO add more clears for any new structures that need to be modified after a turn
    for(int row=0; row<rows; row++)
        for(int col=0; col<cols; col++)
            if(!theGrid[row][col].isWater())
                theGrid[row][col].reset();
    			//TODO if something needs to be set of preserved add it here
};

//outputs move information to the engine
void State::makeMove(const Location &loc, int direction){
    cout << "o " << loc.row << " " << loc.col << " " << CDIRECTIONS[direction] << endl;

    Location nLoc = getLocation(loc, direction);
    //OLD
//    theGrid[nLoc.row][nLoc.col].ant = theGrid[loc.row][loc.col].ant;
//    theGrid[loc.row][loc.col].ant = -1;
    //NEW
    theGrid[nLoc.row][nLoc.col].setAntPlayer(theGrid[loc.row][loc.col].getAntPlayer());
    theGrid[loc.row][loc.col].setAntPlayer(-1);
};

//returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2){
    int d1 = abs(loc1.row-loc2.row),
        d2 = abs(loc1.col-loc2.col),
        dr = min(d1, rows-d1),
        dc = min(d2, cols-d2);
    return sqrt(dr*dr + dc*dc);
};

//returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &loc, int direction){
    return Location( (loc.row + DIRECTIONS[direction][0] + rows) % rows,
                     (loc.col + DIRECTIONS[direction][1] + cols) % cols );
};

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation(){
    std::queue<Location> locQueue;
    Location sLoc, cLoc, nLoc;

    for(int a=0; a<(int) myAnts.size(); a++){
        sLoc = myAnts[a];
        locQueue.push(sLoc);

        std::vector<std::vector<bool> > visited(rows, std::vector<bool>(cols, 0));
        theGrid[sLoc.row][sLoc.col].setVisible();
        visited[sLoc.row][sLoc.col] = 1;

        while(!locQueue.empty()){
            cLoc = locQueue.front();
            locQueue.pop();

            for(int d=0; d<TDIRECTIONS; d++){
                nLoc = getLocation(cLoc, d);

                if(!visited[nLoc.row][nLoc.col] && distance(sLoc, nLoc) <= viewradius){
                    theGrid[nLoc.row][nLoc.col].setVisible();
                    locQueue.push(nLoc);
                }
                visited[nLoc.row][nLoc.col] = 1;
            }
        }
    }
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream& operator<<(ostream &os, const State &state){
	//TODO need to fix this error!
//    for(int row=0; row<state.rows; row++){
//        for(int col=0; col<state.cols; col++){
//            if(state.theGrid[row][col].isWater)
//                os << '%';
//            else if(state.theGrid[row][col].isFood)
//                os << '*';
//            else if(state.theGrid[row][col].isHill)
//                os << (char)('A' + state.theGrid[row][col].getHillPlayer);
//            else if(state.theGrid[row][col].getAntPlayer >= 0)
//                os << (char)('a' + state.theGrid[row][col].getAntPlayer);
//            else if(state.theGrid[row][col].isVisible)
//                os << '.';
//            else
//                os << '?';
//        }
//        os << endl;
//    }
    return os;
};

//input function
istream& operator>>(istream &is, State &state){
    int row, col, player;
    string inputType, junk;

    //finds out which turn it is
    while(is >> inputType){
        if(inputType == "end"){
            state.gameover = 1;
            break;
        }
        else if(inputType == "turn"){
            is >> state.turn;
            break;
        }
        else //unknown line
            getline(is, junk);
    }
    if(state.turn == 0){
        //reads game parameters
        while(is >> inputType){
            if(inputType == "loadtime")
                is >> state.loadtime;
            else if(inputType == "turntime")
                is >> state.turntime;
            else if(inputType == "rows")
                is >> state.rows;
            else if(inputType == "cols")
                is >> state.cols;
            else if(inputType == "turns")
                is >> state.turns;
            else if(inputType == "player_seed")
                is >> state.seed;
            else if(inputType == "viewradius2"){
                is >> state.viewradius;
                state.viewradius = sqrt(state.viewradius);
            }
            else if(inputType == "attackradius2"){
                is >> state.attackradius;
                state.attackradius = sqrt(state.attackradius);
            }
            else if(inputType == "spawnradius2"){
                is >> state.spawnradius;
                state.spawnradius = sqrt(state.spawnradius);
            }
            else if(inputType == "ready") //end of parameter input
            {
                state.timer.start(); break;
            }
            else    //unknown line
                getline(is, junk);
        }
    }
    else{
        //reads information about the current turn
        while(is >> inputType){
            if(inputType == "w") //water square
            {
                is >> row >> col;
                state.theGrid[row][col].setWater();
                //TODO need to destroy all the pointers going into this tile from surrounding
                //TODO need to destroy lots of data within the tile, like its outgoing pointers
            }
            else if(inputType == "f") //food square
            {
                is >> row >> col;

                //TODO Add a check to see if this food is already being gathered
                // Look in set beingGathered
//                std::set<Location>::iterator it = state.beingGathered.find(Location(row, col)),
//                		end = state.beingGathered.end();
                state.theGrid[row][col].setFood();
                state.newVisibleFood.push_back(Location(row, col));
            }
            else if(inputType == "a") //live ant square
            {
                is >> row >> col >> player;
                //TODO decide if we should bother with player numbers, or treat all enemies the same
                state.theGrid[row][col].setAntPlayer(player);
                state.theGrid[row][col].setOccupied();
                if(player == 0)
                    state.myAnts.push_back(Location(row, col));
                else
                    state.enemyAnts.push_back(Location(row, col));
            }
            else if(inputType == "d") //dead ant square
            {
                is >> row >> col >> player;
                //TODO reDirect this dead ant info elsewhere
                //state.theGrid[row][col].deadAnts.push_back(player);
            }
            else if(inputType == "h")
            {
                is >> row >> col >> player;
                state.theGrid[row][col].setHill();
                //TODO again as above, track player numbers or treat all enemies the same?
                state.theGrid[row][col].setHillPlayer(player);
                state.theGrid[row][col].setOccupied();
                if(player == 0)
                    state.myHills.push_back(Location(row, col));
                else
                    state.enemyHills.push_back(Location(row, col));

            }
            else if(inputType == "players") //player information
                is >> state.noPlayers;
            else if(inputType == "scores") //score information
            {
                state.scores = vector<double>(state.noPlayers, 0.0);
                for(int p=0; p<state.noPlayers; p++)
                    is >> state.scores[p];
            }
            else if(inputType == "go") //end of turn input
            {
                if(state.gameover)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            }
            else //unknown line
                getline(is, junk);
        }
    }
    return is;
};
