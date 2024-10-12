#include "header.hpp"
#include "players.cpp"
#include "game.cpp"
#include <ctime>
#include <string>

int main(int argc, char* argv[])
{
    int times_to_run = 1000;
    if (argc > 1 && std::stoi(argv[1]) > 0)
    { times_to_run = std::stoi(argv[1]); }
    srand(time(0));
    Game game;
    game.AddPlayer(new Bailer(15, "Sanjin's lover"));
    game.AddPlayer(new Bailer(21, "21 Jump Street"));
    game.AddPlayer(new Bailer(22, "22 Jump Street"));
    game.AddPlayer(new NRoller(4));
    game.AddPlayer(new NRoller(5));
    game.AddPlayer(new NRoller(6));
    game.AddPlayer(new White());
    game.RunMulti(times_to_run);
}
