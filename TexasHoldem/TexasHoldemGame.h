/**
 *\file card.h
 * \brief Prototype od the card class
 * \author Samuel Ouellet (111 233 977)
 */

#ifndef TEXASHOLDEMGAME_H
#define TEXASHOLDEMGAME_H

#include <string>
#include "contract_exception.h"


namespace game {

    class TexasHoldemGame {

        struct GameState {
            int iteration;
        };

    private:
        int m_iteration;
        int m_hmPlayer;

    public:
        TexasHoldemGame(int p_hmPlayer);

    };
}

#endif //TEXASHOLDEMGAME_H