/* 
    WATER LEVEL - 31/03/2023
    TODO faster flow when greater height difference
    TODO bug where left and/or right is lower is true even though levels are all 0
    TODO bug where there's 1 drop of water balanced on top of a wall
    TODO fixed "jagged" water when it tries to stay levelled

    g++ main.cpp checks.cpp physics.cpp draw.cpp -o water -lncurses
    To compile and run with a single code:
        alias runcode='g++ main.cpp checks.cpp physics.cpp draw.cpp -o water -lncurses;./water'
*/


#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include "checks.h"
#include "physics.h"
#include "draw.h"

#include <stdlib.h>
#include <random>


using namespace std;
using namespace std::this_thread;     // sleep_for
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


const int WIDTH = 170;
const int HEIGHT = 40;
const int MAX_STEPS = 1000;
const int LITERS_PER_TILE = 100;
const float WATER_FLOW = 2;
const int HORIZONTAL_OFFSET = 2;
const int VERTICAL_OFFSET = 1;
const int DECIMALS_WATER_HEIGHT = 2;
int step = 0;

bool leftIsLower[WIDTH] = {false};
bool rightIsLower[WIDTH] = {false};

int groundLevel[WIDTH] = {
    10,9,8,7,6,5,4,3,2,1,1,5,1,1,1,1,1,1,1,1,6,0,1,1,1,1,2,1,1,1,1,1,1,1,1,7,1,1,1,1,2,0,3,17,0,0,0,0,10
}; 

int waterLevel[WIDTH] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 0, 1, 2, 0, 3, 3, 0, 15, 4, 4, 3, 7, 1, 1, 1, 1, 1, 19,0,0,0,0,10,0,0,0,0,0,0,15
};


int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho();

    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> waterUni(0, 35);
    uniform_int_distribution<int> groundUni(1, 15);
    for (int i = 0; i < WIDTH; i++)
    {
        waterLevel[i] = waterUni(rng);
        int randomGroundLevel = groundUni(rng);
        groundLevel[i] = groundUni(rng);
    }

    for (int i = 0; i < WIDTH; i++)
    {
        waterLevel[i] = waterLevel[i] * LITERS_PER_TILE;
        groundLevel[i] = groundLevel[i] * LITERS_PER_TILE;
    }

    // set up colors if terminal supports them
    if(has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
        init_pair(3, COLOR_BLUE, COLOR_WHITE);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
        init_pair(5, COLOR_RED, COLOR_RED);
    }

    int initialWaterVolume = WaterVolume(WIDTH, waterLevel, groundLevel);
    while (true)
    {
        Physics(WIDTH, waterLevel, groundLevel, leftIsLower, rightIsLower, WATER_FLOW);
        Draw( WIDTH,  HEIGHT,  HORIZONTAL_OFFSET,  step,  DECIMALS_WATER_HEIGHT,  waterLevel, groundLevel, leftIsLower, rightIsLower, LITERS_PER_TILE);
        sleep_for(1ms);
        step++;
    }

    getch();
    endwin();
    return 0;
}
