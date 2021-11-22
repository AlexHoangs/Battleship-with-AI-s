//
// Created by ALEX on 5/9/2021.
//

#include "Ship.h"

namespace BattleShip
{
	// delete our dynamically allocated
	// array of hits
    Ship::~Ship() {
        delete[] hit;
    }
	
    Ship::Ship(ShipType type, int posX, int posY, Rotation rotation) : type(type), posX(posX), posY(posY), rotation(rotation) {
		// allocate a new hit array to represent
		// cells of our ship that are hit
        hit = new bool[type.getLength()]();
    }

    // getters
    ShipType Ship::getType() const { return type; }
    int Ship::getX() const { return posX; }
    int Ship::getY() const { return posY; }
    Rotation Ship::getRotation() const { return rotation; }
	
	// resolve an x,y coordinate to an index in our hit
	// array.
	// if we are unable to resolve it to an index,
	// we return -1.
	int Ship::findShipHitIndex(int x, int y) const {
		int hitIdx = -1;
		
		// if vertical, then we check if the column matches
		// since the ship only spans one column.
		// then we get an offset of x from our ship's starting
		// x position, posX by subtracting posX from it.
		// if x is higher than posX (it should be)
		// and the resulting value is shorter than the ship'same
		// length, then it will resolve to a hit array index.
		if (rotation == Rotation::VERTICAL) {
            if (y != posY)
                return -1;

            hitIdx = x - posX;
        }
		// same as above but for y and rows
        else if (rotation == Rotation::HORIZONTAL) {
            if (x != posX)
                return -1;

            hitIdx = y - posY;
        }
		
		// if our offset doesn't land on our ship
		// or falls past our ship, our hit index
		// doesn't exist.
		if (hitIdx < 0 || hitIdx >= type.getLength())
			return -1;
		
        return hitIdx;
	}
	
	// grab our hit array index and then
	// mark our ship as hit at that location
    bool Ship::processHit(int x, int y) {
		int idx = findShipHitIndex(x, y);
		if (idx != -1) {
			// if we're already hit there, return false
			if (hit[idx])
				return false;
			// set hit to true at that location
			hit[idx] = true;
			return true;
		}
		
		return false;
    }
	
	// grab our hit array index and then
	// check if we're hit
    bool Ship::isHit(int x, int y) const {
		int idx = findShipHitIndex(x, y);
		
		if (idx != -1)
			return hit[idx];
		
		return false;
    }
	
	// ship is alive if there is at least
	// one cell that isn't hit
    bool Ship::isAlive() const {
		// iterate our cells
        for (int i = 0; i < type.getLength(); i++)
			// if there is a cell not hit, our ship is alive
            if (!hit[i])
                return true;
        // if all cells are hit, we're dead.
        return false;
    }
}