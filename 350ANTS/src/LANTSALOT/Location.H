//TODO Remove this and place asserts in all relevant functions

/* Location.H
 * 		Authors: Eric Klinger, Jake Brand
 *
 * This was borrowed and modified from the c++ starter package
 *
 * This class holds the points x and y to represent a location on the grid
 * THe Location struct is used to represent a location inside the grid,
 * it simply contains a refer to a row and column
 */

#ifndef LOCATION_H_
#define LOCATION_H_

struct Location
{
    int row, col;

    Location()
    {
        row = col = 0;
    };

    Location(int r, int c)
    {
        row = r;
        col = c;
    };
};

#endif //LOCATION_H_
