#include "MCTS_dummy_player.h"


using namespace std;


namespace game {


	MCTS_dummy_player::MCTS_dummy_player(Tree* p_tree) : Player("MCTS_SLAVE", "MCTS"), m_tree(p_tree), actual_Node(&(p_tree->root)) {
	}


	Action MCTS_dummy_player::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {
		int iter;
		float maximiser = 0;
		float value;
		Action chosen_action;
	    vector<DecisionNode*>* possible_decisions = &(actual_Node->children);
		for (int i = 0; i < possible_decisions->size(); i++) {
			float nb_win = (*possible_decisions)[i]->nb_achievements;
			float nb_visits = (*possible_decisions)[i]->nb_visits;
			value = (nb_win / nb_visits) + sqrt(2 * log((actual_Node->nb_visits)) / nb_visits);
			if (value > maximiser) {
				maximiser = value;
				iter = i;
			}
		}
		chosen_action = (*possible_decisions)[iter]->decision;
		return chosen_action;
	}


}