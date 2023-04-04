/* 
    WATER LEVEL - 31/03/2023
    TODO use something better than just a cmd or powershell terminal, like ncurses
    TODO look into: WINDOW * win = newwin(8,15,1,1)
    TODO faster flow when greater height difference

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


using namespace std;
using namespace std::this_thread;     // sleep_for
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


const int WIDTH = 50;
const int HEIGHT = 20;
const int MAX_STEPS = 1000;
const float WATER_FLOW = 2;
const int HORIZONTAL_OFFSET = 2;
const int VERTICAL_OFFSET = 1;
const int DECIMALS_WATER_HEIGHT = 2;
int step = 0;

bool leftIsLower[WIDTH] = {false};
bool rightIsLower[WIDTH] = {false};

int groundLevel[WIDTH] = {
    10,9,8,7,6,5,4,3,2,1,0,5,0,0,1,0,0,0,0,0,3,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,14,0,0,1,0,2,0,3,17
}; 

int waterLevel[WIDTH] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 0, 0, 2, 0, 3, 0, 0, 15, 4, 4, 3, 7, 1, 1, 1, 1, 1, 19
};


// void Physics();
// void Draw();


int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho();

    for (int i = 0; i < WIDTH; i++)
    {
        waterLevel[i] = waterLevel[i] * 1000;
        groundLevel[i] = groundLevel[i] * 1000;
    }
    

    // set up colors if terminal supports them
    if(has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
        init_pair(3, COLOR_BLUE, COLOR_WHITE);
        init_pair(4, COLOR_WHITE, COLOR_BLUE);
    }

    int initialWaterVolume = WaterVolume(WIDTH, waterLevel, groundLevel);
    while (true)
    {
        Draw( WIDTH,  HEIGHT,  HORIZONTAL_OFFSET,  step,  DECIMALS_WATER_HEIGHT,  waterLevel, groundLevel, leftIsLower, rightIsLower);
        Physics(WIDTH, waterLevel, groundLevel, leftIsLower, rightIsLower, WATER_FLOW);

        if (!Levelled(WIDTH, leftIsLower, rightIsLower) || step == 0)
            sleep_for(1ms);
        else
        {
            move(HEIGHT+4+DECIMALS_WATER_HEIGHT+3, HORIZONTAL_OFFSET);
            printw("### Water is levelled ###");
            getch();
            break;
        }
        step++;
    }

    getch();
    endwin();
    return 0;
}
/*
void Draw()
{
    erase();
    move(1, HORIZONTAL_OFFSET);
    printw("WATER LEVEL SIMULATION");
    move(2, HORIZONTAL_OFFSET);
    printw("Step: %i", step);

    // draw water and ground
    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            char charToPrint = ' ';
            int colorPairIndex = 0;
            int x = i + 2;
            int y = HEIGHT - j + 2;

            if (j*1000 < groundLevel[i])  colorPairIndex = 2;
            else if (j*1000 >= groundLevel[i] && j*1000 < waterLevel[i] + groundLevel[i] - 1) colorPairIndex = 1;

            // This works but it's very ugly
            // if (j < DECIMALS_WATER_HEIGHT+2)
            // {
            //     charToPrint = to_string(int(waterLevel[i]))[j];
            //     if (colorPairIndex == 2) colorPairIndex = 3;
            //     else if (colorPairIndex == 1) colorPairIndex = 4;
            // }

            if (j == 0)
            {
                if (leftIsLower[i] && rightIsLower[i]) charToPrint = '|';
                else if (leftIsLower[i]) charToPrint = '<';
                else if (rightIsLower[i]) charToPrint = '>';
                if (colorPairIndex == 2) colorPairIndex = 3;
                else if (colorPairIndex == 1) colorPairIndex = 4;
            }

            attrset(COLOR_PAIR(colorPairIndex));
            mvaddch(y, x, charToPrint);
            attroff(COLOR_PAIR(colorPairIndex));
        }
    }

    move(HEIGHT+4+DECIMALS_WATER_HEIGHT+2, HORIZONTAL_OFFSET);
    printw("Max water height: %i\tWater volume: %i", MaxWaterHeight(WIDTH, waterLevel, groundLevel), WaterVolume(WIDTH, waterLevel, groundLevel));
    move(HEIGHT+5+DECIMALS_WATER_HEIGHT+2, HORIZONTAL_OFFSET);
    printw("%i + %i = %i", groundLevel[15], waterLevel[15], groundLevel[15] + waterLevel[15]);
    refresh();
}
*/