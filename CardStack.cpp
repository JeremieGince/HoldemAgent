#include<stack>
#include <algorithm>
#include <random>
#include "CardStack.h"

using namespace std;

namespace game {

	CardStack::CardStack(): m_cardStack() {
		reset();
	}

	Card CardStack::getCard() {
		Card card = m_cardStack.top();
		m_cardStack.pop();
		return card;
	}

	void CardStack::reset() {

		m_cardStack = std::stack<Card>();
		vector<Card> cards;

		for (int assetInt = CLUBS; assetInt != DIAMONDS; assetInt++)
		{
			Asset asset = static_cast<Asset>(assetInt);
			for (int i = 1; i < 15; i++)
			{
				cards.push_back(Card(asset, i));
			}
		}

		std::random_shuffle(cards.begin(), cards.end());

		for each (Card card in cards)
		{
			m_cardStack.push(card);
		}
	}

}
