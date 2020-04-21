/**
 *\file card.cpp
 * \brief card class definition
 * \author Samuel Ouellet (111 233 977)
 */
#include <iostream>
#include "Card.h"


using namespace std;

 /**
  * \namespace game
  * \brief Namespace containing all objects related to a poker texas holdem game
  *
  * This namespace contains player, card and table_set classes
  */
namespace game {
    /**
     * \brief Card constructor
     *
     * Will create a valid card with a number and an asset.
     * \param p_asset Asset of the card
     * \param p_number Number of the card.
     * \pre p_asset Must be a string among: clubs, heart, spades and diamonds .
     * \pre p_number Must be an int from 1 to 13.
     *
     * \return A card object.
     */
    Card::Card(Asset p_asset, CardNumber p_number) :
        m_asset(p_asset), m_number(p_number) {
        //PRECONDITION(0 < p_number);
        //PRECONDITION(p_number < 14);

        id = Card::GetId(p_asset, p_number);
        
        POSTCONDITION(m_asset == p_asset);
        POSTCONDITION(m_number == p_number);
        POSTCONDITION(id == p_number * Asset(p_asset))
        
    }

    Card* Card::copy() const
    {
        return new Card(m_asset, m_number);
    }

    /**
     * \brief Getter of attribute m_asset.
     * \return The asset of the card.
     */
    const Asset& Card::reqAsset() const {
        return m_asset;
    }
    /**
     * \brief Getter of the m_number attribute.
     * \return The number of the card.
     */
    const CardNumber& Card::reqNumber() const {
        return m_number;
    }
    /**
     * \fn void Reference::verifieInvariant(void) const
     * \brief Méthode servant à s'assurer que les invariants sont corrects. C'est un implentation de
     * la théorie du contrat.
     */
    void Card::verifyInvariant() const {
        INVARIANT(1 < m_number);
        INVARIANT(m_number < 15);
    }


    int Card::GetId(Asset p_asset, int p_number)
    {
        return p_number * Asset(p_asset);
    }

    int Card::GetId() const {
        return id;
    }

    string Card::assetAsString(Asset p_asset) {
        string assetStr = "";
        switch (p_asset)
        {
        case game::CLUBS:
            assetStr = "Clubs";
            break;
        case game::HEART:
            assetStr = "Heart";
            break;
        case game::SPADES:
            assetStr = "Spades";
            break;
        case game::DIAMONDS:
            assetStr = "Diamonds";
            break;
        default:
            break;
        }
        return assetStr;
    }

    string Card::numberAsString(CardNumber p_number) {
        string numberStr = "";

        switch (p_number)
        {
        case game::TWO:
            numberStr = "2";
            break;
        case game::THREE:
            numberStr = "3";
            break;
        case game::FOUR:
            numberStr = "4";
            break;
        case game::FIVE:
            numberStr = "5";
            break;
        case game::SIX:
            numberStr = "6";
            break;
        case game::SEVEN:
            numberStr = "7";
            break;
        case game::EIGHT:
            numberStr = "8";
            break;
        case game::NINE:
            numberStr = "9";
            break;
        case game::TEN:
            numberStr = "10";
            break;
        case game::JACK:
            numberStr = "J";
            break;
        case game::QUEEN:
            numberStr = "Q";
            break;
        case game::KING:
            numberStr = "K";
            break;
        case game::ACE:
            numberStr = "A";
            break;
        default:
            break;
        }
        return numberStr;
    }
    
    const string& Card::asString() const {
        string out = "";
        out += Card::numberAsString(m_number) + " of " + Card::assetAsString(m_asset);
        return out;
    }

	bool operator==(const Card& p_lCard, const Card& p_rCard)
	{
		return p_lCard.GetId() == p_rCard.GetId();
	}
}

