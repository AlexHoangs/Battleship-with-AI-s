//
// Created by ALEX on 5/9/2021.
//

#include <iostream>
#include <string>
#include "Board.h"
#include "Player.h"

namespace BattleShip
{
    // remove all of our ships on the board
	// and free the memory of both ships and board
    Board::~Board()
    {
        while (!ships.empty())
        {
            delete ships.back();
            ships.pop_back();
        }

        delete[] board;
    }

    // create constuctor with initialization list
	// and allocate a new 2d array[rows][cols] for our board
    Board::Board(int rows, int cols, int shipCount) : rows(rows), cols(cols), shipCount(shipCount) {
        board = new Ship*[rows * cols]();
    }

    // get our board size and config stuff
    int Board::getCols() const { return cols; }
    int Board::getRows() const { return rows; }
    int Board::getShipCount() const { return shipCount; }

    // pointer-based 2d arrays are ugly so we're making use of
	// one array with column offsets to make it 2d
	// x (row) * col skips the rows we don't want in our array
	// then y selects an element from that column
    Ship* Board::get(int x, int y) const { return board[x * cols + y]; }
    Ship* Board::set(int x, int y, Ship* ship) {
        Ship* old = get(x, y);
        board[x * cols + y] = ship;
        return old;
    }
    Ship* Board::get(std::pair<int, int> location) const { return get(location.first, location.second); }
    Ship* Board::set(std::pair<int, int> location, Ship* ship) { return set(location.first, location.second, ship); }
		
	// this function does not validate our position!!!!!!!
    // create new ship and add to the board depending on rotation and length
	// return a pointer to it for future use
    Ship* Board::addShip(int x, int y, ShipType t, Rotation r) {
        Ship* ship = new Ship(t, x, y, r);
		
		// set along a row or column based on
		// our rotation
        for (int i = 0; i < t.getLength(); i++) {
            if (r == Rotation::HORIZONTAL)
                set(x, y + i, ship);
            
            if (r == Rotation::VERTICAL)
                set(x + i, y, ship);
        }
		
		// keep track of ship for easy freeing later
        ships.push_back(ship);

        return ship;
    }

    //check to see if a position is within the bounds of the board
    bool Board::isInBounds(int x, int y) const {
        return x >= 0 && y >= 0 && y < cols && x < rows;
    }

    // check too see if player or opponent has lost yet based on
	// if all of their ships are dead
    bool Board::checkLoseCondition() const {
        return std::find_if(ships.begin(), ships.end(), [] (Ship* s) { return s->isAlive(); }) == ships.end();
    }

    // check to see if a ship of type t and rotation r fits on the board
	// and does not conflict with other ships
    bool Board::isShipPlacable(int x, int y, ShipType t, Rotation r) const {
        for (int i = 0; i < t.getLength(); i++) {
			// check along the row our ship is on
            if (r == Rotation::HORIZONTAL) {
				// not in bounds or a ship already exists at position, say no!
                if (!isInBounds(x, y + i) || get(x, y + i) != nullptr)
                    return false;
            }
			
			// check along the column our ship is on
            if (r == Rotation::VERTICAL) {
                if (!isInBounds(x + i, y) || get(x + i, y) != nullptr)
                    return false;
            }
        }

        return true;
    }
	
	// print our board with no special parameters
    void Board::print() const { print(nullptr, true); }
	
	// print our board for an opponent based on their hits list
	// or print one for ourself
    void Board::print(const Player* opponent, bool placement) const {
		
		// blank space on top right
        std::cout << " ";
		
		// column numbers
        for (int i = 0; i < cols; i++) {
            std::cout << " " << i;
        }
		
		// the correct output has a space
		// at the end for some reason...
        std::cout << " " << std::endl;

        for (int i = 0; i < rows; i++) {

            std::cout << i << " ";

            for (int j = 0; j < cols; j++) {
				// get ship at i, j
                Ship* s = get(i, j);
				
				// if it exists, we print something for it
                if (s != nullptr) {
					
					// if it's hit at i,j the X is unconditionally displayed
                    if (s->isHit(i,j))
                        std::cout << "X ";
                    else {
						// in placement mode, our ship is visible
						// if not, it's not
                        if (placement)
                            std::cout << s->getType().getCharacter() << " ";
                        else
                            std::cout << "* ";
                    }
                }
                else {
					// if we have an opponent, we check if they shot at i, j
					// and print a circle if they did.
                    if (opponent != nullptr && opponent->shotExists(i, j)) {
                        std::cout << "O ";
                    }
                    else {
                        std::cout << "* ";
                    }
                }
            }

            std::cout << std::endl;
        }
    }
}