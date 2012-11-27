//TODO Remove this and place asserts in all relevant functions

/* MyGrid.H
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This is the header file for the Grid class. Working with A* to get it
 * working for our implementation
 */

#include <vector>

class Grid
{
public:

  enum Tile { GROUND, WATER, BLOCKED };

  enum Direction { N, NE, E, SE, S, SW, W, NW };

  static const int CARDINAL_COST = 100; // Cost to move in a compass direction
  static const int DIAGONAL_COST = 141; // Cost to move diagonally

private:

  std::vector<Tile> data;
  int width;
  int height;

  // can be changed by const functions
  mutable std::vector<int> sectors[3];
  mutable int nextSector[3];
  mutable bool sectorsDirty;

  void ResetSectors() const;
  void TouchSector(int size, int x, int y) const;
  void FloodFill(int size, int x, int y, int tile, int sector) const;

public:

  Grid(int width, int height);
  ~Grid() {}

  // Map properties
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  Tile getTile(int x, int y) const;

  // Pathfinding operations

  // Return true iff object with a given size can reside on (x1, y1)
  // and move from there to (x2, y2) while staying at the same tile
  // type without colliding.
  //
  // This should execute FASTER than calling findShortestPath().
  //
  // Bonus: iff the map hasn't changed, subsequent calls with the same
  // x1,y1,x2,y2 coordinates SHOULD BE MUCH FASTER
  bool isConnected(int size, int x1, int y1, int x2, int y2) const;

  // Compute a shortest path from (x1, y1) to (x2, y2) for an object
  // with a given size. Store the shortest path into path, and return
  // the cost of that path (using CARDINAL_COST for the moves N, E, S,
  // and W, and DIAGONAL_COST for the moves NE, SE, SW, and NW) or -1
  // if there is no path.
  int findShortestPath(int size, int x1, int y1, int x2, int y2,
                       std::vector<Direction> & path) const;

  // Mutators
  void setTile(int x, int y, Tile tile);
};

