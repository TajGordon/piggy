#include "header.hpp"
#include <string>

class Bailer : public Player
{
    int limit;
    public:
    Bailer(int new_limit, std::string new_name)
    {
        limit = new_limit;
        name = new_name;
    }

    decision make_decision(GameState gameState) override
    {
        return (banked_money + unbanked_money > 99)
        ?  decision::bank
        : (decision)(unbanked_money > limit);
    }
};

class NRoller : public Player
{
    int n;
    public:
    NRoller(int input)
    {
        n = input;
        char buffer[32];
        sprintf(buffer, "%dth Roller", n);
        name = buffer;
    }
    decision make_decision(GameState gameState) override
    {
        if (gameState.roll_no == n)
        {
            return decision::bank;
        }
        else
        {
            return decision::hit;
        }
    }
};

class WhiteBoy : public Player
{
    int turning_point;
    public:
    WhiteBoy(int input)
    {
        turning_point = input;
        char buffer[32];
        sprintf(buffer, "Whiteboy #%d", turning_point);
        name = buffer;
    }
    decision make_decision(GameState gameState) override
    {
        if (unbanked_money + banked_money > 99)
        {
            return decision::bank;
        }
        else if (unbanked_money > 20.9 && banked_money < turning_point)
        {
            return decision::bank;
        }
        else
        {
            return decision::hit;
        }
    }
};
