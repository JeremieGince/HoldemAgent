#include "MCTS.h"


using namespace std;


namespace game {


	MCTS::MCTS(std::string p_name) : Player(p_name, "MCTS") {
		m_tree.root.is_not_root = false;
	}


	Action MCTS::getAction(GameState p_gameState, vector<ActionType> p_possibleActions) {

		create_game_from_state(p_gameState);
		Action chosen_action;
		for (int i = 0; i < 10000; i++) {

		}

		
		return chosen_action;
	}
	void MCTS::backpropagation_of_probabilities(Node* final_Node) {
		Node* current_Node = final_Node;
		while (current_Node->is_not_root) {
			DecisionNode* parent_decision = current_Node->parent;
			current_Node->nb_achievements++;
			parent_decision->nb_achievements++;
			current_Node = parent_decision->parent;
		}

	}
	void MCTS::initialise_tree(GameState p_gameState) {
		if (!(m_tree.root.children).empty()) {
			bool game_state_found = false;
			Node* node_of_the_game_state;
			for (int i = 0; i < (last_decision_node->children).size(); i++) {
				if ((last_decision_node->children)[i]->gameState.board == p_gameState.board) {
					game_state_found = true;
					node_of_the_game_state = (last_decision_node->children)[i];
				}

			}

			if (game_state_found) {
				m_tree.root = *node_of_the_game_state;
				m_tree.root.is_not_root = false;
			}
			else {
				Node new_root;
				new_root.is_not_root = false;
				new_root.gameState = p_gameState;
				m_tree.root = new_root;
			}
		}
		else
		{
			m_tree.root.gameState = p_gameState;
		}
	}
	bool MCTS::is_decision_sccessful() {
		GameState* finishing_state = &(simulation_game.getState());
		if ((*finishing_state).winnerIdx == m_playerIdx) {
			return true;
		}
		else if ((!(finishing_state->players[m_playerIdx]->m_playerState.active)) && ((*finishing_state).expectedWinnerIdx != m_playerIdx)) {
			return true;
		}
		else
		{
			return false;
		}

	}


	void MCTS::set_game(GameState p_gameState) {
		vector<Card> board = p_gameState.board;
		vector<Card> hand_cards = getCards();
		map<int, vector<Card>> card_set = { {-1, board}, {m_playerIdx, hand_cards} };
		simulation_game.setStartingCards(card_set);
	}

	void MCTS::create_game_from_state(GameState p_gameState) {
		initialise_tree(p_gameState);
		vector<Player*> simulation_players =regenarate_players(p_gameState.players);
		simulation_game = TexasHoldemGame(simulation_players, 10);
		set_game(p_gameState);
	}
	vector<Player*> MCTS::regenarate_players(vector<Player*> real_players) {
		vector<Player*> simulated_players;
		for (int i = 0; i < real_players.size(); i++) {
			if (real_players[i]->getMethod() == "MCTS") {
				MCTS_dummy_player slave_player = MCTS_dummy_player(&m_tree);
				simulated_players.push_back(&slave_player);

			}
			if (real_players[i]->getMethod() == "Base") {
				BaseAgent base_player = BaseAgent(real_players[i]->getName());
				simulated_players.push_back(&base_player);

			}
			else
			{
				Player player = Player(real_players[i]->getName(), real_players[i]->getMethod());
				simulated_players.push_back(&player);
			}

		}

		return simulated_players;


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