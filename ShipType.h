//
// Created by ALEX on 5/9/2021.
//

#ifndef HW3_BATTLESHIP_SHIPTYPE_H
#define HW3_BATTLESHIP_SHIPTYPE_H

#include <iostream>

namespace BattleShip {

    //ship type holds the name of the ship and the length
    class ShipType {

    private:
        char character;
        int length;

    public:
        char getCharacter() const;
        int getLength() const;
        // friend qualifier allows access to private members of a class
		// we want to allow our stream operator to read into the private
		// variables
        friend std::istream& operator>>(std::istream& in, ShipType& rhs);
    };
}

#endif //HW3_BATTLESHIP_SHIPTYPE_H

