/**
 *\file BaseAgent.h
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




namespace game {
    class BaseAgent : public Player {
    private:
        // private

    public:
        BaseAgent(std::string p_name);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
    };

}







#endif // BASEAGENT_H