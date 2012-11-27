//TODO Remove this and place asserts in all relevant functions

/* Ant.H
 * 		Author: Eric Klinger, Jake Brand
 *
 * This is the header file for the Ant class. This object stores information
 * about a live ant under our control and what tasks/missions it may have been
 * assigned from a previous turn
 */

#ifndef ANT_H_
#define ANT_H_

#include <vector>
#include "Tile.H"

class Ant {
public:

	enum Task {NONE, EXPLORE, EXPAND, GATHER, FIGHT, DEFEND };

	std::vector<Tile*> route;

	Ant();
	Ant(int row, int col);
	virtual ~Ant();

	//TODO see if there is a need for CC or AO

	//TODO add getters and setters as needed
	int getRow(){
		return row;
	}
	int getCol(){
		return col;
	}
private:
	int row, col;
	bool task, moved;
	//TODO add functions to work on functions and check conditions
	Task currentTask;
};

#endif /* ANT_H_ */
