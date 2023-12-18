#include <iostream>
#include "Player.h"
#include "Game.h"

int main()
{
    std::list<Player*> players;
    for (int i = 0; i < 456; ++i)
    {
        players.push_back(new Player(i + 1));
    }

    std::vector<Game*> games;
    games.push_back(new RedLightGreenLight(20));
    games.push_back(new RPS());
    games.push_back(new MarbleGame());
    // games.push_back(new SugarHoneycombToffee());
    // games.push_back(new RedLightGreenLight(10));

    for (auto game : games)
    {
        if (game->getGameName() == "Marble Game")
        {
            if (game->getGameType() == 0) // 게임 선택 매커니즘을 만들었음
            {
                delete game;
                game = new OddOrEven();
                for (auto player : players)
                {
                    game->join(player);
                }
                game->play();
            }
            else
            {
                delete game;
                game = new Hole_In_One();
                for (auto player : players)
                {
                    game->join(player);
                }
                game->play();
                break;
            }

            auto alivePlayers = game->getAlivePlayers();
            players.clear();
            for (auto player : alivePlayers)
            {
                players.push_back(new Player(*player));
            }


        }
        else
        {
            for (auto player : players)
                game->join(player);

            game->play();

            auto alivePlayers = game->getAlivePlayers();
            players.clear();
            for (auto player : alivePlayers)
            {
                players.push_back(new Player(*player));
            }
        }

    }

    for (auto game : games)
    {

        delete game;
    }

}