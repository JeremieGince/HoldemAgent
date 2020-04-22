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

		for (int assetInt = CLUBS; assetInt <= HEART; assetInt++)
		{
			Asset asset = static_cast<Asset>(assetInt);
			for (int i = Card::minNumber; i <= Card::maxNumber; i++)
			{
				CardNumber cNumber = static_cast<CardNumber>(i);

				cards.push_back(Card(asset, cNumber));
			}
		}

		std::random_shuffle(cards.begin(), cards.end());

		for(int i = 0; i < cards.size(); i++)
		{
			ASSERTION(!isInStack(cards[i]));
			m_cardStack.push(cards[i]);
		}
	}

	void CardStack::removeFromStack(Card p_card)
	{
		std::stack<Card> tempStack = std::stack<Card>();
		while (!m_cardStack.empty())
		{
			Card card = m_cardStack.top();
			m_cardStack.pop();

			if (card != p_card) {
				tempStack.push(card);
			}
		}

		while (!tempStack.empty())
		{
			Card card = tempStack.top();
			tempStack.pop();

			m_cardStack.push(card);
		}

	}

	void CardStack::putInTheStack(Card p_card)
	{
		if (!isInStack(p_card)) {
			m_cardStack.push(p_card);
			shuffle();
		}
		
	}

	void CardStack::shuffle()
	{
		vector<Card> cards;
		while (!m_cardStack.empty())
		{
			Card card = m_cardStack.top();
			m_cardStack.pop();

			cards.push_back(card);
		}

		std::random_shuffle(cards.begin(), cards.end());

		for (int i = 0; i < cards.size(); i++)
		{
			m_cardStack.push(cards[i]);
		}

	}

	bool CardStack::isInStack(Card p_card)
	{
		bool inStack = false;

		std::stack<Card> tempStack = std::stack<Card>();
		while (!m_cardStack.empty())
		{
			Card card = m_cardStack.top();
			m_cardStack.pop();

			if (card == p_card) {
				inStack = true;
			}
			tempStack.push(card);
		}

		while (!tempStack.empty())
		{
			Card card = tempStack.top();
			tempStack.pop();

			m_cardStack.push(card);
		}


		return inStack;
	}



	

}
