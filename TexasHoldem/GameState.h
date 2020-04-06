/**
 *\file card.h
 * \brief Prototype od the card class
 * \author Samuel Ouellet (111 233 977)
 */

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <string>
#include "contract_exception.h"


namespace game {

    class GameState {

    private:
        int m_iteration;
        int m_hmPlayer;

    public:
        TexasHoldemGame(int p_hmPlayer);

    };
}

#endif //GAMESTATE_H
