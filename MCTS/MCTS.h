/**
 *\file MCTS.h
 * \brief Prototype od the card class
 * \author Jérémie Gince (111 224 046)
 */

#ifndef BASEAGENT_H
#define BASEAGENT_H



#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include "contract_exception.h"
#include "player.h"
#include "TexasHoldemGame.h"




namespace game {


    struct Node {
        GameState gameState;
        DecisionNode* parent;
        std::vector<DecisionNode*> children;
    };

    struct DecisionNode {
        Action decision;
        Node* parent;
        std::vector<Node*> children;
    };

    struct Tree {
        Node root;
        std::vector<Node*> nodes;
        std::vector<DecisionNode*> decisionNodes;
        std::vector<Node*> leaves;
    };


    class MCTS : public Player {
    private:
        Node m_root;
        std::vector<Node*> m_nodes;
        std::vector<DecisionNode*> m_decisionNodes;

        Tree m_tree;

        void generateTree(GameState p_gameState);
        void generateTree(Node p_root);
        Tree* generateTree(Tree* p_tree);

        void attachTrees(Tree* p_rootTree, Tree* p_leafTree);
        void attachTrees(Tree* p_rootTree, Tree* p_leafTree, DecisionNode* p_dNode);
        void attachTree(DecisionNode* p_dNode, Tree* p_tree);

    public:
        MCTS(std::string p_name);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);

        std::string getNodeAsSTring(Node* p_node);
        std::string getDecisionNodeAsString(DecisionNode* p_dNode);
        std::string getTreeAsString(Tree* p_tree);
        void displayTree();
    };

}







#endif // BASEAGENT_H
