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
        GameState* gameState = new GameState();
        float nb_visits = 0;
        float nb_achievements = 0;
        DecisionNode* parent;
        std::vector<DecisionNode*> children = std::vector<DecisionNode*>();
        bool is_leaf = false;
        bool is_not_root = true;

        ~Node() {
            delete gameState;
            for (int i = 0; i < children.size(); i++)
            {
                delete children[i];
            }
        }

    };

    struct DecisionNode {
        Action* decision = new Action();
        float nb_visits = 0;
        float nb_achievements = 0;
        Node* parent = new Node();
        std::vector<Node*> children = std::vector<Node*>();

        ~DecisionNode() {
            delete decision;
            for (int i = 0; i < children.size(); i++)
            {
                delete children[i];
            }
        }
    };

    struct Tree {
        Node* root = new Node();
        DecisionNode* slave_last_choice = new DecisionNode();

        ~Tree() {
            delete root;
        }
    };

    class MCTS_dummy_player : public Player {
    private:
        Tree* m_tree = new Tree();
        Node* actual_Node = new Node();
        DecisionNode* actual_DecisionNode = new DecisionNode();
        DecisionNode* last_decision_node_of_simulation;
        bool root_initialised = false;
        bool first_gameState = true;

    public:
        MCTS_dummy_player(Tree* p_tree, std::string p_name);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
        void update_decisions_of_Node(std::vector<ActionType> p_possible_actions);
        void create_new_Node(GameState p_gameState);
        virtual void reset();

    };

}







#endif // MCTS_DUMMY_PLAYER_H