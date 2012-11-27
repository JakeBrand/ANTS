//TODO Remove this and place asserts in all relevant functions

/* MyGrid.C
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This is the source file for the Grid class. Working with A* to get it
 * working for our implementation
 */

#include "MyGrid.H"
#include <cassert>
#include <queue>
#include <cstdlib>

// Helper struct to represent coordinates
struct Point
{
  int x,y;

  Point() : x(-1),y(-1) {}
  Point(int x, int y) : x(x),y(y) {}

  Point move(Grid::Direction direction) const
  {
    static const int offsets[][2] = {
      {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}
    };
    return Point(x+offsets[direction][0],y+offsets[direction][1]);
  }

  bool operator == (const Point & p) const { return x==p.x && y==p.y; }
};

// Retrieve the cost of moving in a particular direction
static int getMoveCost(Grid::Direction direction)
{
  static const int costs[8] = {
    Grid::CARDINAL_COST, Grid::DIAGONAL_COST,
    Grid::CARDINAL_COST, Grid::DIAGONAL_COST,
    Grid::CARDINAL_COST, Grid::DIAGONAL_COST,
    Grid::CARDINAL_COST, Grid::DIAGONAL_COST };
  return costs[direction];
}

// Estimate the cost of moving between two points
static int estimateCost(const Point & a, const Point & b)
{
  const int dx(abs(b.x-a.x));
  const int dy(abs(b.y-a.y));
  return dx > dy ?
    dy * Grid::DIAGONAL_COST + (dx-dy) * Grid::CARDINAL_COST :
    dx * Grid::DIAGONAL_COST + (dy-dx) * Grid::CARDINAL_COST;
}

// Create a grid with set width and height, initialise all tiles to GROUND
Grid::Grid(int width, int height)
  : data(width*height,GROUND), width(width), height(height)
{
  assert(width > 0 && height > 0);

  for(size_t i(0); i<3; ++i) {
    sectors[i].resize(width*height,0);
    nextSector[i] = 1;
  }
  sectorsDirty = false;
}

// Return the tile at (x,y), where 0 <= x < width and 0 <= y < height
Grid::Tile Grid::getTile(int x, int y) const
{
  assert(x >= 0 && x < width);
  assert(y >= 0 && y < height);
  return data[y*width+x];
}

// Set the tile at (x,y), where 0 <= x < width and 0 <= y < height,
// and tile is a valid Tile type.
void Grid::setTile(int x, int y, Tile tile)
{
  assert(tile >= GROUND && tile <= BLOCKED);
  assert(x >= 0 && x < width);
  assert(y >= 0 && y < height);
  data[y*width+x] = tile;
  sectorsDirty = true;
}

void Grid::ResetSectors() const
{
  for (size_t i(0); i<3; ++i) {
    sectors[i].clear();
    sectors[i].resize(width*height,0);
    nextSector[i] = 1;
  }
  sectorsDirty = false;
}

void Grid::TouchSector(int size, int x, int y) const
{
  // If the map has been written to, throw away the sectors
  if (sectorsDirty) {
    ResetSectors();
  }

  // If this point has its sector assigned, return early
  if (sectors[size][y*width+x]) {
    return;
  }

  // Otherwise assign a sector by floodfilling from this point
  FloodFill(size, x, y, getTile(x,y), nextSector[size]);
  ++nextSector[size];
}

void Grid::FloodFill(int size, int x, int y, int tile, int sector) const
{
  // Skip if we have wandered off the map boundaries
  if (x < 0 || y < 0 || x+size >= width || y+size >= height) {
    return;
  }

  // Skip this spot if a sector was assigned here
  int & spotSector(sectors[size][y*width+x]);
  if (spotSector != 0) {
    return;
  }

  // Skip this spot if the terrain is wrong
  for (int i(0); i <= size; ++i) {
    for (int j(0); j <= size; ++j) {
      if (getTile(x+j, y+i) != tile) {
        return;
      }
    }
  }

  // Assign this sector
  spotSector = sector;

  // Flood out in cardinal directions only for accessibility
  FloodFill(size, x-1, y,   tile, sector);
  FloodFill(size, x+1, y,   tile, sector);
  FloodFill(size, x,   y-1, tile, sector);
  FloodFill(size, x,   y+1, tile, sector);
}

// Return true iff object with a given size can reside on (x1, y1)
// and move from there to (x2, y2) while staying at the same tile
// type without colliding.
//
// This should execute FASTER than calling findShortestPath().
bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const
{
  // Start location is on the map
  assert(x1 >= 0 && x1+size < width);
  assert(y1 >= 0 && y1+size < height);

  // End location is on the map
  assert(x2 >= 0 && x2+size < width);
  assert(y2 >= 0 && y2+size < height);

  // Points are connected if they are accessible
  // and belong to the same sector.
  TouchSector(size, x1, y1);
  const int sector1(sectors[size][y1*width+x1]);
  const int sector2(sectors[size][y2*width+x2]);
  return sector1 == sector2 && sector1 != 0;
}

