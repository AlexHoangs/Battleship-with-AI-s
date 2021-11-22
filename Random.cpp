#include "Random.h"

namespace BattleShip
{
    static bool initialized = false;
    static std::mt19937 rng;

    void initializeRandom()
    {
        if (!initialized)
        {
            rng = std::mt19937();
            initialized = true;
        }
    }

    void initializeRandom(unsigned seed)
    {
        if (!initialized)
        {
            rng = std::mt19937(seed);
            initialized = true;
        }
    }

    int random(int min, int max)
    {
        if (!initialized)
            throw std::exception();
        
        return std::uniform_int_distribution<int>(min, max)(rng);
    }
}