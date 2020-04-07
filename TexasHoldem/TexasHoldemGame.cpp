


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
		}

		m_iteration = 0;
	}


	void TexasHoldemGame::doRound() {
		for each (Player player in m_players)
		{
			Action playerAction = player.getAction(m_currentGameState, getPossibleActions(player.m_playerState, m_currentGameState));
			player.m_playerState.bet += playerAction.bet;
			player.m_playerState.bank -= playerAction.bet;
		}
		m_currentGameState.board.push_back(m_cardStack.getCard());
		m_iteration++;
	}


	vector<Action> TexasHoldemGame::getPossibleActions(PlayerState p_playerState, GameState p_gameState) {
		return vector<Action>{ALL_IN};
	}



	GameState TexasHoldemGame::getState() {
		return m_currentGameState;
	}

}