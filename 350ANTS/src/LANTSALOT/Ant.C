//TODO Remove this and place asserts in all relevant functions. Yea, what he said

/* Ant.C
 * 		Author: Eric Klinger, Jake Brand
 *
 * This is the source file for the Ant class. This object stores information
 * about a live ant under our control and what tasks/missions it may have been
 * assigned from a previous turn
 */

#include "Ant.H"
#include "State.H"

using namespace std;

Ant::Ant() {
	row = col = -1;
	task = moved = 0;
	currentTask = NONE;
}

Ant::Ant(int row, int col){
	this->row = row;
	this->col = col;
	task = moved = 0;
	currentTask = NONE;
}

Ant::~Ant() {
	// TODO Auto-generated destructor stub
	// TODO nothing to clear, no heap allocation
}
