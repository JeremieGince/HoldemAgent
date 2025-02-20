/**
 *\file player.h
 * \brief Prototype of player class
 * \author Samuel Ouellet (111 233 977)
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream> 
#include <vector>
#include <string>
#include <time.h>
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

    protected:
        std::string m_name;
        std::string m_method;
        std::vector<Card> m_cards;

    public:
        Player();
        Player(std::string p_name, std::string p_method);

        int m_playerIdx;

        PlayerState m_playerState;

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
        virtual void reset();

        void setCards(std::vector<Card> p_cards);
        std::vector<Card> getCards();

        std::string getName();
        std::string getMethod();
    };
}
#endif //PLAYER_H
