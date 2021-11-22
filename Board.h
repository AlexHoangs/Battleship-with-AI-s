//
// Created by ALEX on 5/9/2021.
//

#ifndef HW3_BATTLESHIP_BOARD_H
#define HW3_BATTLESHIP_BOARD_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "Ship.h"

namespace BattleShip
{
    class Player;
    class Board
    {
        int rows;
        int cols;
		// useless but we keep it here for
		// completeness
        int shipCount;
		// 2d array of board cells
		// with null if no ships are on that cell
        Ship** board;
		// vector of our ships for fast deletion
        std::vector<Ship*> ships;
    public:
        Board(int rows, int cols, int shipCount);
        int getRows() const;
        int getCols() const;
        int getShipCount() const;
        ~Board();
        void print() const;
        void print(const Player* opponent, bool placement) const;
        bool isInBounds(int x, int y) const;
        bool isShipPlacable(int x, int y, ShipType t, Rotation r) const;
        bool checkLoseCondition() const;
        Ship* addShip(int x, int y, ShipType t, Rotation r);
        Ship* get(int x, int y) const;
        Ship* get(std::pair<int, int> location) const;
        Ship* set(int x, int y, Ship* ship);
        Ship* set(std::pair< int, int> location, Ship* ship);
    };
}

#endif //HW3_BATTLESHIP_BOARD_H
