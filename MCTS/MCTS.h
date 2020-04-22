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
#include "MCTS_dummy_player.h"
#include "BaseAgent.h"




namespace game {

    class MCTS : public Player {
    private:
        Tree m_tree;
        TexasHoldemGame* simulation_game;
        DecisionNode* last_decision_node;


        void backpropagation_of_probabilities(Node* final_Node);
        void create_game_from_state(GameState p_gameState);
        std::vector<Player*> regenarate_players(std::vector<Player*> real_players);
        void initialise_tree(GameState p_gameState);
        void set_game(GameState p_gameState);
        void simulate_game();
        bool is_decision_sccessful();

        

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
