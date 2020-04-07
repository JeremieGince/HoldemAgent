/**
 *\file card.cpp
 * \brief card class definition
 * \author Samuel Ouellet (111 233 977)
 */

#include "Card.h"

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
    Card::Card(Asset p_asset, int p_number) :
        m_asset(p_asset), m_number(p_number) {
        //PRECONDITION(0 < p_number);
        //PRECONDITION(p_number < 14);

        id = Card::GetId(p_asset, p_number);
        
        POSTCONDITION(m_asset == p_asset);
        POSTCONDITION(m_number == p_number);
        POSTCONDITION(id == p_number * Asset(p_asset))
        
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
    const int& Card::reqNumber() const {
        return m_number;
    }
    /**
     * \fn void Reference::verifieInvariant(void) const
     * \brief Méthode servant à s'assurer que les invariants sont corrects. C'est un implentation de
     * la théorie du contrat.
     */
    void Card::verifyInvariant() const {
        INVARIANT(0 < m_number);
        INVARIANT(m_number < 14);
    }


    int Card::GetId(Asset p_asset, int p_number)
    {
        return p_number * Asset(p_asset);
    }

    int Card::GetId() {
        return id;
    }
    ;
}

