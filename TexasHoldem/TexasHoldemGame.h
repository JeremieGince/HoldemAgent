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
        HIGHCARD = ACE + KING + QUEEN + JACK + TEN,
        PAIR = 2*ACE + KING + QUEEN + JACK,
        TWOPAIR = 2 * ACE + 2 * KING + QUEEN,
        THREEOFAKIND = 3 * ACE + KING + QUEEN,
        STRAIGHT = ACE + KING + QUEEN + JACK + TEN,
        FLUSH = ACE + KING + QUEEN + JACK + TEN,
        FULLHOUSE = 3 * ACE + 2 * KING,
        FOUROFAKIND = 4 * ACE + KING,
        STRAIGHTFLUSH = ACE + KING + QUEEN + JACK + TEN,
        ROYALFLUSH = ACE + KING + QUEEN + JACK + TEN
    };
    enum HandScoreOffset{
        HIGHCARD = 0, 
        PAIR = HandMaximumValue::HIGHCARD,
        TWOPAIR = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR,
        THREEOFAKIND = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR,
        STRAIGHT = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND,
        FLUSH = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND + HandMaximumValue::STRAIGHT,
        FULLHOUSE = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND + HandMaximumValue::STRAIGHT + HandMaximumValue::FLUSH,
        FOUROFAKIND = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND + HandMaximumValue::STRAIGHT + HandMaximumValue::FLUSH + HandMaximumValue::FULLHOUSE,
        STRAIGHTFLUSH = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND + HandMaximumValue::STRAIGHT + HandMaximumValue::FLUSH + HandMaximumValue::FULLHOUSE + HandMaximumValue::FOUROFAKIND,
        ROYALFLUSH = HandMaximumValue::HIGHCARD + HandMaximumValue::PAIR + HandMaximumValue::TWOPAIR + HandMaximumValue::THREEOFAKIND + HandMaximumValue::STRAIGHT + HandMaximumValue::FLUSH + HandMaximumValue::FULLHOUSE + HandMaximumValue::FOUROFAKIND + HandMaximumValue::STRAIGHTFLUSH,
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

    };
}

#endif //TEXASHOLDEMGAME_H