#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include "header.hpp"

/* Putting game as a class because we can run multiple of them */
/* Private: */
void Game::UpdateRankings()
{
    static auto comp = [](Player* a, Player* b) {
        return (a->banked_money + a->unbanked_money > b->banked_money + b->unbanked_money);
    };
    std::sort(players.begin(), players.end(), comp);
}
void Game::BankPlayer(Player* player)
{
    player->banked_money += player->unbanked_money;
    player->unbanked_money = 0;
    gameState.players_banked_this_round.push_back(player);
    /* Find is a pointer to the element in the vector */
    auto it = std::find(gameState.unbanked_players.begin(), gameState.unbanked_players.end(), player);
    /* The erase function wants a pointer to the item for removal */
    gameState.unbanked_players.erase(it);
}
void Game::ResetForNextRound()
{
        gameState.unbanked_players.clear();
    for (auto& player : players)
    {   player->unbanked_money = 0;
        gameState.unbanked_players.push_back(player);
    }   gameState.players_banked_this_round.clear();
}
void Game::ResetForNextGame()
{
    for (auto& player : players)
    {
        player->score += player->unbanked_money + player->banked_money;
        player->unbanked_money = 0;
        player->banked_money = 0;
    }
}
/* This function might not work properly */
/* I'm just avoiding the potential issue */
bool Game::player_has_won()
{
    for (const auto& player : players)
    {
        if (player->banked_money > 99) // if greater than 99, its 100 or more, no doubles
        return true;
    }
    return false;
}

/* Public: */
void Game::AddPlayer(Player* player)
{
    players.push_back(player);
}
void Game::Run() // run 1 game
{
    if (players.size() < 1) { std::cout << "Cannot start a game with 0 players." << std::endl; return; }

    for (const auto& player : players)
    {
        std::cout << player->name << " is playing." << std::endl;
    }

    int round_count = 0;
    while (!player_has_won()) /* run rounds */
    {
        round_count++;
        ResetForNextRound();

        bool rolled_one = false;
        while (!rolled_one && gameState.unbanked_players.size() > 0) /* Rolling the dice (asking for decision first) */
        {
            // std::cout << "pls dont show up" << gameState.unbanked_players.size() << std::endl;
            /* Ask for decision */
            for (auto& player : gameState.unbanked_players)
            {
                // std::cout << player->name << ":" << player->banked_money << ":" << player->unbanked_money << std::endl;
                if (player->make_decision(gameState) == decision::bank)
                {
                    // std::cout << "Banked " << player->unbanked_money << " coins" << std::endl;
                    BankPlayer(player);
                }
            }
            // std::cout << gameState.unbanked_players.size() << std::endl;

            /* Roll dice */
            int roll = roll_dice();
            if (roll == 1)
            {
                rolled_one = true;
                // std::cout << "if statement reached, roll = " << roll << std::endl;
            }
            else
            {
                // std::cout << gameState.unbanked_players.size() << std::endl;
                for (auto& player : gameState.unbanked_players)
                {
                    // std::cout << "Player unbanked money before: " << player->unbanked_money;
                    player->unbanked_money += roll;
                    // std::cout << ", after: " << player->unbanked_money << std::endl;
                }
            }
        }
    }
    for (const auto& player : players)
    {
        if (player->banked_money > 99)
        {
            std::cout << "Player: '" << player->name << "' collected " << player->banked_money << " coins and won!" << std::endl;
        }
    }
}
/* Runs concurrently */
void Game::RunMulti(int gameCount)
{
    if (players.size() < 1) { std::cout << "Cannot start a game with 0 players." << std::endl; }

    for (const auto& player : players)
    {
        std::cout << player->name << " is playing." << std::endl;
    }
    std::cout<<std::endl;

    int gamesRan = 0;
    while (gamesRan < gameCount)
    {
        /* Emplace_back calls constructor */
        sameGameMulti.gameStates.emplace_back();
        gameState = sameGameMulti.gameStates.back();

        ResetForNextGame();

        while (!(player_has_won()))
        {
            gameState.round_no++;
            gameState.roll_no++;
            ResetForNextRound();

            bool rolled_one = false;
            while (!rolled_one && gameState.unbanked_players.size() > 0) /* Rolling the dice (asking for decision first) */
            {
                /* Ask for decision */
                for (auto& player : gameState.unbanked_players)
                {
                    if (player->make_decision(gameState) == decision::bank)
                    {
                        BankPlayer(player);
                    }
                }

                /* Roll dice */
                int roll = roll_dice();
                if (roll == 1)
                {
                    rolled_one = true;
                }
                else
                {
                    for (auto& player : gameState.unbanked_players)
                    {
                        player->unbanked_money += roll;
                        player->times_rolled++;
                    }
                }
            }
            for (const auto& player : players)
            {
                if (player->banked_money > 99)
                {
                    player->wins++;
                    /* Fuck me this could be stored better, like with a hashmap or something */
                    if (find(winners.begin(), winners.end(), player) == winners.end()) winners.push_back(player);
                    // std::cout << "Player: '" << player->name << "' collected " << player->banked_money << " coins and won!" << std::endl;
                }
            }
        }
        gamesRan++;
    }
    std::cout<<std::endl;
    long double total_score = 0;
    for (auto& player : players)
    {
        total_score += player->score;
        if (total_score < 0) { std::cout << "OVERFLOW!" << std::endl; return; }
    }
    /* Sort the winners vector before outputting it, so that we can see the ranking */
    auto wc = [](Player* a, Player* b) {
        return (a->score > b->score);
    };
    /* Also sorting the whole player vector to show the score the bad bots are getting */
    sort(players.begin(), players.end(), wc);
    sort(winners.begin(), winners.end(), wc);
    for (auto& player : winners)
    {
        std::cout << "Player: '" << player->name << "' won " << player->wins << " times (" << 100. * (double)player->wins / (double)gameCount << "% of the time) and had a total score of " << player->score << " (" << 100. * (double)player->score / total_score << "% of the total score)." << std::endl;
    }
    std::cout << "No other player won any games." << std::endl;

    /* outputting the data to a csv, temporary? maybe */
    static auto output = [this, gameCount, total_score]() {
        char filename[32];
        sprintf(filename, "outdata/outputdata%lld.csv", time(0));
        freopen(filename, "w+", stdout);
        std::cout << "Name,Score,Average Score Per Game,Average Score Per Roll,% of Total Score,Wins, Win-rate (%)" << std::endl;
        for (auto& player : players)
        {
            std::cout << player->name << "," << player->score << "," << (double)player->score / (double)gameCount << "," << (double)player->score / (double)player->times_rolled <<  "," << 100. * (double)player->score / total_score << "," << player->wins << "," << 100. * (double)player->wins / (double)gameCount << std::endl;
        }
        fclose(stdout);
    };
    output();
}
