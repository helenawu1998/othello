#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
#include <vector>

using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);
    
    vector<Move> moves;
    
    static const int VERYGOOD = 10;
    static const int GOOD = 5;
    static const int VERYBAD = -100000;
    static const int BAD = -10;
    static const int NORMAL = 0;
    int scores[8][8] = {{VERYGOOD, BAD, GOOD, GOOD, GOOD, GOOD, BAD, VERYGOOD},
               {BAD, VERYBAD, NORMAL, NORMAL, NORMAL, NORMAL, VERYBAD, BAD},
               {GOOD, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, GOOD},
               {GOOD, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, GOOD},
               {GOOD, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, GOOD},
               {GOOD, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, GOOD},
               {BAD, VERYBAD, NORMAL, NORMAL, NORMAL, NORMAL, VERYBAD, BAD},
               {VERYGOOD, BAD, GOOD, GOOD, GOOD, GOOD, BAD, VERYGOOD}};
};

#endif
