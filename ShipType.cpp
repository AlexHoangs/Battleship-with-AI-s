//
// Created by ALEX on 5/9/2021.
//

#include "ShipType.h"

namespace BattleShip {
	
	// getters
    char ShipType::getCharacter() const { return character; }
    int ShipType::getLength() const { return length; }

	// read our ship from a stream
	// like our config file stream
    std::istream& operator>>(std::istream& in, ShipType& rhs) {
        in >> rhs.character;
        in >> rhs.length;

        return in;
    }
}
