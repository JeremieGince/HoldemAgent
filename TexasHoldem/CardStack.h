#pragma once

#include<vector>
#include<stack>
# include "Card.h"

namespace game{

	class CardStack
	{
	private:
		std::stack<Card> m_cardStack;

	public:
		CardStack();

		Card getCard();

		void reset();

	};

}


