//
// Created by ALEX on 5/9/2021.
//

#include "Game.h"
#include "Random.h"

int main(int argc, char** argv)
{
    std::string c(argv[1]);

    if (argc > 2)
        BattleShip::initializeRandom(std::stoul(argv[2]));
    else
        BattleShip::initializeRandom();
        
    BattleShip::Game game;
    game.play(c);
}