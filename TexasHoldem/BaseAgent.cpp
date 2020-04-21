#include <vector>
#include<map>
#include <algorithm>
#include "BaseAgent.h"
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;


namespace game {


	BaseAgent::BaseAgent(std::string p_name) : Player(p_name, "Base") {
	}


	Action BaseAgent::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {
		Action action;
		action.bet = 1.0;

		vector<ActionType>::iterator it;

		vector<Card> playerHand = m_cards;
		playerHand.insert(playerHand.end(), p_gameState.board.begin(), p_gameState.board.end());


		if (TexasHoldemGame::checkRoyalFlush(playerHand)) {
			action.actionType = ALL_IN;
		}
		else if (TexasHoldemGame::checkStraightFlush(playerHand)) {
			action.actionType = ALL_IN;
		}
		else if (TexasHoldemGame::checkFourOfAKind(playerHand)) {
			action.actionType = ALL_IN;
		}
		else if (TexasHoldemGame::checkFullHouse(playerHand)) {
			action.actionType = ALL_IN;
		}
		else if (TexasHoldemGame::checkFlush(playerHand)) {
			action.actionType = ALL_IN;
		}
		else if (TexasHoldemGame::checkStraight(playerHand)) {
			action.actionType = FOLLOW;
		}
		else if (TexasHoldemGame::checkThreeOfAKind(playerHand)) {
			action.actionType = FOLLOW;
		}
		else if (TexasHoldemGame::checkTwoPair(playerHand)) {
			action.actionType = FOLLOW;
		}
		else if (TexasHoldemGame::checkPair(playerHand)) {

			it = find(p_possibleActions.begin(), p_possibleActions.end(), CHECK);
			if (p_gameState.board.size() <= 4) {
				action.actionType = FOLLOW;
			}else if (it != p_possibleActions.end()) {
				action.actionType = CHECK;
			}
			else {
				action.actionType = FOLD;
			}
		}
		else
		{
			it = find(p_possibleActions.begin(), p_possibleActions.end(), CHECK);
			if (p_gameState.board.size() <= 3) {
				action.actionType = FOLLOW;

			} else if (p_gameState.board.size() <= 4) {
				action.actionType = CHECK;
			}
			else if (it != p_possibleActions.end()) {
				action.actionType = CHECK;
			}
			else {
				action.actionType = FOLD;
			}
		}

		return action;
	}

}