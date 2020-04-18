/**
 *\file QAgent.h
 * \brief Prototype od the card class
 * \author Jérémie Gince (111 224 046)
 */

#ifndef QAGENT_H
#define QAGENT_H



#include <iostream> 
#include <vector>
#include <map>
#include <string>
#include "contract_exception.h"
#include "player.h"




namespace game {
    class QAgent : public Player {
    private:
        // private

    public:
        QAgent(std::string p_name);

        virtual Action getAction(GameState p_gameState, std::vector<ActionType> p_possibleActions);
    };

}







#endif // QAGENT_H
