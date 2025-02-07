#pragma once

#include<vector>
#include<stack>
#include <time.h>
#include "Card.h"

namespace game{

	class CardStack
	{
	private:
		std::stack<Card> m_cardStack;

	public:
		CardStack();

		Card getCard();

		void reset();

		void removeFromStack(Card p_card);

		void putInTheStack(Card p_card);

		void shuffle();

		bool isInStack(Card p_card);

	};

}


