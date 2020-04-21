/**
 *\file card.h
 * \brief Prototype od the card class
 * \author Samuel Ouellet (111 233 977)
 */

#ifndef CARD_H
#define CARD_H

#include <string>
#include "contract_exception.h"


namespace game {

    enum Asset { CLUBS = 1, SPADES = 2, DIAMONDS = 3, HEART = 4};
    enum CardNumber {TWO=2, THREE=3, FOUR=4, FIVE=5, SIX=6, SEVEN=7 , EIGHT=8, NINE=9, TEN=10, JACK=11, QUEEN=12, KING=13, ACE=14};

    class Card {

    private:
        Asset m_asset;
        CardNumber m_number;
        int id;

    public:
        Card(Asset p_asset, CardNumber p_number);

        const Asset& reqAsset() const;
        const CardNumber& reqNumber() const;
        void  verifyInvariant() const;
        const std::string& asString() const;
        int GetId() const;

        friend bool operator==(const Card& p_lCard, const Card& p_rCard);

        static const int minNumber = TWO;
        static const int maxNumber = ACE;

        static std::string assetAsString(Asset p_asset);
        static std::string numberAsString(CardNumber p_number);

        static int GetId(Asset p_asset, int p_number);

    };
}

#endif //CARD_H