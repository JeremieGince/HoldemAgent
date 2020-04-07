//
// Created by osamu on 2020-02-21.
//

#include "player.h"
#include "TexasHoldemGame.h"

using namespace std;


namespace game {

	Player::Player(string p_name, string p_method) : m_name(p_name), m_method(p_name), m_cards() {

	}


	Action Player::getAction(GameState p_gameState, std::vector<Action> p_possibleActions) {
		Action action;
		action.actionType = ALL_IN;
		action.bet = 1.0;
		return p_possibleActions[0];
	}

	void Player::setCards(vector<Card> p_cards) {
		m_cards = p_cards;
	}


}
