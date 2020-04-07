#include<stack>
#include "CardStack.h"



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

		for (int assetInt = CLUBS; assetInt != DIAMONDS; assetInt++)
		{
			Asset asset = static_cast<Asset>(assetInt);
			for (int i = i; i < 15; i++)
			{
				m_cardStack.push(Card(asset, i));
			}
		}
	}

}
