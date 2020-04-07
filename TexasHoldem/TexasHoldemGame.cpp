


#include <vector>
#include "TexasHoldemGame.h"
#include "player.h"

using namespace std;

namespace game {

	TexasHoldemGame::TexasHoldemGame(vector<Player> p_players, int p_startBank): m_players(p_players), m_startBank(p_startBank) {
		
	}

	void TexasHoldemGame::start() {
		reset();

		for each (Player player in m_players)
		{
			vector<Card> hand;
			for (int i = 0; i < 2; i++)
			{
				hand.push_back(m_cardStack.getCard());
			}
			player.setCards(hand);
		}
	}

	void TexasHoldemGame::reset() {

		m_cardStack.reset();

		vector<Card> cards;

		for (int i = 0; i < 3; i++) {
			cards.push_back(m_cardStack.getCard());
		}

		m_currentGameState.board = cards;
		m_currentGameState.iteration = 0;
		m_currentGameState.players = m_players;

		for each (Player player in m_players)
		{
			player.m_playerState.bank = m_startBank;
			player.m_playerState.bet = 0;
			player.m_playerState.active = true;
		}

		m_iteration = 0;
	}


	void TexasHoldemGame::doRound() {
		for each (Player player in m_players)
		{
			if (!player.m_playerState.active) continue;

			Action playerAction = player.getAction(m_currentGameState, getPossibleActions(player.m_playerState, m_currentGameState));

			applyActionOnPlayer(player, playerAction);
		}
		updateCurrentGameState();
		m_currentGameState.board.push_back(m_cardStack.getCard());
		m_iteration++;
	}

	void TexasHoldemGame::applyActionOnPlayer(Player p_player, Action p_action) {
		ActionType actionType = p_action.actionType;

		switch (actionType)
		{
		case game::CHECK:
			
			break;
		case game::ALL_IN:
			p_player.m_playerState.bet = p_player.m_playerState.bank;
			break;
		case game::RAISE:
			break;
		case game::FLOD:
			p_player.m_playerState.active = false;
			break;
		case game::FOLLOW:
			p_player.m_playerState.bet = m_currentGameState.currentBet;
			p_player.m_playerState.bank;
			break;
		default:
			p_player.m_playerState.active = false;
			break;
		}
	}


	vector<ActionType> TexasHoldemGame::getPossibleActions(PlayerState p_playerState, GameState p_gameState) {
		vector<ActionType> possibleActions { ALL_IN, FLOD };
		return possibleActions;
	}

	void TexasHoldemGame::updateCurrentGameState() {

	}



	GameState TexasHoldemGame::getState() {
		return m_currentGameState;
	}

	int TexasHoldemGame::getReward(vector<Card> p_hand) {
		int reward = 0;
		for each (Card card in p_hand)
		{
			reward += card.reqNumber();
		}
		return reward;
	}

}