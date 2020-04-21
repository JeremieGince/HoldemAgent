//
// Created by osamu on 2020-02-21.
//
#include <cstdlib> 
#include <ctime> 
#include "player.h"
#include "TexasHoldemGame.h"

using namespace std;


namespace game {

	Player::Player() : m_name(), m_method(), m_cards() {

	}

	Player::Player(string p_name, string p_method) : m_name(p_name), m_method(p_method), m_cards() {

	}


	Action Player::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {
		Action action;
		if (p_possibleActions.size() > 0) {
			action.actionType = p_possibleActions[rand() % p_possibleActions.size()];
		}
		
		action.bet = 1.0;
		return action;
	}

	void Player::setCards(vector<Card> p_cards) {
		m_cards = p_cards;
	}

	vector<Card> Player::getCards() {
		return m_cards;
	}


	string Player::getName() {
		return m_name;
	}

	string Player::getMethod() {
		return m_method;
	}
}
