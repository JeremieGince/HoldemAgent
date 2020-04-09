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
    ALL_IN => bet tout l'argent de la bank du joueur. Attention, on ne peut que gagner l'�quivalent de sa bank.
    RAISE => augmente la mise avec un bet supp�rieur au joueur pr�c�dent.
    FLOD => jetter ses cartes.
    FOLLOW => suivre le bet actuel.
    */
    enum ActionType{CHECK=0, ALL_IN=1, RAISE=2, FLOD=3, FOLLOW=4 };
    struct Action {
        ActionType actionType = CHECK;
        int bet = 0;
    };

    struct GameState {
        int iteration = 0;
        std::vector<Player> players;
        std::vector<Card> board;
        int currentBet = 0;
        bool currentCheck = true;
    };

    /*
    reward of a hand = HandScoreOffset + value of the hand
    HandScoreOffset[i] = maximum value of the hand i-1 + HandScoreOffset[i-1]

    Maximum values:

        HIGHCARD = ACE * HEART -> One Ace
        PAIR = 2 * ACE * Heart -> 2 Ace
        TWOPAIR = 2 * ACE + 2 * KING -> 2 Ace and 2 King
        THREEOFAKIND = 3 * Ace -> tree Ace
        STRAIGHT = ACE + KING + QUEEN + JACK + TEN -> The suit 10, Jack, Queen, King, Ace,
        FLUSH = ACE + KING + QUEEN + JACK + TEN -> the collection 10, Jack, Queen, King, Ace in a certain kind,
        FULLHOUSE = 3* ACE + 2 * KING, -> 3 Ace and a pair of King,
        FOUROFAKIND = 4 * Ace -> Four ,
        STRAIGHTFLUSH,
        ROYALFLUSH


    ref: https://www.cardplayer.com/rules-of-poker/hand-rankings

    */
    enum HandMaximumValue {
        HIGHCARDMaxVal = ACE + KING + QUEEN + JACK + TEN,
        PAIRMaxVal = 2*ACE + KING + QUEEN + JACK,
        TWOPAIRMaxVal = 2 * ACE + 2 * KING + QUEEN,
        THREEOFAKINDMaxVal = 3 * ACE + KING + QUEEN,
        STRAIGHTMaxVal = ACE + KING + QUEEN + JACK + TEN,
        FLUSHMaxVal = ACE + KING + QUEEN + JACK + TEN,
        FULLHOUSEMaxVal = 3 * ACE + 2 * KING,
        FOUROFAKINDMaxVal = 4 * ACE + KING,
        STRAIGHTFLUSHMaxVal = ACE + KING + QUEEN + JACK + TEN,
        ROYALFLUSHMaxVal = ACE + KING + QUEEN + JACK + TEN
    };
    enum HandScoreOffset{
        HIGHCARDoffset = 0, 
        PAIRoffset = HIGHCARDMaxVal,
        TWOPAIRoffset = HIGHCARDMaxVal + PAIRMaxVal,
        THREEOFAKINDoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal,
        STRAIGHToffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal,
        FLUSHoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal + STRAIGHTMaxVal,
        FULLHOUSEoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal + STRAIGHTMaxVal + FLUSHMaxVal,
        FOUROFAKINDoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal + STRAIGHTMaxVal + FLUSHMaxVal + FULLHOUSEMaxVal,
        STRAIGHTFLUSHoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal + STRAIGHTMaxVal + FLUSHMaxVal + FULLHOUSEMaxVal + FOUROFAKINDMaxVal,
        ROYALFLUSHoffset = HIGHCARDMaxVal + PAIRMaxVal + TWOPAIRMaxVal + THREEOFAKINDMaxVal + STRAIGHTMaxVal + FLUSHMaxVal + FULLHOUSEMaxVal + FOUROFAKINDMaxVal + STRAIGHTFLUSHMaxVal,
    };


    class TexasHoldemGame {

    private:
        std::vector<GameState> m_states;
        std::vector<Player> m_players;
        GameState m_currentGameState;
        int m_startBank;
        CardStack m_cardStack;

        void endHand();
        void applyActionOnPlayer(Player& p_player, Action p_action);


    public:
        TexasHoldemGame(std::vector<Player> p_players, int p_startBank);

        static GameState getRandomNextState(GameState p_gameState);
        GameState getState();
        std::vector<ActionType> getPossibleActions(PlayerState p_playerState, GameState p_gameState);

        int getReward(std::vector<Card> p_hand);

        void start();
        bool doRound();
        void reset();

        void updateCurrentGameState();
        void doRoundUntilEnd();
        void doHand(bool p_verbose = true);
        std::string getCurrentStateAsString();

        // checker of hands
        bool checkRoyalFlush(std::vector<Card> p_hand);
        bool checkStraightFlush(std::vector<Card> p_hand);
        bool checkFourOfAKind(std::vector<Card> p_hand);
        bool checkFullHouse(std::vector<Card> p_hand);
        bool checkFlush(std::vector<Card> p_hand);
        bool checkStraight(std::vector<Card> p_hand);
        bool checkThreeOfAKind(std::vector<Card> p_hand);
        bool checkTwoPair(std::vector<Card> p_hand);
        bool checkPair(std::vector<Card> p_hand);

    };
}

#endif //TEXASHOLDEMGAME_H