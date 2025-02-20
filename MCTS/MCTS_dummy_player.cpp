#include "MCTS_dummy_player.h"


using namespace std;


namespace game {


	MCTS_dummy_player::MCTS_dummy_player(Tree* p_tree, string p_name) : Player(p_name, "MCTS"), m_tree(p_tree), actual_Node(p_tree->root)
	{
	}


	Action MCTS_dummy_player::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions)
	{
		int iter = 0;
		float maximiser = 0;
		float value;
		Action chosen_action;
		create_new_Node(p_gameState);
		update_decisions_of_Node(p_possibleActions);
	    vector<DecisionNode*>* possible_decisions = &(actual_Node->children);
		for (int i = 0; i < possible_decisions->size(); i++)
		{	
			float nb_win = (*possible_decisions)[i]->nb_achievements;
			float nb_visits = (*possible_decisions)[i]->nb_visits;

			if (nb_visits > 0.0f) {
				value = (nb_win / nb_visits) + sqrt(2 * log((actual_Node->nb_visits)+1)/ nb_visits);
			}
			else {
				value = 99999999999.0f;
			}
			if (value > maximiser)
			{
				maximiser = value;
				iter = i;
			}
		}
		chosen_action = *((*possible_decisions)[iter]->decision);
		m_tree->slave_last_choice = (*possible_decisions)[iter];
		actual_DecisionNode = (*possible_decisions)[iter];
		return chosen_action;
	}

	void MCTS_dummy_player::create_new_Node(GameState p_gameState)
	{
		if (!(actual_DecisionNode->children.empty()))
		{
			bool not_found = true;
			std::vector<Node*> children = actual_DecisionNode->children;
			for (int i = 0; i < actual_DecisionNode->children.size(); i++)
				if ((actual_DecisionNode->children)[i]->gameState->board == p_gameState.board)
				{
					actual_Node = (actual_DecisionNode->children)[i];
					not_found = false;
				}

			if (not_found)
			{
				Node* new_node = new Node();
				*(new_node->gameState) = p_gameState;
				new_node->parent = actual_DecisionNode;
				(actual_DecisionNode->children).push_back(new_node);
				actual_Node = new_node;

			}

		}
		else
		{
			
			if (!root_initialised) 
			{
				m_tree->root->is_not_root = false;
				*(m_tree->root->gameState) = p_gameState;
				root_initialised = true;
				first_gameState = false;

			}
			else if (first_gameState) {
				first_gameState = false;
			}
			else {
				Node* new_node = new Node();
				*(new_node->gameState) = p_gameState;
				new_node->parent = actual_DecisionNode;
				(actual_DecisionNode->children).push_back(new_node);
				actual_Node = new_node;
			}

		}
		
	}
	void MCTS_dummy_player::update_decisions_of_Node(std::vector<ActionType> p_possible_actions)
	{
		if ((actual_Node->children).empty())
		{
			for (int i = 0; i < p_possible_actions.size(); i++)
			{
				Action* possible_action = new Action();
				possible_action->actionType = p_possible_actions[i];
				DecisionNode* new_decision_node = new DecisionNode();
				new_decision_node->parent = actual_Node;
				new_decision_node->decision = possible_action;
				(actual_Node->children).push_back(new_decision_node);

			}

		}


	}
	void MCTS_dummy_player::reset()
	{
		actual_Node = m_tree->root;
		DecisionNode* actual_Decision_Node = new DecisionNode();
		actual_DecisionNode = actual_Decision_Node;
		first_gameState = true;
	}

}