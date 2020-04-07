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
    enum ActionType;

    struct PlayerState {
        bool dealer = false;
        bool small_blind = false;
        bool big_blind = false;

        int bet = 0;
        int raise = 0;
        int bank = 0;
        bool active = true;
    };

    class Player {

    private:
        std::string m_name;
        std::string m_method;
        std::vector<Card> m_cards;
        //GameState m_currentGameState;

    public:
        Player();
        Player(std::string p_name, std::string p_method);

        PlayerState m_playerState;

        Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);

        void setCards(std::vector<Card> p_cards);
        std::vector<Card> getCards();

        std::string getName();
        std::string getMethod();
    };
}
#endif //POKER_MONTE_CARLO_PLAYER_H
