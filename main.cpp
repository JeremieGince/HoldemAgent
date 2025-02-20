// System includes
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <chrono>

// CUDA runtime
#include <cuda.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

// Project includes
#include <Card.h>
#include <player.h>
#include "BaseAgent.h"
#include <TexasHoldemGame.h>
#include "MCTS/MCTS.h"

using namespace game;
using namespace std;



void checkHands() {
	Player player_rn1 = Player("rn1", "random");
	BaseAgent player_base0 = BaseAgent("base0");

	vector<Player*> players{ &player_base0, &player_rn1 };
	TexasHoldemGame game = TexasHoldemGame(players, 10);

	vector<Card> dummyRoyalFlush{ Card(HEART, TEN), Card(HEART, JACK) , Card(HEART, QUEEN) , Card(HEART, KING) , Card(HEART, ACE) };
	vector<Card> dummyStraightFlush{ Card(HEART, TWO), Card(HEART, THREE) , Card(HEART, FOUR) , Card(HEART, FIVE) , Card(HEART, ACE) };
	vector<Card> dummyFourOfKind{ Card(HEART, TEN), Card(CLUBS, TEN) , Card(DIAMONDS, TEN) , Card(SPADES, TEN) , Card(HEART, ACE) };
	vector<Card> dummyFullHouse{ Card(HEART, TEN), Card(CLUBS, TEN) , Card(DIAMONDS, TEN) , Card(SPADES, ACE) , Card(HEART, ACE) };
	vector<Card> dummyFlush{ Card(HEART, TEN), Card(HEART, JACK) , Card(HEART, QUEEN) , Card(HEART, KING) , Card(HEART, ACE) };
	vector<Card> dummyStraight{ Card(HEART, TEN), Card(DIAMONDS, JACK) , Card(SPADES, QUEEN) , Card(HEART, KING) , Card(CLUBS, ACE) };
	vector<Card> dummyThreeOfKind{ Card(HEART, TEN), Card(SPADES, TEN) , Card(CLUBS, TEN) , Card(HEART, KING) , Card(HEART, ACE) };
	vector<Card> dummyTwoPair{ Card(HEART, TEN), Card(CLUBS, TEN) , Card(HEART, QUEEN) , Card(HEART, KING) , Card(DIAMONDS, KING) };
	vector<Card> dummyPair{ Card(SPADES, JACK), Card(HEART, JACK) , Card(HEART, QUEEN) , Card(DIAMONDS, KING) , Card(CLUBS, ACE) };

	if (game.checkRoyalFlush(dummyRoyalFlush)) {
		cout << "RoyalFlush Done \n";
	}
	else {
		cout << "RoyalFlush Failed \n";
	}
	if (game.checkStraightFlush(dummyStraightFlush)) {
		cout << "checkStraightFlush Done \n";
	}
	else {
		cout << "checkStraightFlush Failed \n";
	}
	if (game.checkFourOfAKind(dummyFourOfKind)) {
		cout << "checkFourOfAKind Done \n";
	}
	else {
		cout << "checkFourOfAKind Failed \n";
	}
	if (game.checkFullHouse(dummyFullHouse)) {
		cout << "checkFullHouse Done \n";
	}
	else {
		cout << "checkFullHouse Failed \n";
	}
	if (game.checkFlush(dummyFlush)) {
		cout << "checkFlush Done \n";
	}
	else {
		cout << "checkFlush Failed \n";
	}
	if (game.checkStraight(dummyStraight)) {
		cout << "checkStraight Done \n";
	}
	else {
		cout << "checkStraight Failed \n";
	}
	if (game.checkThreeOfAKind(dummyThreeOfKind)) {
		cout << "checkThreeOfAKind Done \n";
	}
	else {
		cout << "checkThreeOfAKind Failed \n";
	}
	if (game.checkTwoPair(dummyTwoPair)) {
		cout << "checkTwoPair Done \n";
	}
	else {
		cout << "checkTwoPair Failed \n";
	}
	if (game.checkPair(dummyPair)) {
		cout << "checkPair Done \n";
	}
	else {
		cout << "checkPair Failed \n";
	}
	cout << endl;


	cout <<
		"Royal Flush offset: " + to_string(ROYALFLUSHoffset) << endl
		<< "Straight Flush offset: " + to_string(STRAIGHTFLUSHoffset) << endl
		<< "Four of a kind offset: " + to_string(FOUROFAKINDoffset) << endl
		<< "Full house ofsset: " + to_string(FULLHOUSEoffset) << endl
		<< "Flush offset: " + to_string(FLUSHoffset) << endl
		<< "Straight offset: " + to_string(STRAIGHToffset) << endl
		<< "Three of a kind offset: " + to_string(THREEOFAKINDoffset) << endl
		<< "Two pair offset: " + to_string(TWOPAIRoffset) << endl 
		<< "Pair offset: " + to_string(PAIRoffset) << endl
		<< "Hight Card offset: " + to_string(HIGHCARDoffset) << endl
		<< endl;

}

int main(void) {

	//checkHands();
	//main_test_cuda();

	//vector<int> hmSimulationsVector = { 10, 100, 1000, 10000, 100000};
	//vector<int> hmSimulationsVector = { 10, 100, 1000 };
	vector<int> hmSimulationsVector = { 100000 };
	for each (int hmSimulations in hmSimulationsVector)
	{
		Player player_rn0 = Player("rn0", "random");
		//Player player_rn1 = Player("rn1", "random");
		//Player player_rn2 = Player("rn2", "random");
		//Player player_rn3 = Player("rn3", "random");
		BaseAgent player_base0 = BaseAgent("base0");
		//BaseAgent player_base1 = BaseAgent("base1");
		//BaseAgent player_base2 = BaseAgent("base2");
		//BaseAgent player_base3 = BaseAgent("base3");
		MCTS mcts_agent = MCTS("mcts", hmSimulations);

		//vector<Player*> players{ &player_rn0, &player_rn1, &player_rn2, &player_rn3, &player_base0, &player_base1, &player_base2, &player_base3 };
		//vector<Player*> players{&mcts_agent, &player_base0 };
		//vector<Player*> players{ &mcts_agent, &player_base0, &player_rn0 };
		//vector<Player*> players{ &mcts_agent, &player_rn0 };
		vector<Player*> players{ &player_base0, &player_rn0 };
		//vector<Player*> players{ &player_base0, &player_base1, &player_base2 };


		TexasHoldemGame game = TexasHoldemGame(players, 10);

		auto startTime = chrono::steady_clock::now();

		std::cout << endl << "hm simulations: " + to_string(hmSimulations) << endl;

		game.start();

		game.doHands(false, true,  1000, "TH-base-rn");

		std::cout << game.getWinsStatsAsString() << endl;

		auto endTime = chrono::steady_clock::now();

		std::cout << "--- Elapse time: " << to_string(chrono::duration_cast<chrono::seconds>(endTime - startTime).count()) << " [s] ---" << endl;
	}
	

	int f;
	//std::cin >> f;

	return 0;
}