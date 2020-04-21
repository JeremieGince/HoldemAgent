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
#include "MCTS.h"



namespace game {


    class MCTS_dummy_player : public Player {
    private:
        Tree* m_tree;
        Node* actual_Node;
        DecisionNode* actual_DecisionNode;

    public:
        MCTS_dummy_player(Tree* p_tree);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);

    };

}







#endif // MCTS_DUMMY_PLAYER_H