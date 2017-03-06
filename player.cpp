#include "player.hpp"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

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
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
      
    board.doMove(opponentsMove, otherSide);
    
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
