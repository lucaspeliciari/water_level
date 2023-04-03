/* 
    WATER LEVEL - 31/03/2023
    TODO use something better than just a cmd or powershell terminal, like ncurses
    TODO look into: WINDOW * win = newwin(8,15,1,1)
    TODO faster flow when greater height difference

    g++ main.cpp -o water -lncurses
    To compile and run with a single code:
        alias runcode='g++ main.cpp -o water -lncurses;./water'
*/



#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
using namespace std;

using namespace std::this_thread;     // sleep_for
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


const int WIDTH = 50;
const int HEIGHT = 20;
const int MAX_STEPS = 1000;
const float WATER_LEVEL_CHANGE = 2;
const int HORIZONTAL_OFFSET = 2;
const int VERTICAL_OFFSET = 1;
const int DECIMALS_WATER_HEIGHT = 2;
int step = 0;

bool leftIsLower[WIDTH] = {false};
bool rightIsLower[WIDTH] = {false};

int groundLevel[WIDTH] = {
    10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2
}; 

int waterLevel[WIDTH] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 10, 10, 0, 0, 2, 0, 3, 0, 0, 15, 4, 4, 3, 7, 1, 1, 1, 1, 1, 19
};


void Physics();
void Draw();
int MaxWaterHeight();
int WaterVolume();
bool Levelled();


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

    int initialWaterVolume = WaterVolume();
    while (true)
    {
        Draw();
        Physics();

        if (!Levelled() || step == 0)
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


// TODO bug: thinks water is level if there is only a small height difference
// TODO bug: water gets "stuck" sometimes
// TODO bug: acts like there are invisible barriers sometimes
void Physics()
{
    for (int i = 0; i < WIDTH; i++)
    {   
        leftIsLower[i] = false;
        rightIsLower[i] = false;
    }

    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] < WATER_LEVEL_CHANGE) continue;

        leftIsLower[i] = false;
        rightIsLower[i] = false;

        int currentHeight = groundLevel[i] + waterLevel[i];
        int leftHeight = -1;
        int rightHeight = -1;
        if (i > 0) leftHeight = groundLevel[i-1] + waterLevel[i-1];
        if (i < WIDTH) rightHeight = groundLevel[i+1] + waterLevel[i+1];

        if (leftHeight > -1 && currentHeight > leftHeight) leftIsLower[i] = true;
        if (rightHeight > -1 && currentHeight > rightHeight) rightIsLower[i] = true;

        if (leftIsLower[i] && rightIsLower[i])
        {
            waterLevel[i] -= WATER_LEVEL_CHANGE;
            waterLevel[i-1] += WATER_LEVEL_CHANGE / 2;
            waterLevel[i+1] += WATER_LEVEL_CHANGE / 2;
        }
        else if (leftIsLower[i])
        {
            waterLevel[i] -= WATER_LEVEL_CHANGE;
            waterLevel[i-1] += WATER_LEVEL_CHANGE;
        }
        else if (rightIsLower[i])
        {
            waterLevel[i] -= WATER_LEVEL_CHANGE;
            waterLevel[i+1] += WATER_LEVEL_CHANGE;
        }
        else continue;
    }
}

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
            /*if (j < DECIMALS_WATER_HEIGHT+2)
            {
                charToPrint = to_string(int(waterLevel[i]))[j];
                if (colorPairIndex == 2) colorPairIndex = 3;
                else if (colorPairIndex == 1) colorPairIndex = 4;
            }*/
            
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
    printw("Max water height: %i\tWater volume: %i", MaxWaterHeight(), WaterVolume());
    move(HEIGHT+5+DECIMALS_WATER_HEIGHT+2, HORIZONTAL_OFFSET);
    printw("%i + %i = %i", groundLevel[15], waterLevel[15], groundLevel[15] + waterLevel[15]);
    refresh();
}

int MaxTotalHeight()
{
    int maxTotalHeight = 0;
    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] + groundLevel[i] > maxTotalHeight) maxTotalHeight = waterLevel[i] + groundLevel[i];
    }
    return maxTotalHeight;
}

int MaxWaterHeight()
{
    int maxWaterHeight = 0;
    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] > maxWaterHeight) maxWaterHeight = waterLevel[i];
    }
    return maxWaterHeight;
}

int WaterVolume()
{
    int waterVolume = 0;
    for (int i = 0; i < WIDTH; i++)
    {
        waterVolume += waterLevel[i];
    }
    return waterVolume;
}

// return true if water is perfectly levelled (what should happen after an infinite amount of time)
bool Levelled()
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (leftIsLower[i] || rightIsLower[i]) return false;
    }
    return true;
}
