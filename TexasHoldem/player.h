/**
 *\file player.h
 * \brief Prototype of player class
 * \author Samuel Ouellet (111 233 977)
 */
#ifndef POKER_MONTE_CARLO_PLAYER_H
#define POKER_MONTE_CARLO_PLAYER_H

#include <iostream> 
#include <vector>
#include <string>
#include "contract_exception.h"
#include "Card.h"


namespace game {

    struct GameState;
    struct Action;

    struct PlayerState {
        bool dealer;
        bool small_blind;
        bool big_blind;

        int bet;
        int bank;
        bool active;
    };

    class Player {

    private:
        std::string m_name;
        std::string m_method;

        std::vector<Card> m_cards;
        //GameState m_currentGameState;

    public:
        Player(std::string p_name, std::string p_method);

        PlayerState m_playerState;

        Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);

        void setCards(std::vector<Card> p_cards);
    };
}
#endif //POKER_MONTE_CARLO_PLAYER_H
