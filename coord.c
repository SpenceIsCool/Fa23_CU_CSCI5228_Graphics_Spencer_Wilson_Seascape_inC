#include "SDW.h"

/**
 * an xyz tuple style object
 */

struct Coord newCoord(double x, double y, double z)
{
    struct Coord res;
    res.x = x;
    res.y = y;
    res.z = z;
    return res;
}


void printCoord(struct Coord coord)
{
    printStuff("\tx: %lf, y: %lf, z: %lf\n", coord.x, coord.y, coord.z);
}


void plotCoord(struct Coord coord)
{
     glVertex3f(coord.x, coord.y, coord.z);
}
