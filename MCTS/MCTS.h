/**
 *\file MCTS.h
 * \brief Prototype od the card class
 * \author Jérémie Gince (111 224 046)
 */

#ifndef MCTS_H
#define MCTS_H



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
    };

    struct DecisionNode {
        Action decision;
        float nb_visits = 0;
        float nb_achievements = 0;
        Node* parent;
        std::vector<Node*> children;
        bool is_leaf =false;
    };
    
    struct Tree {
        Node root;
    };


    class MCTS : public Player {
    private:
        Tree m_tree;
        TexasHoldemGame simulation_game;


        void backpropagation_of_probabilities();
        void create_game_from_state(GameState p_gameState);
        void simulate_game();
        void get_info_from_game();

        

    public:
        MCTS(std::string p_name);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
        std::string getNodeAsSTring(Node* p_node);
        std::string getDecisionNodeAsString(DecisionNode* p_dNode);
        std::string getTreeAsString(Tree* p_tree);
        void displayTree();
    };

}







#endif // MCTS_H
