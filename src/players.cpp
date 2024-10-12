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

class White : public Player
{
    public:
    White()
    {
        name = "Whiteboy";
    }
    decision make_decision(GameState gameState) override
    {
        if (unbanked_money + banked_money > 99)
        {
            return decision::bank;
        }
        else if (unbanked_money > 20.9 && banked_money < 55)
        {
            return decision::bank;
        }
        else
        {
            return decision::hit;
        }
    }
};
