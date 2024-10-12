#pragma once
#include <string>
#include <vector>
#include <map>

typedef enum decision
{
    hit,
    bank,
} decision;

class Game;
class Player;
struct GameState;

struct GameState
{
    int round_no;
    int roll_no;
    std::vector<Player*> players_banked_this_round;
    std::vector<Player*> unbanked_players;
    std::map<Player*, int> banked_money;
    std::map<Player*, int> unbanked_money;
};

struct SameGameMulti
{
    std::vector<GameState> gameStates;
};

class Player
{
    public:
    std::string name;
    int banked_money;
    int unbanked_money;
    int score; /* Total score achieved */
    int wins;

    virtual decision make_decision(GameState gameState) = 0; // a copy of the game state, should it be a const reference?
    virtual ~Player() = default; /* Gotta have a destructor I guess... And it's gotta say "default" too... */
};

/* Putting game as a class because we can run multiple of them */
class Game
{
    /* Players vector is sorted by ranking */
    std::vector<Player*> players;
    std::vector<Player*> winners;
    GameState gameState;
    SameGameMulti sameGameMulti; /* Separated out like this in case I need more data, stores a vector<GameState> */

    void UpdateRankings();
    void BankPlayer(Player* player);
    void ResetForNextRound();
    void ResetForNextGame();
    bool player_has_won();

    public:
    void AddPlayer(Player* player);
    void Run(); // run 1 game
    void RunMulti(int gameCount); /* run multiple games (purposely ignored for now) concurrently */
};

/* Non class specific functions! */
inline int roll_dice() // basically just a define at this point...
{
    return rand() % 6 + 1;
}
