


#include <vector>
#include<map>
#include <algorithm>
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;

namespace game {

	TexasHoldemGame::TexasHoldemGame(): m_players(), m_startBank(0){
		
	}

	TexasHoldemGame::TexasHoldemGame(vector<Player*> p_players, int p_startBank): m_players(p_players), m_startBank(p_startBank) {
		cout << "nb player: " + to_string(m_players.size());
		for (int i = 0; i < p_players.size(); i++)
		{
			m_playerWins.insert({ p_players[i]->getName(), 0});
			m_playerGains.insert({ p_players[i]->getName(), 0 });
			m_playerLoss.insert({ p_players[i]->getName(), 0 });
			m_playerExpectedWins.insert({ p_players[i]->getName(), 0 });
			m_playerSuccessfulWins.insert({ p_players[i]->getName(), 0 });
			m_playerSuccessfulLoss.insert({ p_players[i]->getName(), 0 });
			m_playerBluffDetected.insert({ p_players[i]->getName(), 0 });
		}
	}

	void TexasHoldemGame::start() {
		reset();

		redistributeCards();
		
		updateCurrentGameState();
	}

	void TexasHoldemGame::reset() {
		m_cardStack.reset();
		
		m_currentGameState.iteration = 0;
		m_currentGameState.players = m_players;
		m_currentGameState.cardStack = &m_cardStack;
		m_currentGameState.terminal = false;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			m_currentGameState.players[i]->m_playerState.bank = m_startBank;
			m_currentGameState.players[i]->m_playerState.bet = 0;
			m_currentGameState.players[i]->m_playerState.active = true;
		}

