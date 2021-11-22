//
// Created by ALEX on 5/9/2021.
//

#ifndef HW3_BATTLESHIP_GAME_H
#define HW3_BATTLESHIP_GAME_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "ShipType.h"
#include "Player.h"
#include "AIChoice.h"
#include "GameType.h"

namespace BattleShip {

    class Game {

    private:
        // vector of ship name and length
        std::vector<ShipType> types;
        // board dimensions, ship count, and array of players
		// for our convenince in accessing
        int boardSizeX;
        int boardSizeY;
        int boardShipCount;
        Player* players[2];

    public:
        int getOpponentIndex(int idx) const;
        Player* initializePlayer(int num, GameType t);
        Player* createAI(int num, AIChoice ai);
        void play(std::string configPath);
    };
}

#endif //HW3_BATTLESHIP_GAME_H
