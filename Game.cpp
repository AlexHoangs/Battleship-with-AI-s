//
// Created by ALEX on 5/9/2021.
//

#include "Game.h"
#include "Player.h"


namespace BattleShip
{
    void Game::play(std::string configPath)
    {
        //in file stream to get the configuration data
        std::ifstream config(configPath);
        //data form config file
        // we read rows, cols, shipcount as per the spec
        config >> boardSizeX >> boardSizeY >> boardShipCount;

        ShipType t;

        //transfer data from configuration file to t and push it back into a vector
        // we have a custom >> operator defined for ShipType
        while (config >> t)
            types.push_back(t);

        //sort the vector from the beginning to end in ascii value order
        std::sort(types.begin(), types.end(), [] (ShipType a, ShipType b) { return a.getCharacter() < b.getCharacter(); });
        
        GameType gameType;
        unsigned choice;

        std::cout << "What type of game do you want to play?\n1. Human vs Human\n2. Human vs AI\n3. AI vs AI\nYour choice: ";
        std::cin >> choice;
        gameType = static_cast<GameType>(choice);

        std::string dummy;
        std::getline(std::cin, dummy);
        
        // initialize 2 players and place them
        // in indexes 0 and 1 for easy access.
        players[0] = initializePlayer(1, gameType);
        players[1] = initializePlayer(2, gameType);


        // keep track of currentPlayerIdx
        // which keeps track of which player's turn
        // it is
        int currentPlayerIdx = 0;
        while (true)
        {
            std::pair<int, int> location;

            Player* player = players[currentPlayerIdx];
            Player* opponent = players[getOpponentIndex(currentPlayerIdx)];

            // if no ships are alive, the player loses and we exit.
            if (player->getBoard()->checkLoseCondition())
            {
                std::cout << opponent->getName() << " won the game!" << std::endl;
                break;
            }

            location = player->promptShot(opponent);
			
            bool hit = false;
            // retrieve the ship on the opponent's board at x, y
            Ship* ship = opponent->getBoard()->get(location.first, location.second);

            // if the ship exists (not null), we process a hit on it
            // which will change the ship's internal state.
            // at this point we are already guaranteed to hit the ship
            // but processShip returns a sanity check for us
            if (ship != nullptr)
                hit = ship->processHit(location.first, location.second);

            // we add our new coordinate pair to our
            // map of shots taken
            player->addShot(location.first, location.second, hit);

            // print board after action
            std::cout << player->getName() << "'s Firing Board" << std::endl;
            opponent->getBoard()->print(player, false);
            std::cout << "\n\n";
            std::cout << player->getName() << "'s Placement Board" << std::endl;
            player->getBoard()->print(opponent, true);

            // if we hit, we say we hit
            if (hit)
            {
                std::cout << player->getName() << " hit " << opponent->getName() << "'s " << ship->getType().getCharacter() << "!" << std::endl;
                // if we killed the ship, we say we did
                if (!ship->isAlive())
                    std::cout << player->getName() << " destroyed " << opponent->getName() << "'s " << ship->getType().getCharacter() << "!" << std::endl;
                std::cout << std::endl;
            }
            else
                std::cout << "Missed." << std::endl << std::endl;

            // flip the player for the turn.
            currentPlayerIdx = getOpponentIndex(currentPlayerIdx);
        }

        delete players[0];
        delete players[1];
    }

    // idx ^ 1 gives you 0 if it's 1 and 1 if it's 0
    // could've used a ternary or something here but
    // we are 1337 hackermen
    int Game::getOpponentIndex(int idx) const { return idx ^ 1; }

    Player* Game::createAI(int num, AIChoice ai) {
        switch (ai) {
            case AIChoice::CHEATING_AI:
                return new CheatingAI(num, boardSizeX, boardSizeY, boardShipCount);
            case AIChoice::HUNT_DESTROY_AI:
                return new SearchAndDestroyAI(num, boardSizeX, boardSizeY, boardShipCount);
            case AIChoice::RANDOM_AI:
                return new RandomAI(num, boardSizeX, boardSizeY, boardShipCount);
            default:
                return nullptr;
        }
    }

    Player* Game::initializePlayer(int num, GameType type)
    {
        unsigned choice;
        Player* player;

        if (type == GameType::HUMAN_VS_HUMAN) {
            player = new HumanPlayer(HumanPlayer::getNameInput(num), boardSizeX, boardSizeY, boardShipCount);
        } else if (type == GameType::AI_VS_AI) {
            std::cout << "What AI do you want?\n1. Cheating AI\n2. Random AI\n3. Hunt Destroy AI\nYour choice: ";
            std::cin >> choice;
            player = createAI(num, static_cast<AIChoice>(choice));
        } else if (type == GameType::HUMAN_VS_AI && num == 1) {
            player = new HumanPlayer(HumanPlayer::getNameInput(1), boardSizeX, boardSizeY, boardShipCount);
        } else if (type == GameType::HUMAN_VS_AI && num == 2) {
            std::cout << "What AI do you want?\n1. Cheating AI\n2. Random AI\n3. Hunt Destroy AI\nYour choice: ";
            std::cin >> choice;
            player = createAI(1, static_cast<AIChoice>(choice));
        }

        player->placeShips(types);

        return player;
    }
}

