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
#include "contract_exception.h"
#include "Card.h"
#include "CardStack.h"


namespace game {

    struct PlayerState;
    class Player;

    /*
    CHECK => mise rien, demande aux autres joueur de voir la prochaine carte.
    ALL_IN => bet tout l'argent de la bank du joueur. Attention, on ne peut que gagner l'équivalent de sa bank.
    RAISE => augmente la mise avec un bet suppérieur au joueur précédent.
    FLOD => jetter ses cartes.
    FOLLOW => suivre le bet actuel.
    */
    enum ActionType{CHECK=0, ALL_IN=1, RAISE=2, FLOD=3, FOLLOW=4 };
    struct Action {
        ActionType actionType;
        int bet;
    };

    struct GameState {
        int iteration;
        std::vector<Player> players;
        std::vector<Card> board;
    };

    class TexasHoldemGame {

    private:
        int m_iteration;
        int m_hmPlayer;
        std::vector<Player> m_players;
        GameState m_currentGameState;
        int m_startBank;
        CardStack m_cardStack;

        // TODO
        void incrementBankForWinner();


    public:
        TexasHoldemGame(std::vector<Player> p_players, int p_startBank);

        static GameState getRandomNextState(GameState p_gameState);
        GameState getState();
        std::vector<Action> getPossibleActions(PlayerState p_playerState, GameState p_gameState);

        int getReward(std::vector<Card> p_hand);

        void start();
        void doRound();
        void reset();

        // TODO: implémenter
        void doRoundUntilEnd();
        void doHand();

    };
}

#endif //TEXASHOLDEMGAME_H