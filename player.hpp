#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
using namespace std;

class Node{
	public:
		Node(Board *b, Move *m, Side s);
		~Node();
		Board* board;
		int min_score;
		Side move_side;
		Move* last_move;
		vector <Node*> children;
};

class Player {

public:
    Player(Side side);
    ~Player();

	const int DEPTH = 8;
    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doRandomMove();
    Move *doHeuristicMove(); 
    void sim_states(Node* n, Side played_side);
    int rec_minimax(Node* n, int depth);
    Move* doMinimaxMove(Move* opponentsMove);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Board board;
    Side side;
    Side otherSide;
    
};

#endif