// Data structures used for searching
struct Node
{
  typedef Grid::Direction Action;

  Point	state;	// Current state
  Node *	parent; // Parent node
  Action	action;	// Action taken to reach state
  int		gCost;	// Cost to reach state
  int		fCost;	// Total heuristic cost of state

  Node(const Point & start, const Point & goal) :
    state(start), parent(0), gCost(0),
    fCost(estimateCost(start,goal)) {}

  Node(Node * parent, Action action, const Point & goal) :
    state(parent->state.move(action)), parent(parent),
    action(action),	gCost(parent->gCost+getMoveCost(action)),
    fCost(gCost+estimateCost(state,goal)) {}
};

struct LeastCost
{
  bool operator()(const Node * a, const Node * b) const
  {
    if(a->fCost > b->fCost) return true;
    if(a->fCost < b->fCost) return false;
    return a->gCost < b->gCost;
  }
};

typedef std::priority_queue<Node*,std::vector<Node*>,LeastCost>	OpenList;
typedef std::vector<Node*> ClosedList;

// Compute a shortest path from (x1, y1) to (x2, y2) for an object with a
// given size. Return the cost of the shortest path or -1 if there is no
// path
int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2,
                           std::vector<Direction> & path) const
{
  // Start location is on the map
  assert(x1 >= 0 && x1+size < width);
  assert(y1 >= 0 && y1+size < height);

  // Start location is of a consistent tile type
  const Tile tile(getTile(x1,y1));
  for (int y(y1); y <= y1+size; ++y) {
    for (int x(x1); x <= x1+size; ++x) {
      assert(getTile(x,y) == tile);
    }
  }

  // End location is on the map
  const Point goal(x2,y2);
  assert(x2 >= 0 && x2+size < width);
  assert(y2 >= 0 && y2+size < height);

  // Initialise search
  int		cost(-1);
  OpenList	open;
  ClosedList	closed(width*height,(Node*)0);

  // heap allocation OK for this assignment, but too slow for high-performance
  // pathfinding
  open.push(new Node(Point(x1,y1), goal));

  // While we have more nodes on the open list
  while (!open.empty()) {

    // Get information about the current node
    Node *		curNode(open.top()); open.pop();
    const Point &	curState(curNode->state);
    const int	curIndex(curState.y*width+curState.x);

    // If state was previously closed
    if (closed[curIndex]) {
      // Free node memory and skip
      delete curNode;
      continue;
    }

    // Close the state corresponding to this node
    closed[curIndex] = curNode;

    // If we have reached the goal state
    if (curState == goal) {

      // Store the final cost
      cost = curNode->gCost;

      // Retrieve the directions for the path
      std::vector<Direction> revPath;
      while (curNode->parent) {
        revPath.push_back(curNode->action);
        curNode = curNode->parent;
      }
      path.assign(revPath.rbegin(), revPath.rend());

      // Stop searching
      break;
    }

    // Enqueue neighboring states
    for (int i(0); i < 8; ++i) {

      // Determine the next state
      const Direction direction(static_cast<Direction>(i));
      const Point	    nextState(curState.move(direction));

      // Skip moves that move us off the edge of the map
      const int moveX0(std::min(curState.x,nextState.x));
      const int moveY0(std::min(curState.y,nextState.y));
      const int moveX1(std::max(curState.x,nextState.x)+size);
      const int moveY1(std::max(curState.y,nextState.y)+size);
      if (moveX0 < 0 || moveX1 >= width || moveY0 < 0 || moveY1 >= height) {
        continue;
      }

      // Skip moves that move us to previously closed states
      if (closed[nextState.y*width+nextState.x]) {
        continue;
      }

      // Skip moves that are obstructed by terrain
      if (!isConnected(size,curState.x,curState.y,
                       nextState.x,nextState.y) ||
          !isConnected(size,curState.x,curState.y,
                       nextState.x,curState.y) ||
          !isConnected(size,curState.x,curState.y,
                       curState.x,nextState.y)) {
        continue;
      }

      // Enqueue next node
      open.push(new Node(curNode, direction, goal));
    }
  }

  // Free remaining nodes on open list
  while(!open.empty()) {
    delete open.top();
    open.pop();
  }

  // Free nodes on closed list
  for(ClosedList::iterator it(closed.begin()), end(closed.end());
      it!=end;
      ++it) {
    delete *it;
  }

  // Return the final cost of the path (or -1 if we didn't find the goal)
  return cost;
}
