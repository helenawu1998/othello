#include "player.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     this->side = side;
     if (side == WHITE)
     {
         otherSide = BLACK;
     }
     else
     {
         otherSide = WHITE;
     }
     
     board = Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    
    board.doMove(opponentsMove, otherSide);
    
    //return doRandomMove();
    //return doHeuristicMove();
    
    // Minimax + Heuristics Implementation
	return doMinimaxMove(opponentsMove);
}

/*
 * Returns a random valid next move.
 */
Move *Player::doRandomMove() 
{
    vector<Move*> moves;
       
    if (board.hasMoves(side))
    {
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                Move *m = new Move(x, y);
                if (board.checkMove(m, side))
                {
                    moves.push_back(m);
                }
            }
        }
        
        srand(time(NULL));
        int i = rand() % moves.size();
        
        board.doMove(moves[i], side);
        return moves[i];
    }
   
    return nullptr;
}

/*
 * Returns a valid next move using a heuristic function based on
 * position on the board and resulting difference in number of stones.
 * Enough to consistently beat SimplePlayer
 */
Move *Player::doHeuristicMove() 
{
    map<int, Move*> moves;

    if (board.hasMoves(side))
    {
        for (int x = 0; x < 8; x++)
        {
            for (int y = 0; y < 8; y++)
            {
                Move *m = new Move(x, y);
                if (board.checkMove(m, side))
                {
                    Board *b = board.copy();
                    b->doMove(m, side);
                    int diffCount = b->count(side) - b->count(otherSide);
                    delete b;
                    moves[board.scores[x][y] + diffCount] = m;
                }
            }
        }
        
        Move * best = moves.rbegin()->second;
        
        board.doMove(best, side);
        return best;
    }
   
    return nullptr;
}

/*
 * Returns the best score for a node using recursion
 */
int Player::rec_minimax(Node* n, int depth)
{
	Side played_side;
	int Max;
	if (depth % 2 == 0)
	{
		played_side = side;
		Max = INT_MIN;
	}
	else
	{
		played_side = otherSide;
		Max = INT_MAX;
	}
    
    // Base case return score
	if (depth >= DEPTH)
		return n->board->justGetScore(played_side, n->last_move);
	// Find possible moves and use recursion to find max/min
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Move *m = new Move(x, y);
			if (n->board->checkMove(m, played_side))
			{
				Node* new_n = new Node(n->board->copy(), m, played_side);
                new_n->board->doMove(m, played_side);
				Max = max(Max, -rec_minimax(new_n, depth + 1));
			}
		}
	}
	return Max;
	
}

/* Finds the next possible states for a node for 1 level down to assist with
 * the doMinimaxMove function.
 */
void Player::sim_states(Node* n, Side played_side)
{
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			 Move *m = new Move(x, y);
			 if (n->board->checkMove(m, played_side))
			 {
			 	// Create child node
			 	Node *node = new Node(n->board->copy(), m, played_side);
			 	node->board->doMove(m, played_side);
			 	int score = node->board->getScore(this->side, otherSide, played_side, m, testingMinimax);
			 	if (score < n->min_score)
					n->min_score = score;
			 	// Add node to children of the original node n
				n->children.push_back(node);
			 }
			
		}
	}
}

/* Returns a valid move based on a 2-ply decision making tree with 
 * move position and difference in stones heuristics 
 */
Move *Player::doMinimaxMove(Move* opponentsMove) {
	  if (board.hasMoves(side))
    {
		// Parent Node
        Node * n = new Node(board.copy(), opponentsMove, otherSide);
        int extreme;
        extreme = INT_MIN;
        Move* best;
        
        // 1-Ply
        sim_states(n, side);
        
        // Finding best move with max score, for even depths only
        for (unsigned int i = 0; i < n->children.size(); i++)
        {
			int s = rec_minimax(n->children[i], DEPTH);
			if (s > extreme)
			{
				extreme = s;
				best = n->children[i]->last_move;
			}
		}
        
        //-------------alphabeta attempt-------------
        
        /* 
        for (unsigned int i = 0; i < n->children.size(); i++)
        {
			int s = alphabeta(n->children[i], 0, INT_MIN, INT_MAX, false);
			if (s > extreme)
			{
				extreme = s;
				best = n->children[i]->last_move;
			}
		}
        */
        
        //-------------Old 2-ply non-recursive minimax-------------
        
        /* 
        // 2-Ply
        for (int i = 0; i < n->children.size(); i++)
        {
			sim_states(n->children[i], otherSide);
			
		}
		// Find minimax
		int max = -10000000;
		Move* best;
		for (int i = 0; i < n->children.size(); i++)
		{
			if (n->children[i]->min_score >= max)
			{
				max = n->children[i]->min_score;
				best = n->children[i]->last_move;
			}
				
		}
		*/
        
		delete n;
		
		board.doMove(best, side);
        return best;
    } 
    return nullptr;   
}

/*
 * Returns minimax score with alpha beta pruning
 */ 
int Player::alphabeta(Node *n, int depth, int alpha, int beta, bool maximizingPlayer)
{
    Side played_side;

	if (depth % 2 == 0)
	{
		played_side = side;
	}
	else
	{
		played_side = otherSide;
	}
    
    //basecase
    if (depth == DEPTH)
    {
        return n->board->justGetScore(played_side, n->last_move);
    }
    
    //making children
    for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Move *m = new Move(x, y);
			if (n->board->checkMove(m, played_side))
			{
				Node* new_n = new Node(n->board->copy(), m, played_side);
				n->children.push_back(new_n);
			}
		}
	}
    
    if (maximizingPlayer)
    {
        int v = INT_MIN;
        for (unsigned int i = 0; i < n->children.size(); i++)
        {
            v = max(v, alphabeta(n->children[i], depth + 1, alpha, beta, false));
            alpha = max(alpha, v);
            if (beta <= alpha)
            {
                break;
            }
        }
        return v;
    }
    else
    {
        int v = INT_MAX;
        for (unsigned int i = 0; i < n->children.size(); i++)
        {
            v = min(v, alphabeta(n->children[i], depth + 1, alpha, beta, true));
            beta = min(beta, v);
            if (beta <= alpha)
            {
                break;
            }
        }
        return v;
    }
}

Node::Node(Board *b, Move *m, Side s){
	min_score = 10000000;
	board = b;
	last_move = m;
	move_side = s;
}

Node::~Node(){
	delete board;
	
	for (unsigned int i = 0; i < children.size();i++)
	{
		delete children[i];
	}
	
}
