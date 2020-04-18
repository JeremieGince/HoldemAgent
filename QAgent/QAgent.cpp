#include <vector>
#include<map>
#include <algorithm>
#include "QAgent.h"
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;


namespace game {


	QAgent::QAgent(std::string p_name) : Player(p_name, "QAgent") {
	}


	Action QAgent::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {
		Action action;
		action.bet = 1.0;

		vector<ActionType>::iterator it;

		vector<Card> playerHand = m_cards;
		playerHand.insert(playerHand.end(), p_gameState.board.begin(), p_gameState.board.end());


		


		return action;
	}

}