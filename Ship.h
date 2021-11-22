//
// Created by ALEX on 5/9/2021.
//

#ifndef HW3_BATTLESHIP_SHIP_H
#define HW3_BATTLESHIP_SHIP_H

#include "ShipType.h"
#include "Rotation.h"

namespace BattleShip {

    class Ship {

    private:
		// our ship's type
        ShipType type;
		// leftmost x position
        int posX;
		// topmost y positon
        int posY;
		// rotation of vertical or horizontal
		// if our ship is vertical, it spans a column
		// if horizontal, it spans a row
        Rotation rotation;
		
		// array of cells where our ship is hit
		// a method is provided to translate between x,y
		// and hit indexes.
        bool* hit;

    public:
        Ship(ShipType type, int posX, int posY, Rotation rotation);
        ~Ship();
        ShipType getType() const;
        int getX() const;
        int getY() const;
        Rotation getRotation() const;
		int findShipHitIndex(int x, int y) const;
        bool processHit(int x, int y);
        bool isHit(int x, int y) const;
        bool isAlive() const;
    };
}


#endif //HW3_BATTLESHIP_SHIP_H
