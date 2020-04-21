#include "MCTS.h"


using namespace std;


namespace game {


	MCTS::MCTS(std::string p_name) : Player(p_name, "MCTS") {
	}


	Action MCTS::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {

		create_game_from_state(p_gameState);
		Action chosen_action;
		for (int i = 0; i < 10000; i++) {

		}


		return chosen_action;
	}

	void MCTS::create_game_from_state(GameState p_gameState) {


	}

	std::string MCTS::getNodeAsSTring(Node* p_node)
	{
		string out = "Node: ";

		//out += to_string(p_node->);

		return out;
	}

	std::string MCTS::getDecisionNodeAsString(DecisionNode* p_dNode)
	{
		string out = "Decision Node: ";
		return out;
	}

	std::string MCTS::getTreeAsString(Tree* p_tree)
	{
		string out = "Tree: \n";
		return out;
	}

	void MCTS::displayTree()
	{
		cout << getTreeAsString(&m_tree);
	}

}