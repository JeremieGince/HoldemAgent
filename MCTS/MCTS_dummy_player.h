/**
 *\file MCTS_dummy_player.h
 * \brief Prototype of the MCTS_dummy_player class
 * \author Samel Ouellet (111 223 977)
 */

#ifndef MCTS_DUMMY_PLAYER_H
#define MCTS_DUMMY_PLAYER_H


#include <cmath>
#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include "contract_exception.h"
#include "player.h"
#include "TexasHoldemGame.h"



namespace game {

    struct DecisionNode;

    struct Node {
        GameState gameState;
        float nb_visits = 1;
        float nb_achievements = 0;
        DecisionNode* parent;
        std::vector<DecisionNode*> children;
        bool is_leaf = false;
        bool is_not_root = true;
    };

    struct DecisionNode {
        Action decision;
        float nb_visits = 0;
        float nb_achievements = 0;
        Node* parent;
        std::vector<Node*> children;
        bool is_leaf = false;
    };

    struct Tree {
        Node root;
    };

    class MCTS_dummy_player : public Player {
    private:
        Tree* m_tree;
        Node* actual_Node;
        DecisionNode* actual_DecisionNode;

    public:
        MCTS_dummy_player(Tree* p_tree);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
        void update_decisions_of_Node(std::vector<ActionType> p_possible_actions);
        void create_new_Node(GameState p_gameState);

    };

}







#endif // MCTS_DUMMY_PLAYER_H