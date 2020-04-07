


#include <vector>
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;

namespace game {

	TexasHoldemGame::TexasHoldemGame(vector<Player> p_players, int p_startBank): m_players(p_players), m_startBank(p_startBank) {
		
	}

	void TexasHoldemGame::start() {
		reset();

		for(int i = 0; i < m_currentGameState.players.size(); i++)
		{
			vector<Card> hand;
			for (int i = 0; i < 2; i++)
			{
				Card card = m_cardStack.getCard();
				hand.push_back(card);
			}
			m_currentGameState.players[i].setCards(hand);
		}
		updateCurrentGameState();
	}

	void TexasHoldemGame::reset() {

		m_cardStack.reset();

		vector<Card> cards = vector<Card>();

		for (int i = 0; i < 3; i++) {
			cards.push_back(m_cardStack.getCard());
		}

		m_currentGameState.board = cards;
		m_currentGameState.iteration = 0;
		m_currentGameState.players = m_players;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			m_currentGameState.players[i].m_playerState.bank = m_startBank;
			m_currentGameState.players[i].m_playerState.bet = 0;
			m_currentGameState.players[i].m_playerState.active = true;
		}

		m_currentGameState.iteration = 0;
	}


	bool TexasHoldemGame::doRound() {

		bool endRound = false;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (!m_currentGameState.players[i].m_playerState.active) continue;

			Action playerAction = m_currentGameState.players[i].getAction(m_currentGameState, getPossibleActions(m_currentGameState.players[i].m_playerState, m_currentGameState));

			applyActionOnPlayer(m_currentGameState.players[i], playerAction);
		}
		
		if (m_currentGameState.board.size() == 5)
		{
			endHand();
			endRound = true;
		}
		else {
			m_currentGameState.board.push_back(m_cardStack.getCard());
		}
		updateCurrentGameState();

		return endRound;
	}

	void TexasHoldemGame::doHand(bool p_verbose) {
		bool endRound = false;

		while (!endRound)
		{
			if (p_verbose) cout << getCurrentStateAsString();
			endRound = doRound();
			
		}
	}

	void TexasHoldemGame::applyActionOnPlayer(Player& p_player, Action p_action) {
		ActionType actionType = p_action.actionType;

		switch (actionType)
		{
		case game::CHECK:
			m_currentGameState.currentCheck = m_currentGameState.currentCheck && true;
			// TODO; faire sur que les joueur qui ont check vont Follow, Flod, Raise ou All_in si le check n'est pas accepté.
			break;
		case game::ALL_IN:
			p_player.m_playerState.bet += p_player.m_playerState.bank;
			p_player.m_playerState.bank = 0;
			break;
		case game::RAISE:
			p_player.m_playerState.bet += p_player.m_playerState.raise;
			p_player.m_playerState.bank -= p_player.m_playerState.raise;
			break;
		case game::FLOD:
			p_player.m_playerState.active = false;
			break;
		case game::FOLLOW:
			p_player.m_playerState.bank -= m_currentGameState.currentBet - p_player.m_playerState.bet;
			p_player.m_playerState.bet = m_currentGameState.currentBet;
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
		GameState nextState = m_currentGameState;

		m_states.push_back(m_currentGameState);

		nextState.iteration++;
		m_currentGameState = nextState;
	}

	void TexasHoldemGame::endHand() {
		int banking = 0;
		int maxReward = 0;
		int winner = 0;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{			
			
			int playerReward = getReward(m_currentGameState.players[i].getCards());

			if (playerReward > maxReward) {
				maxReward = playerReward;
				winner = i;
			}
			
			banking += m_currentGameState.players[i].m_playerState.bet;
		}

		m_currentGameState.players[winner].m_playerState.bank += banking;

		cout << "\n " + m_currentGameState.players[winner].getName() + " win " + "a banking of " + to_string(banking) + " with method " + m_currentGameState.players[winner].getMethod() + "\n";


	}



	GameState TexasHoldemGame::getState() {
		return m_currentGameState;
	}





	int TexasHoldemGame::getReward(vector<Card> p_hand) {
		int reward = 0;

		vector<Card> playerHand = p_hand;
		playerHand.insert(playerHand.end(), m_currentGameState.board.begin(), m_currentGameState.board.end());

		for each (Card card in playerHand)
		{
			reward += card.reqNumber();
		}
		return reward;
	}






	string TexasHoldemGame::getCurrentStateAsString() {
		string out = "";
		out += "iteration: " + to_string(m_currentGameState.iteration) + "\n\n";

		out += "Board: ";
		for each (Card card in m_currentGameState.board)
		{
			out += card.asString() + " - ";
		}
		out += "\n";
		out += "Current bet: " + to_string(m_currentGameState.currentBet) + " current check: " + to_string(m_currentGameState.currentCheck) + "\n";

		out += "--- Players: \n \n";
		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			out += m_currentGameState.players[i].getName() + " - " + m_currentGameState.players[i].getMethod() + "\n";
			out += "Cards: ";
			for each (Card card in m_currentGameState.players[i].getCards())
			{
				out += card.asString() + " - ";
			}
			out += "\n";
			out += "Active: " + to_string(m_currentGameState.players[i].m_playerState.active) + "\n";
			out += "Bank: " + to_string(m_currentGameState.players[i].m_playerState.bank) + " bet: " 
				+ to_string(m_currentGameState.players[i].m_playerState.bet) 
				+ " raise: " + to_string(m_currentGameState.players[i].m_playerState.raise) + "\n";
			out += "Current reward: " + to_string(getReward(m_currentGameState.players[i].getCards())) + "\n";
			out += "--- \n";
		}

		out += "\n\n\n";

		return out;
	}

}