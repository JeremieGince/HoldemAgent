#include "MCTS.h"


using namespace std;


namespace game {


	MCTS::MCTS(std::string p_name) : Player(p_name, "MCTS")
	{
	}


	Action MCTS::getAction(GameState p_gameState, vector<ActionType> p_possibleActions)
	{
		create_game_from_state(p_gameState);
		for (int i = 0; i < 10000; i++) {
			set_game(p_gameState);
			simulation_game->doHand(false);
			Node* last_Node = manage_leafs(&p_gameState);
			backpropagation_of_probabilities(last_Node);


		}
		float max_success_rate = 0;
		int iter_of_optimal_choice = -1;
		for (int i = 0; i < m_tree->root->children.size(); i++)
		{
			float success_rate = 0.0f;
			if ((m_tree->root->children[i]->nb_visits) > 0) {
				success_rate = (m_tree->root->children[i]->nb_achievements) / (m_tree->root->children[i]->nb_visits);
			}
			if (success_rate > max_success_rate)
			{
				iter_of_optimal_choice = i;
			}


		}
		//iter_of_optimal_choice = -1;
		last_decision_node = m_tree->root->children[iter_of_optimal_choice];
		return *(last_decision_node->decision);
	}
	Node* MCTS::manage_leafs(GameState* p_gamestate) {

		Node* last_Node = new Node();
		if (!(m_tree->slave_last_choice->children.empty()))
		{
			bool not_found = true;
			for (int i = 0; i < m_tree->slave_last_choice->children.size(); i++)
				if (m_tree->slave_last_choice->children[i]->gameState->board == p_gamestate->board)
				{
					last_Node = m_tree->slave_last_choice->children[i];
					not_found = false;
				}

			if (not_found)
			{
				last_Node->parent = m_tree->slave_last_choice;
				last_Node->is_leaf = true;
				m_tree->slave_last_choice->children.push_back(last_Node);

			}

		}
		else
		{
			last_Node->parent = m_tree->slave_last_choice;
			last_Node->is_leaf = true;
			m_tree->slave_last_choice->children.push_back(last_Node);
		}
		return last_Node;

	}

	void MCTS::backpropagation_of_probabilities(Node* final_Node)
	{
		bool success = is_decision_sccessful();
		Node* current_Node = final_Node;
		while (current_Node->is_not_root)
		{
			if (success)
			{
				current_Node->nb_achievements++;
				current_Node->parent->nb_achievements++;
			}
			current_Node->nb_visits++;
			current_Node->parent->nb_visits++;
			current_Node = current_Node->parent->parent;
		}
		if (success)
		{
			current_Node->nb_achievements++;
		}
		current_Node->nb_visits++;

	}
	bool MCTS::is_decision_sccessful()
	{
		GameState* finishing_state = new GameState();
		*finishing_state = simulation_game->getState();
		if ((*finishing_state).winnerIdx == m_playerIdx)
		{
			return true;
		}
		else if ((!(finishing_state->players[m_playerIdx]->m_playerState.active)) && (finishing_state->expectedWinnerIdx != m_playerIdx))
		{
			return true;
		}
		else
		{
			return false;
		}

	}


	void MCTS::set_game(GameState p_gameState)
	{
		vector<Card> new_board = vector<Card>();
		for (int i = 0; i < p_gameState.board.size(); i++)
			new_board.push_back(*((p_gameState.board[i])).copy());
		vector<Card> hand_cards = vector<Card>();
		for (int i = 0; i < getCards().size(); i++)
			hand_cards.push_back(*((getCards()[i])).copy());
		map<int, vector<Card>> card_set = map<int, vector<Card>>({ {-1, new_board}, {m_playerIdx, hand_cards} });
		simulation_game->setStartingCards(card_set);
	}

	void MCTS::create_game_from_state(GameState p_gameState)
	{
		vector<Player*> simulation_players = regenarate_players(p_gameState.players);
		simulation_game = new TexasHoldemGame(simulation_players, 10);
	}
	vector<Player*> MCTS::regenarate_players(vector<Player*> real_players)
	{
		vector<Player*> simulated_players;
		for (int i = 0; i < real_players.size(); i++)
		{
			if (real_players[i]->getMethod() == "MCTS")
			{
				MCTS_dummy_player* slave_player = new MCTS_dummy_player(m_tree, real_players[i]->getName());
				simulated_players.push_back(slave_player);

			}
			else if (real_players[i]->getMethod() == "Base")
			{
				BaseAgent* base_player = new BaseAgent(real_players[i]->getName());
				simulated_players.push_back(base_player);

			}
			else
			{
				Player* player = new Player(real_players[i]->getName(), real_players[i]->getMethod());
				simulated_players.push_back(player);
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
		cout << getTreeAsString(m_tree);
	}
	void MCTS::reset() {
		delete m_tree;
		m_tree = new Tree();
	}

}