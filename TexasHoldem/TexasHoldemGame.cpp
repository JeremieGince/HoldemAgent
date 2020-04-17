


#include <vector>
#include<map>
#include <algorithm>
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;

namespace game {

	TexasHoldemGame::TexasHoldemGame(vector<Player*> p_players, int p_startBank): m_players(p_players), m_startBank(p_startBank) {
		for (int i = 0; i < p_players.size(); i++)
		{
			m_playerWins.insert({ p_players[i]->getName(), 0});
			m_playerGains.insert({ p_players[i]->getName(), 0 });
			m_playerLoss.insert({ p_players[i]->getName(), 0 });
		}
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
			m_currentGameState.players[i]->setCards(hand);
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
			m_currentGameState.players[i]->m_playerState.bank = m_startBank;
			m_currentGameState.players[i]->m_playerState.bet = 0;
			m_currentGameState.players[i]->m_playerState.active = true;
		}

		m_currentGameState.iteration = 0;
	}


	bool TexasHoldemGame::doRound() {

		bool endRound = false;
		vector<int> playerIdxOnCheck = vector<int>();
		vector<int> activePlayers = vector<int>();

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (!m_currentGameState.players[i]->m_playerState.active) continue;

			if (activePlayers.size() == 0 && i == (m_currentGameState.players.size() - 1)) break;
			Action playerAction = m_currentGameState.players[i]->getAction(m_currentGameState, getPossibleActions(m_currentGameState.players[i]->m_playerState, m_currentGameState));

			if (playerAction.actionType == CHECK) playerIdxOnCheck.push_back(i);

			applyActionOnPlayer(m_currentGameState.players[i], playerAction);
			if (m_currentGameState.players[i]->m_playerState.active) activePlayers.push_back(i);
		}

		if (!m_currentGameState.currentCheck) {
			// le check courant n'a pas passé, les joueurs ayant checké doivent rejouer.
			for (int i = 0; i < playerIdxOnCheck.size(); i++)
			{
				int idx = playerIdxOnCheck[i];

				if (activePlayers.size() == 0 && i == (playerIdxOnCheck.size() - 1)) break;

				Action playerAction = m_currentGameState.players[idx]->getAction(m_currentGameState, getPossibleActions(m_currentGameState.players[idx]->m_playerState, m_currentGameState));
				applyActionOnPlayer(m_currentGameState.players[idx], playerAction);
				if(!m_currentGameState.players[i]->m_playerState.active) activePlayers.erase(remove(activePlayers.begin(), activePlayers.end(), idx), activePlayers.end());
			}
		}
		
		if (m_currentGameState.board.size() == 5 || activePlayers.size() <= 1)
		{
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
		if (p_verbose) cout << getCurrentStateAsString();

		while (!endRound)
		{
			endRound = doRound();
			if (p_verbose) cout << getCurrentStateAsString();

			if (endRound) {
				int winner = endHand(p_verbose);
			}
			
			
		}
	} 

	void TexasHoldemGame::doHands(bool p_verbose, int p_hmHands) {
		for (int i = 0; i < p_hmHands; i++)
		{
			start();
			doHand(p_verbose);
		}

		if (p_verbose) cout << getWinsStatsAsString();
	}

	void TexasHoldemGame::incrementPlayerWins(string p_playerName) {
		std::map<string, int>::iterator it;
		it = m_playerWins.find(p_playerName);
		if (it != m_playerWins.end()) {
			m_playerWins[p_playerName]++;
		}
	}

	void TexasHoldemGame::increasePlayerGains(string p_playerName, int p_gains) {
		std::map<string, int>::iterator it;
		it = m_playerGains.find(p_playerName);
		if (it != m_playerGains.end()) {
			m_playerGains[p_playerName] += p_gains;
		}
	}

	void TexasHoldemGame::increasePlayerLoss(string p_playerName, int p_banking) {
		std::map<string, int>::iterator it;
		it = m_playerLoss.find(p_playerName);
		if (it != m_playerLoss.end()) {
			m_playerLoss[p_playerName] -= p_banking;
		}
	}

	void TexasHoldemGame::applyActionOnPlayer(Player* p_player, Action p_action) {
		ActionType actionType = p_action.actionType;

		switch (actionType)
		{
		case game::CHECK:
			m_currentGameState.currentCheck = m_currentGameState.currentCheck && true;
			break;
		case game::ALL_IN:
			m_currentGameState.currentCheck = m_currentGameState.currentCheck && false;
			p_player->m_playerState.bet += p_player->m_playerState.bank;
			p_player->m_playerState.bank = 0;
			break;
		case game::RAISE:
			// TODO: c'est l'action qui doit avoir le raise et non le joueur lui-meme...
			m_currentGameState.currentCheck = m_currentGameState.currentCheck && false;
			p_player->m_playerState.bet += p_player->m_playerState.raise;
			p_player->m_playerState.bank -= p_player->m_playerState.raise;
			break;
		case game::FOLD:
			p_player->m_playerState.active = false;
			break;
		case game::FOLLOW:
			p_player->m_playerState.bank -= m_currentGameState.currentBet - p_player->m_playerState.bet;
			p_player->m_playerState.bet = m_currentGameState.currentBet;
			break;
		default:
			p_player->m_playerState.active = false;
			break;
		}
	}


	vector<ActionType> TexasHoldemGame::getPossibleActions(PlayerState p_playerState, GameState p_gameState) {
		vector<ActionType> possibleActions { ALL_IN, FOLD, FOLLOW};
		if (m_currentGameState.currentCheck) possibleActions.push_back(CHECK);
		return possibleActions;
	}

	void TexasHoldemGame::updateCurrentGameState() {
		GameState nextState = m_currentGameState;

		m_states.push_back(m_currentGameState);

		nextState.iteration++;
		m_currentGameState = nextState;
		m_currentGameState.currentCheck = true;
	}

	int TexasHoldemGame::endHand(bool p_verbose) {
		int banking = 0;
		int maxReward = 0;
		int winner = 0;
		int activeCount = 0;

		bool draw = false;
		vector<int> winners;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (m_currentGameState.players[i]->m_playerState.active) activeCount++;
			int playerReward = getReward(m_currentGameState.players[i]->getCards()) * (int) m_currentGameState.players[i]->m_playerState.active;

			if (playerReward > maxReward) {
				maxReward = playerReward;
				winner = i;
			}
			if (playerReward == maxReward){
				if(!draw) winners.push_back(winner);
				winners.push_back(i);
				draw = true;
			}
			
			banking += m_currentGameState.players[i]->m_playerState.bet;
		}

		if (draw) {
			// TODO: split the banking
		}

		m_currentGameState.players[winner]->m_playerState.bank += banking;

		incrementPlayerWins(m_currentGameState.players[winner]->getName());
		increasePlayerGains(m_currentGameState.players[winner]->getName(), banking - m_currentGameState.players[winner]->m_playerState.bet);

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (i != winner) increasePlayerLoss(m_currentGameState.players[i]->getName(), m_currentGameState.players[i]->m_playerState.bet);
		}
		
		m_handCounter++;
		m_totalGains += banking;

		if(p_verbose) cout << "\n " + m_currentGameState.players[winner]->getName() + " win " + "a banking of " + to_string(banking) + " with method " + m_currentGameState.players[winner]->getMethod() + "\n";

		return winner;
	}

	GameState TexasHoldemGame::getState() {
		return m_currentGameState;
	}


	bool TexasHoldemGame::checkRoyalFlush(std::vector<Card> p_hand) {
		bool hasAce = false;
		bool hasKing = false;
		bool hasQueen = false;
		bool hasJack = false;
		bool hasTen = false;

		for each (Card card in p_hand)
		{
			switch (card.reqNumber())
			{
			case ACE:
				hasAce = true;
				break;
			case KING:
				hasKing = true;
				break;
			case QUEEN:
				hasQueen = true;
				break;
			case JACK:
				hasJack = true;
				break;
			case TEN:
				hasTen = true;
				break;
			default:
				break;
			}
		}
		return hasAce && hasKing && hasQueen && hasJack && hasTen;
	}


	bool TexasHoldemGame::checkStraightFlush(std::vector<Card> p_hand) {
		bool sameSuit = true;
		bool hasACE = false;
		Asset suit = p_hand[0].reqAsset();
		vector<int> sequence;
		int sum = 0;
		for each (Card card in p_hand)
		{
			if (card.reqAsset() != suit) return false;

			if (card.reqNumber() == ACE) {
				hasACE = true;
			}
			else {
				int num = Asset(card.reqNumber());
				sequence.push_back(num);
				sum += num;
			}
		}

		sort(sequence.begin(), sequence.end());
		if (sequence[0] == 2) {
			sequence.push_back(1);
			sum += 1;
			sort(sequence.begin(), sequence.end());
		}
		else if(sequence[sequence.size()-1] == Asset(KING)){
			sequence.push_back(Asset(KING)+1);
			sum += Asset(KING)+1;
		}

		int expectedResult = ((sequence[sequence.size()-1] * (sequence[sequence.size()-1] + 1)) / 2) - (((sequence[0]-1) * sequence[0]) / 2);

		return sameSuit && expectedResult == sum;
	}

	bool TexasHoldemGame::checkFourOfAKind(std::vector<Card> p_hand) {
		map<CardNumber, int> count;
		map<CardNumber, int>::iterator it;

		for each (Card card in p_hand)
		{
			it = count.find(card.reqNumber());
			if (it == count.end()) {
				count.insert({ card.reqNumber(), 1 });
			}
			else {
				it->second++;
			}
		}
		it = count.begin();
		while (it != count.end()) {

			if (it->second >= 4) return true;

			it++;
		}
		return false;
	}


	bool TexasHoldemGame::checkFullHouse(std::vector<Card> p_hand) {
		bool hasThree = false;
		bool hasPair = false;

		map<CardNumber, int> count;
		map<CardNumber, int>::iterator it;

		for each (Card card in p_hand)
		{
			it = count.find(card.reqNumber());
			if (it == count.end()) {
				count.insert({ card.reqNumber(), 1 });
			}
			else {
				it->second++;
			}
		}
		it = count.begin();
		while (it != count.end()) {

			if (it->second == 3) hasThree = true;
			else if (it->second == 2) hasPair = true;

			it++;
		}
		return hasPair && hasThree;
	}


	bool TexasHoldemGame::checkFlush(std::vector<Card> p_hand){
		Asset suit = p_hand[0].reqAsset();
		for each (Card card in p_hand)
		{
			if (card.reqAsset() != suit) return false;
		}
		return true;
	}


	bool TexasHoldemGame::checkStraight(std::vector<Card> p_hand) {
		bool hasACE = false;
		Asset suit = p_hand[0].reqAsset();
		vector<int> sequence;
		int sum = 0;
		for each (Card card in p_hand)
		{
			if (card.reqNumber() == ACE) {
				hasACE = true;
			}
			else {
				int num = Asset(card.reqNumber());
				sequence.push_back(num);
				sum += num;
			}
		}

		sort(sequence.begin(), sequence.end());
		if (sequence[0] == 2) {
			sequence.push_back(1);
			sum += 1;
			sort(sequence.begin(), sequence.end());
		}
		else if (sequence[sequence.size() -1] == Asset(KING)) {
			sequence.push_back(Asset(KING) + 1);
			sum += Asset(KING) + 1;
		}

		int expectedResult = ((sequence[sequence.size() - 1] * (sequence[sequence.size() - 1] + 1)) / 2) - (((sequence[0] - 1) * sequence[0]) / 2);

		return expectedResult == sum;
	}


	bool TexasHoldemGame::checkThreeOfAKind(std::vector<Card> p_hand) {
		map<CardNumber, int> count;
		map<CardNumber, int>::iterator it;

		for each (Card card in p_hand)
		{
			it = count.find(card.reqNumber());
			if (it == count.end()) {
				count.insert({ card.reqNumber(), 1 });
			}
			else {
				it->second++;
			}
		}
		it = count.begin();
		while (it != count.end()) {

			if (it->second >= 3) return true;

			it++;
		}
		return false;
	}


	bool TexasHoldemGame::checkTwoPair(std::vector<Card> p_hand) {
		int pairCount = 0;

		map<CardNumber, int> count;
		map<CardNumber, int>::iterator it;

		for each (Card card in p_hand)
		{
			it = count.find(card.reqNumber());
			if (it == count.end()) {
				count.insert({ card.reqNumber(), 1 });
			}
			else {
				it->second++;
			}
		}
		it = count.begin();
		while (it != count.end()) {

			if (it->second >= 2) pairCount++;

			it++;
		}
		return pairCount >= 2;
	}


	bool TexasHoldemGame::checkPair(std::vector<Card> p_hand) {
		map<CardNumber, int> count;
		map<CardNumber, int>::iterator it;

		for each (Card card in p_hand)
		{
			it = count.find(card.reqNumber());
			if (it == count.end()) {
				count.insert({ card.reqNumber(), 1 });
			}
			else {
				it->second++;
			}
		}
		it = count.begin();
		while (it != count.end()) {

			if (it->second >= 2) return true;

			it++;
		}
		return false;
	}



	int TexasHoldemGame::getReward(vector<Card> p_hand) {
		int reward = 0;

		vector<Card> playerHand = p_hand;
		playerHand.insert(playerHand.end(), m_currentGameState.board.begin(), m_currentGameState.board.end());

		for each (Card card in playerHand)
		{
			reward += card.reqNumber();
		}

		if (checkRoyalFlush(playerHand)) {
			reward += HandScoreOffset::ROYALFLUSHoffset;
		}
		else if (checkStraightFlush(playerHand)) {
			reward += HandScoreOffset::STRAIGHTFLUSHoffset;
		}
		else if (checkFourOfAKind(playerHand)) {
			reward += HandScoreOffset::FOUROFAKINDoffset;
		}
		else if (checkFullHouse(playerHand)) {
			reward += HandScoreOffset::FULLHOUSEoffset;
		}
		else if (checkFlush(playerHand)) {
			reward += HandScoreOffset::FLUSHoffset;
		}
		else if (checkStraight(playerHand)) {
			reward += HandScoreOffset::STRAIGHToffset;
		}
		else if (checkThreeOfAKind(playerHand)) {
			reward += HandScoreOffset::THREEOFAKINDoffset;
		}
		else if (checkTwoPair(playerHand)) {
			reward += HandScoreOffset::TWOPAIRoffset;
		}
		else if (checkPair(playerHand)) {
			reward += HandScoreOffset::PAIRoffset;
		}


		return reward;
	}



	string TexasHoldemGame::getWinsStatsAsString() {
		string out = "\n Number of hands: " + to_string(m_handCounter) + "\n";
		map<string, int>::iterator it = m_playerWins.begin();
		while (it != m_playerWins.end())
		{
			out += "Player: " + it->first 
				+ ", win rate: " + to_string(((float)it->second) / m_handCounter) 
				+ ", gains: " + to_string(m_playerGains[it->first]) 
				+ ", loss: " + to_string(m_playerLoss[it->first]) 
				+ " \n";

			it++;
		}
		return out;
	}


	string TexasHoldemGame::getCurrentStateAsString() {
		string out = "\n";
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
			out += m_currentGameState.players[i]->getName() + " - " + m_currentGameState.players[i]->getMethod() + "\n";
			out += "Cards: ";
			for each (Card card in m_currentGameState.players[i]->getCards())
			{
				out += card.asString() + " - ";
			}
			out += "\n";
			out += "Active: " + to_string(m_currentGameState.players[i]->m_playerState.active) + "\n";
			out += "Bank: " + to_string(m_currentGameState.players[i]->m_playerState.bank) + " bet: " 
				+ to_string(m_currentGameState.players[i]->m_playerState.bet) 
				+ " raise: " + to_string(m_currentGameState.players[i]->m_playerState.raise) + "\n";
			out += "Current reward: " + to_string(getReward(m_currentGameState.players[i]->getCards())) + "\n";
			out += "--- \n";
		}

		out += "\n\n\n";

		return out;
	}

}