		m_currentGameState.iteration = 0;
	}

	void TexasHoldemGame::redistributeCards(std::vector<int> p_dontTuchIdx) {
		/*
		idx = -1 -> board
		*/
		m_cardStack.reset();

		vector<Card> board = vector<Card>();

		if (find(p_dontTuchIdx.begin(), p_dontTuchIdx.end(), -1) == p_dontTuchIdx.end()) {
			for (int i = 0; i < 3; i++) {
				board.push_back(m_cardStack.getCard());
			}
			m_currentGameState.board = board;
		}

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (find(p_dontTuchIdx.begin(), p_dontTuchIdx.end(), i) == p_dontTuchIdx.end()) {
				vector<Card> hand;
				for (int i = 0; i < 2; i++)
				{
					Card card = m_cardStack.getCard();
					hand.push_back(card);
				}
				m_currentGameState.players[i]->setCards(hand);
				m_currentGameState.players[i]->m_playerIdx = i;
			}
			
		}
	}

	void TexasHoldemGame::setStartingCards(std::map<int, std::vector<Card>>p_cardsToSet)
	{
		reset();
		std::map<int, std::vector<Card>>::iterator mapIt;
		vector<int> dontTuchIdx;

		for (mapIt = p_cardsToSet.begin(); mapIt != p_cardsToSet.end(); mapIt++)
		{
			if (mapIt->first == -1) {
				setBoard(mapIt->second);
			}
			else {
				setPlayerHand(mapIt->second, mapIt->first);
			}
			dontTuchIdx.push_back(mapIt->first);
		}
		redistributeCards(dontTuchIdx);
	}

	void TexasHoldemGame::resetCardStack()
	{
		m_cardStack.reset();
	}

	bool TexasHoldemGame::doRound() {

		bool endRound = false;
		vector<int> playerIdxOnCheck = vector<int>();
		vector<int> activePlayers = vector<int>();

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (!m_currentGameState.players[i]->m_playerState.active) continue;

			if (activePlayers.size() == 0 && i == (m_currentGameState.players.size() - 1)) break;
			vector<ActionType> possibleActions = getPossibleActions(m_currentGameState.players[i]->m_playerState, m_currentGameState);

			if (possibleActions.size() == 0) continue;

			Action playerAction = m_currentGameState.players[i]->getAction(m_currentGameState, possibleActions);

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

				vector<ActionType> possibleActions = getPossibleActions(m_currentGameState.players[i]->m_playerState, m_currentGameState);

				if (possibleActions.size() == 0) continue;

				Action playerAction = m_currentGameState.players[idx]->getAction(m_currentGameState, possibleActions);
				applyActionOnPlayer(m_currentGameState.players[idx], playerAction);
				if(!m_currentGameState.players[i]->m_playerState.active) activePlayers.erase(remove(activePlayers.begin(), activePlayers.end(), idx), activePlayers.end());
			}
		}
		
		if (m_currentGameState.board.size() == 5 || activePlayers.size() <= 1)
		{
			array<int, 2> winners;
			endRound = true;
			winners = endHand();
		}
		else {
			m_currentGameState.board.push_back(m_cardStack.getCard());
		}
		updateCurrentGameState();

		return endRound;
	}

	array<int, 2> TexasHoldemGame::doHand(bool p_verbose) {
		bool endRound = false;
		array<int, 2> winners;
		if (p_verbose) cout << getCurrentStateAsString();

		while (!endRound)
		{
			endRound = doRound();

			if (endRound) {
				int winner = m_currentGameState.winnerIdx;
				int expectedWinner = m_currentGameState.expectedWinnerIdx;
				winners = {winner, expectedWinner};
			}

			if (p_verbose) cout << getCurrentStateAsString();

			
		}
		return winners;
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
			m_playerLoss[p_playerName] += p_banking;
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
		vector<ActionType> possibleActions { };

		if (p_gameState.board.size() < 5) {
			
			possibleActions.push_back(ALL_IN);
			possibleActions.push_back(FOLD);
			possibleActions.push_back(FOLLOW);

			if (p_gameState.currentCheck) possibleActions.push_back(CHECK);
		}
		
		return possibleActions;
	}

	void TexasHoldemGame::updateCurrentGameState() {
		GameState nextState = m_currentGameState;

		m_states.push_back(m_currentGameState);

		nextState.iteration++;
		m_currentGameState = nextState;
		m_currentGameState.currentCheck = true;
	}

	void TexasHoldemGame::incrementPlayerExpectedWins(std::string p_playerName)
	{
		std::map<string, int>::iterator it;
		it = m_playerExpectedWins.find(p_playerName);
		if (it != m_playerExpectedWins.end()) {
			m_playerExpectedWins[p_playerName]++;
		}
	}

	void TexasHoldemGame::incrementPlayerSuccessfulWins(std::string p_playerName)
	{
		std::map<string, int>::iterator it;
		it = m_playerSuccessfulWins.find(p_playerName);
		if (it != m_playerSuccessfulWins.end()) {
			m_playerSuccessfulWins[p_playerName]++;
		}
	}

	void TexasHoldemGame::incrementPlayerSuccessfulLoss(std::string p_playerName)
	{
		std::map<string, int>::iterator it;
		it = m_playerSuccessfulLoss.find(p_playerName);
		if (it != m_playerSuccessfulLoss.end()) {
			m_playerSuccessfulLoss[p_playerName]++;
		}
	}

	void TexasHoldemGame::incrementPlayerBluffDetected(std::string p_playerName)
	{
		std::map<string, int>::iterator it;
		it = m_playerBluffDetected.find(p_playerName);
		if (it != m_playerBluffDetected.end()) {
			m_playerBluffDetected[p_playerName]++;
		}
	}

	array<int, 2> TexasHoldemGame::endHand() {
		int banking = 0;

		int maxReward = 0;
		int winner = 0;
		int activeCount = 0;

		int maxExpectedReward = 0;
		int expectedWinner = 0;

		bool draw = false;
		vector<int> winners;

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (m_currentGameState.players[i]->m_playerState.active) activeCount++;
			
			int expectedPlayerReward = getReward(m_currentGameState.players[i]->getCards());
			int playerReward = expectedPlayerReward * (int)m_currentGameState.players[i]->m_playerState.active;

			if (playerReward > maxReward) {
				maxReward = playerReward;
				winner = i;
			}

			if (expectedPlayerReward > maxExpectedReward) {
				maxExpectedReward = expectedPlayerReward;
				expectedWinner = i;
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
		incrementPlayerExpectedWins(m_currentGameState.players[expectedWinner]->getName());
		increasePlayerGains(m_currentGameState.players[winner]->getName(), banking - m_currentGameState.players[winner]->m_playerState.bet);

		if (winner == expectedWinner) {
			incrementPlayerSuccessfulWins(m_currentGameState.players[winner]->getName());
		}
		else {
			incrementPlayerBluffDetected(m_currentGameState.players[winner]->getName());
		}

		for (int i = 0; i < m_currentGameState.players.size(); i++)
		{
			if (i != winner) increasePlayerLoss(m_currentGameState.players[i]->getName(), m_currentGameState.players[i]->m_playerState.bet);
			if (i != winner && i != expectedWinner && !m_currentGameState.players[i]->m_playerState.active) incrementPlayerSuccessfulLoss(m_currentGameState.players[i]->getName());
		}
		
		m_handCounter++;
		m_totalGains += banking;

		m_currentGameState.winnerIdx = winner;
		m_currentGameState.expectedWinnerIdx = expectedWinner;
		m_currentGameState.terminal = true;

		array<int, 2> out = { winner, expectedWinner };

		return out;
	}

	GameState TexasHoldemGame::getRandomNextState(GameState p_gameState, Action p_action)
	{

		switch (p_action.actionType)
		{
		case game::CHECK:
			p_gameState.currentCheck = p_gameState.currentCheck && true;
			break;
		case game::ALL_IN:
			p_gameState.currentCheck = p_gameState.currentCheck && false;
			break;
		case game::RAISE:
			// TODO: c'est l'action qui doit avoir le raise et non le joueur lui-meme...
			p_gameState.currentCheck = p_gameState.currentCheck && false;
			break;
		case game::FOLD:
			break;
		case game::FOLLOW:
			break;
		default:
			break;
		}

		return p_gameState;
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

	void TexasHoldemGame::setBoard(std::vector<Card> p_board)
	{
		ASSERTION(p_board.size() >= 3 && p_board.size() <= 5);
		for (int i = 0; i < p_board.size(); i++)
		{
			m_cardStack.removeFromStack(p_board[i]);
		}
		for (int i = 0; i < m_currentGameState.board.size(); i++)
		{
			m_cardStack.putInTheStack(m_currentGameState.board[i]);
		}
		m_currentGameState.iteration = p_board.size() - 1;
		m_currentGameState.board = p_board;

	}

	void TexasHoldemGame::setPlayerHand(std::vector<Card> p_hand, int p_playerIdx)
	{
		ASSERTION(p_hand.size() == 2);
		for (int i = 0; i < p_hand.size(); i++)
		{
			m_cardStack.removeFromStack(p_hand[i]);
		}
		cout << to_string(m_currentGameState.players.size());
		for (size_t i = 0; i < m_currentGameState.players[p_playerIdx]->getCards().size(); i++)
		{
			m_cardStack.putInTheStack(m_currentGameState.players[p_playerIdx]->getCards()[i]);
		}
		m_currentGameState.players[p_playerIdx]->setCards(p_hand);
	}



	string TexasHoldemGame::getWinsStatsAsString() {
		string out = "\n Number of hands: " + to_string(m_handCounter) + "\n";
		map<string, int>::iterator it = m_playerWins.begin();
		while (it != m_playerWins.end())
		{
			float win_rate = ((float)it->second) / m_handCounter;
			float expected_win_rate = ((float)m_playerExpectedWins[it->first]) / m_handCounter;
			out += "Player: " + it->first 
				+ ", win rate: " + to_string(win_rate) 
				+ ", expected win rate: " + to_string(expected_win_rate)
				+ ", successful win rate: " + to_string(((float)m_playerSuccessfulWins[it->first]) / ((float)m_playerExpectedWins[it->first]))
				+ ", successful loss rate: " + to_string(((float)m_playerSuccessfulLoss[it->first]) / (m_handCounter - m_playerExpectedWins[it->first]))
				+ ", bluff detected rate: " + to_string(((float)m_playerBluffDetected[it->first]) / ((float)it->second))
				+ ", efficiency: " + to_string(getEfficiency(it->first))
				+ ", n.rn-efficiency: " + to_string(getNonRandomEfficiency(it->first))
				//+ ", gains: " + to_string(((float)m_playerGains[it->first]) / m_totalGains)
				//+ ", loss: " + to_string(((float)m_playerLoss[it->first]) / m_totalGains)
				+ " \n";

			it++;
		}
		return out;
	}

	float TexasHoldemGame::getSuccessfullWinRate(std::string p_playerName)
	{
		float sw_rate = 0.0f;
		if (m_playerExpectedWins[p_playerName]>0) sw_rate = ((float)m_playerSuccessfulWins[p_playerName]) / m_playerExpectedWins[p_playerName];
		return sw_rate;
	}

	float TexasHoldemGame::getSuccessfullLossRate(std::string p_playerName)
	{
		float sl_rate = 0.0f;
		if((m_handCounter - m_playerExpectedWins[p_playerName]) > 0) sl_rate = ((float)m_playerSuccessfulLoss[p_playerName]) / (m_handCounter - m_playerExpectedWins[p_playerName]);
		return sl_rate;
	}

	float TexasHoldemGame::getBluffDetectedRate(std::string p_playerName)
	{
		float bd_rate = 0.0f;
		if(m_playerWins[p_playerName] > 0) bd_rate = ((float)m_playerBluffDetected[p_playerName]) / m_playerWins[p_playerName];
		return bd_rate;
	}

	float TexasHoldemGame::getEfficiency(std::string p_playerName)
	{
		float efficiency = 0.0f;
		if (m_handCounter > 0) {

			float sw_rate = getSuccessfullWinRate(p_playerName);
			float bd_rate = getBluffDetectedRate(p_playerName);
			float sl_rate = getSuccessfullLossRate(p_playerName);

			efficiency = (sw_rate + bd_rate + sl_rate)/3.0f;
		}
		return efficiency;
	}

	float TexasHoldemGame::getNonRandomEfficiency(std::string p_playerName)
	{
		return getEfficiency(p_playerName) - 0.5f;
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

		if (m_currentGameState.terminal) {
			out += "Winner: " + m_currentGameState.players[m_currentGameState.winnerIdx]->getName() + " - " + m_currentGameState.players[m_currentGameState.winnerIdx]->getMethod() + "\n";
			out += "Expected Winner: " + m_currentGameState.players[m_currentGameState.expectedWinnerIdx]->getName() + " - " + m_currentGameState.players[m_currentGameState.expectedWinnerIdx]->getMethod() + "\n";
		}

		out += "\n\n\n";

		return out;
	}

}