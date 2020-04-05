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

    enum Asset { CLUBS = 1, HEART = 2, SPADES = 3, DIAMONDS = 4 };

    class Card {

    private:
        Asset m_asset;
        int m_number;
        int id;

    public:
        Card(Asset p_asset, int p_number);

        const Asset& reqAsset() const;
        const int& reqNumber() const;
        void  verifyInvariant() const;

        int GetId();

        static int GetId(Asset p_asset, int p_number);

    };
}

#endif //CARD_H