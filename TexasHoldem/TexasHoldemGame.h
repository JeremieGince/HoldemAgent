/**
 *\file card.h
 * \brief Prototype od the card class
 * \author Samuel Ouellet (111 233 977)
 */

#ifndef TEXASHOLDEMGAME_H
#define TEXASHOLDEMGAME_H

#include <iostream> 
#include <vector>
#include <array>
#include <map>
#include <string>
#include <cmath>
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
    enum ActionType{CHECK=0, ALL_IN=1, RAISE=2, FOLD=3, FOLLOW=4 };
    struct Action {
        ActionType actionType = CHECK;
        int bet = 0;
    };

    struct GameState {
        int iteration = 0;
        std::vector<Player*> players;
        std::vector<Card> board;
        int currentBet = 0;
        bool currentCheck = true;
        CardStack* cardStack;

        bool terminal = false;
        int winnerIdx = 0;
        int expectedWinnerIdx = 0;
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
        std::vector<Player*> m_players;
        GameState m_currentGameState;
        int m_startBank;
        CardStack m_cardStack;

        int m_handCounter = 0;
        int m_totalGains = 0;

        std::map<std::string, int> m_playerWins;
        void incrementPlayerWins(std::string p_playerName);

        std::map<std::string, int> m_playerGains;
        void increasePlayerGains(std::string p_playerName, int p_gains);

        std::map<std::string, int> m_playerLoss;
        void increasePlayerLoss(std::string p_playerName, int p_banking);

        std::map<std::string, int> m_playerExpectedWins;
        void incrementPlayerExpectedWins(std::string p_playerName);

        std::map<std::string, int> m_playerSuccessfulWins;
        void incrementPlayerSuccessfulWins(std::string p_playerName);

        std::map<std::string, int> m_playerSuccessfulLoss;
        void incrementPlayerSuccessfulLoss(std::string p_playerName);

        std::map<std::string, int> m_playerBluffDetected;
        void incrementPlayerBluffDetected(std::string p_playerName);

        std::array<int, 2> endHand();
        void applyActionOnPlayer(Player* p_player, Action p_action);


    public:
        TexasHoldemGame();
        TexasHoldemGame(std::vector<Player*> p_players, int p_startBank);

        static GameState getRandomNextState(GameState p_gameState, Action p_action);
        GameState getState();
        static std::vector<ActionType> getPossibleActions(PlayerState p_playerState, GameState p_gameState);

        int getReward(std::vector<Card> p_hand);

        void setBoard(std::vector<Card> p_board);
        void setPlayerHand(std::vector<Card> p_hand, int p_playerIdx);
        void redistributeCards(std::vector<int> p_dontTuchIdx = {});
        void setStartingCards(std::map<int, std::vector<Card>> p_cardsToSet);

        void resetCardStack();

        void start();
        bool doRound();
        void reset();

        void updateCurrentGameState();
        void doRoundUntilEnd();
        std::array<int, 2> doHand(bool p_verbose = true);
        void doHands(bool p_verbose = true, int p_hmHands = 10);
        std::string getCurrentStateAsString();

        std::string getWinsStatsAsString();

        float getSuccessfulWinRate(std::string p_playerName);
        float getSuccessfulLossRate(std::string p_playerName);
        float getBluffDetectedRate(std::string p_playerName);
        float getEfficiency(std::string p_playerName);
        float getNonRandomEfficiency(std::string p_playerName);

        // checker of hands
        static bool checkRoyalFlush(std::vector<Card> p_hand);
        static bool checkStraightFlush(std::vector<Card> p_hand);
        static bool checkFourOfAKind(std::vector<Card> p_hand);
        static bool checkFullHouse(std::vector<Card> p_hand);
        static bool checkFlush(std::vector<Card> p_hand);
        static bool checkStraight(std::vector<Card> p_hand);
        static bool checkThreeOfAKind(std::vector<Card> p_hand);
        static bool checkTwoPair(std::vector<Card> p_hand);
        static bool checkPair(std::vector<Card> p_hand);

    };
}

#endif //TEXASHOLDEMGAME_H