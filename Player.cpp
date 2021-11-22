//
// Created by ALEX on 5/9/2021.
//

#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include "Player.h"

namespace BattleShip
{	
	// getters
    std::string Player::getName() const { return name; }

    Board* Player::getBoard() { return &board; }

    const Board* Player::getBoard() const { return &board; }
	
	// alias for the other function not involving a pair
    bool Player::addShot(int x, int y, bool hit) {
        return addShot(std::pair<int, int>(x, y), hit); 
    }
	
	// add a shot for our player's shots.
    bool Player::addShot(std::pair<int, int> p, bool hit) {
        return shots.insert(std::pair<std::pair<int, int>, bool>(p, hit)).second; 
    }
	
	// alias for the other function not involving a pair
    bool Player::shotExists(int x, int y) const { return shotExists(std::pair<int, int>(x, y)); }
	
	// check if a shot exists at x, y
    bool Player::shotExists(std::pair<int, int> p) const { return shots.find(p) != shots.end(); }

    std::string HumanPlayer::getNameInput(int id) {
        std::string name;
        std::cout << "Player " << id << " please enter your name: ";
        std::getline(std::cin, name);

        return name;
    }
    // get shot input from player
    std::pair<int, int> HumanPlayer::promptShot(Player* opponent) {
        std::string in;
        int x, y;
        // for some reason we still print the board when our 
        // input is a valid number but out of bounds but don'table
        // when it's something like "dog"?????
promptfireoutofbounds:
        // Goto because it's easier and simpler than a while loop
        std::cout << this->getName() << "'s Firing Board" << std::endl;
        opponent->getBoard()->print(this, false);
        std::cout << "\n\n";
        std::cout << this->getName() << "'s Placement Board" << std::endl;
        this->getBoard()->print(opponent, true);
promptfire:
        std::cout << this->getName() << ", where would you like to fire?" << std::endl;
        std::cout << "Enter your attack coordinate in the form row col: ";

        std::getline(std::cin, in);
        std::stringstream str(in);

        // we validate that the input can be read,
        // and we did not already fire a shot there.
        if (!(str >> x) || !(str >> y) || (str >> in))
            goto promptfire;
        
        // if out of bounds or shot exists, we reprint the board for some reason
        if (!opponent->getBoard()->isInBounds(x, y) || this->shotExists(x, y))
            goto promptfireoutofbounds;
        
        return std::pair<int, int>(x, y);
    }

    // prompt human to place ship
    void HumanPlayer::placeShips(std::vector<ShipType> types) {
        board.print();

        for (ShipType t : types) {
            // We read the entire line  into a
            // string stream. This way, we do not leave
            // unused characters in the input buffer.
            std::stringstream str;
            std::string in;
            Rotation r;
            char rc;
            int x, y;

            // We are using a goto here, because it's cleaner and easier than a while loop.
beginprompt:
            std::cout << this->getName() << ", do you want to place " << t.getCharacter() << " horizontally or vertically?" << std::endl;
            std::cout << "Enter h for horizontal or v for vertical\nYour choice: ";
            std::getline(std::cin, in);
            str = std::stringstream(in);
			
			// grab rotation, then check for
			// extraneous input 
            if (!(str >> rc) || (str >> in))
                goto beginprompt;

            // check if input is valid when asking for direction
            // if not, go to beginning of prompt
            if (rc == 'h' || rc == 'H') {
                r = Rotation::HORIZONTAL;
            }
            else if (rc == 'v' || rc == 'V') {
                r = Rotation::VERTICAL;
            }
            else {
                goto beginprompt;
            }

            std::cout << this->getName() << ", enter the row and column you want to place " << t.getCharacter() << ", which is " << t.getLength() <<
                      " long, at with a space in between row and col: ";

            std::getline(std::cin, in);
            str = std::stringstream(in);

            // see if we have valid numbers in the input buffer
			// then check the buffer for extraneous input
            // check to see if the ship is placeable. if not go to beginning of prompt.
            if (!(str >> x) || !(str >> y) || (str >> in) || !board.isShipPlacable(x, y, t, r))
                goto beginprompt;

            // if it passes all the test cases, add the ship
            board.addShip(x, y, t, r);
            board.print();
        }
    }

    // place ships per the spec
    void AIPlayer::placeShips(std::vector<ShipType> types) {
        Rotation r;
        int x, y;
        for (ShipType t : types) {
            do {
                r = random(0, 1) == 0 ? Rotation::HORIZONTAL : Rotation::VERTICAL;
                x = r == Rotation::HORIZONTAL ? random(0, board.getRows() - 1) : random(0, board.getRows() - t.getLength());
                y = r == Rotation::HORIZONTAL ? random(0, board.getCols() - t.getLength()) : random(0, board.getCols() - 1);
            } while (!board.isShipPlacable(x, y, t, r));

            board.addShip(x, y, t, r);

            std::cout << getName() << "'s Board" << std::endl;
            board.print();
            std::cout << std::endl;
        }
    }

    std::pair<int, int> CheatingAI::promptShot(Player* opponent) {
        for (int i = 0; i < opponent->getBoard()->getRows(); i++) {
            for (int j = 0; j < opponent->getBoard()->getCols(); j++) {
                Ship* s = opponent->getBoard()->get(i, j);
                if (s != nullptr && !s->isHit(i, j))
                    return std::pair<int, int>(i, j);
            }
        }

        return std::pair<int, int>(-1, -1);
    }

    std::pair<int, int> RandomAI::popRandomLocation() {
        int index = random(0, locations.size() - 1);
        std::pair<int, int> ret = locations[index];
        locations.erase(locations.begin() + index);
        return ret;
    }

    std::pair<int, int> RandomAI::promptShot(Player* opponent) {
        return popRandomLocation();
    }

    bool SearchAndDestroyAI::locationsContains(std::pair<int, int> location) const {
        return std::find(locations.begin(), locations.end(), location) != locations.end();
    }

    void SearchAndDestroyAI::enqueueLocation(std::pair<int, int> location) {
        if (!locationsContains(location))
            return;

        locations.erase(std::find(locations.begin(), locations.end(), location));
        firingLocations.push(location);
    }

    std::pair<int, int> SearchAndDestroyAI::promptShot(Player* opponent) {
        std::pair<int, int> location;

        if (!firingLocations.empty()) {
            location = firingLocations.front();
            firingLocations.pop();
        } else {
            location = popRandomLocation();
        }

        if (opponent->getBoard()->get(location) != nullptr) {
            enqueueLocation(std::pair<int, int>(location.first, location.second - 1));
            enqueueLocation(std::pair<int, int>(location.first - 1, location.second));
            enqueueLocation(std::pair<int, int>(location.first, location.second + 1));
            enqueueLocation(std::pair<int, int>(location.first + 1, location.second));
        }

        return location;
    }
};
