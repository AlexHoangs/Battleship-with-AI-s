//
// Created by ALEX on 5/9/2021.
//

#ifndef HW3_BATTLESHIP_PLAYER_H
#define HW3_BATTLESHIP_PLAYER_H

#include <map>
#include <utility>
#include <queue>
#include "Board.h"
#include "Random.h"

namespace BattleShip {
    class Player {
    protected:
        std::string name;
        Board board;
        std::map<std::pair<int, int>, bool> shots;
    public:
        virtual ~Player() {}
        Player(std::string name, int boardX, int boardY, int shipCount) : name(name), board(boardX, boardY, shipCount) {};
        std::string getName() const;
        Board* getBoard();
        const Board* getBoard() const;
        bool addShot(int x, int y, bool hit);
        bool addShot(std::pair<int, int> p, bool hit);
        bool shotExists(int x, int y) const;
        bool shotExists(std::pair<int, int> p) const;
        virtual std::pair<int, int> promptShot(Player* opponent) = 0;
        virtual void placeShips(std::vector<ShipType> types) = 0;
    };

    class HumanPlayer : public Player {
    public:
        static std::string getNameInput(int id);

        HumanPlayer(std::string name, int boardX, int boardY, int shipCount) : Player(name, boardX, boardY, shipCount) {}
        virtual std::pair<int, int> promptShot(Player* opponent) override;
        virtual void placeShips(std::vector<ShipType> types) override;
    };

    class AIPlayer : public Player {
    public:
        AIPlayer(int id, int boardX, int boardY, int shipCount) : Player(std::string("AI ") + std::to_string(id), boardX, boardY, shipCount) {}
        virtual void placeShips(std::vector<ShipType> types) override;
    };

    class CheatingAI : public AIPlayer {
    public:
        CheatingAI(int id, int boardX, int boardY, int shipCount) : AIPlayer(id, boardX, boardY, shipCount) {}
        virtual std::pair<int, int> promptShot(Player* opponent) override;
    };

    class RandomAI : public AIPlayer {
    protected:
        std::vector<std::pair<int, int>> locations;
        std::pair<int, int> popRandomLocation();
    public:
        RandomAI(int id, int boardX, int boardY, int shipCount) : AIPlayer(id, boardX, boardY, shipCount) {
            for (int i = 0; i < boardX; i++)
                for (int j = 0; j < boardY; j++)
                    locations.push_back(std::pair<int, int>(i, j));
        }
        virtual std::pair<int, int> promptShot(Player* opponent) override;
    };

    class SearchAndDestroyAI : public RandomAI {
    private:
        std::queue<std::pair<int, int>> firingLocations;
    public:
        SearchAndDestroyAI(int id, int boardX, int boardY, int shipCount) : RandomAI(id, boardX, boardY, shipCount) {}
        bool locationsContains(std::pair<int, int> location) const;
        void enqueueLocation(std::pair<int, int> location);
        virtual std::pair<int, int> promptShot(Player* opponent) override;
    };
}
#endif //HW3_BATTLESHIP_PLAYER_H
