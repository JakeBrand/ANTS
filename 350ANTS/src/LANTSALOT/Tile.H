#ifndef TILE_H_
#define TILE_H_

//TODO Remove this and place asserts in all relevant functions

/* Tile.C
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This class is used to represent one tile on the map
 * It will hold information about what's on that tile during the turn
 * If its a water tile, that will never be reset since we only get that
 * information once
 *
 * Instead of having a reset on all items, try to use the generation
 * counter trick for a future implementation
 */

#include "Location.H"

class Tile{
public:
	//Pointers to the surrounding tiles
	Tile **neighbors;

	//Constructs a new Tile
	//Tile* will be initialized in the Grid class which uses tiles
	Tile(){
		//Set all the boolean flags to 0
		food = hill = water = explored = visible = reached = distance = occupied = 0;
		//set player number and distance to -1 meaning non
		antPlayer = hillPlayer = -1;
		//4 neighbor tiles for each tile on the map, accounting for
		//neighbors = new Tile*[4];
	}

	void reset(){
		//TODO add more initial states that are added to the tiles
		food = hill = visible = reached = distance = occupied = 0;
		antPlayer = hillPlayer = -1;
	}

	//Getter Setter pair for Food
	void setFood(){
		food = 1;
	}
	bool isFood(){
		return food;
	}
	//Getter Setter pair for Hill
	void setHill(){
		hill = 1;
	}
	bool isHill(){
		return hill;
	}
	//Getter Setter pair for water
	void setWater(){
		water = 1;
	}
	bool isWater(){
		return water;
	}
	//Getter Setter pair for Explored
	void setExplored(){
		explored = 1;
	}
	bool isExplored(){
		return explored;
	}
	//Getter Setter pair for Visible
	void setVisible(){
		visible = 1;
	}
	bool isVisible(){
		return visible;
	}
	//Getter Setter pair for Occupied
	void setOccupied(){
		occupied = 1;
	}
	bool isOccupied(){
		return occupied;
	}
	void unOccupied(){
		occupied = 0;
	}
	//Getter Setter pair for Reached
	void setReached(){
		reached = 1;
	}
	bool isReached(){
		return reached;
	}
	void notReached(){
		reached = 0;
	}
	//Getter Setter pair for ant
	void setAntPlayer(int playerNumber){
		antPlayer = playerNumber;
	}
	int getAntPlayer(){
		return antPlayer;
	}
	//Getter Setter pair for hillPlayer
	void setHillPlayer(int playerNumber){
		hillPlayer = playerNumber;
	}
	int getHillPlayer(){
		return hillPlayer;
	}
	//Getter Setter pair for distance
	void setDistance(int dist){
		distance = dist;
	}
	int getDistance(){
		return distance;
	}
	//Getter and Setter for Col and Row in loc (X,Y)
	void setCol(int col){
		loc.col = col;
	}
	int getCol(){
		return loc.col;
	}
	void setRow(int row){
		loc.row = row;
	}
	int getRow(){
		return loc.row;
	}


	//Check if a tile has a friendly ant on it or not
	bool friendlyAnt(){
		if(antPlayer == 0){
			return true;
		}return false;
	}

	//AO for a Tile object
	Tile& operator=(const Tile& rhsTile){
		//Guard against self assignment
		if(this == &rhsTile) return *this;
		//Make this = rhsTile
		this->antPlayer = rhsTile.antPlayer;
		this->hillPlayer = rhsTile.hillPlayer;
		this->distance = rhsTile.distance;
		this->food = rhsTile.food;
		this->hill = rhsTile.hill;
		this->water = rhsTile.water;
		this->explored = rhsTile.explored;
		this->visible = rhsTile.visible;
		this->reached = rhsTile.reached;
		return *this;
	}

private:
	int antPlayer, hillPlayer, distance;
	bool food, hill, water, explored, visible, reached, occupied;
	Location loc;

	//TODO add more state to the tile rather than keeping it in lists
};

#endif //TILE_H_
