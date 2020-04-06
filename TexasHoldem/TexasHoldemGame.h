/**
 *\file card.h
 * \brief Prototype od the card class
 * \author Samuel Ouellet (111 233 977)
 */

#ifndef TEXASHOLDEMGAME_H
#define TEXASHOLDEMGAME_H

#include <iostream> 
#include <vector>
#include <string>
#include <bits/stdc++.h> 
#include "contract_exception.h"
#include "Card.h"
#include "player.h"


namespace game {

    class TexasHoldemGame {

        struct GameState {
            int iteration;
            int hmPlayer;
            std::vector<Card> cardsOnTable;
            std::vector<Player> players;
        };

    private:
        int m_iteration;
        int m_hmPlayer;
        GameState m_currentGameState;

    public:
        TexasHoldemGame(int p_hmPlayer);

        static GameState getRandomNextState(GameState p_gameState);
        GameState getState();

    };
}

#endif //TEXASHOLDEMGAME_H