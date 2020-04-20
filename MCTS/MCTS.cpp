#include <vector>
#include<map>
#include <algorithm>
#include "MCTS.h"
#include "TexasHoldemGame.h"
#include "player.h"


using namespace std;


namespace game {


	MCTS::MCTS(std::string p_name) : Player(p_name, "MCTS") {
	}


	Action MCTS::getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions) {
		Action action;
		action.bet = 1.0;

		vector<ActionType>::iterator it;

		vector<Card> playerHand = m_cards;
		playerHand.insert(playerHand.end(), p_gameState.board.begin(), p_gameState.board.end());





		return action;
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


	void MCTS::generateTree(Node p_root) {
		m_nodes.clear();
		m_decisionNodes.clear();

		m_root = p_root;
		m_nodes.push_back(&m_root);

	}

	void MCTS::generateTree(GameState p_gameState)
	{

	}

	Tree* MCTS::generateTree(Tree* p_tree)
	{
		for (int i = 0; i < p_tree->leaves.size(); i++)
		{
			vector<ActionType> possibleActions = TexasHoldemGame::getPossibleActions(m_playerState, p_tree->leaves[i]->gameState);
			for (int j = 0; j < possibleActions.size(); j++)
			{
				DecisionNode* dNode = new DecisionNode();
				dNode->parent = p_tree->leaves[i];
				Action decision{possibleActions[i]};
				dNode->decision = decision;

				Node* node = new Node();
				GameState gameState = TexasHoldemGame::getRandomNextState(p_tree->leaves[i]->gameState, decision);
				node->parent = dNode;
				Tree* nextTree = generateTree(new Tree{ *node, vector<Node*> {node},  vector<DecisionNode*> {}, vector<Node*>{node} });
				attachTrees(p_tree, nextTree, dNode);
			}
		}

		return p_tree;
	}

	void MCTS::attachTrees(Tree* p_rootTree, Tree* p_leafTree)
	{

	}

	void MCTS::attachTrees(Tree* p_rootTree, Tree* p_leafTree, DecisionNode* p_dNode)
	{
		p_dNode->children.push_back(&p_leafTree->root);
		p_leafTree->root.parent = p_dNode;
	}

	void MCTS::attachTree(DecisionNode* p_dNode, Tree* p_tree)
	{
		p_dNode->children.push_back(&p_tree->root);
		p_tree->root.parent = p_dNode;
	}

}