/* 
    WATER LEVEL - 31/03/2023
    TODO use something better than just a cmd or powershell terminal, like ncurses
    TODO look into: WINDOW * win = newwin(8,15,1,1)

    g++ main.cpp -o water -lncurses
*/



#include <ncurses.h>
#include <iostream>
// #include <unistd.h>
#include <chrono>
#include <thread>
#include <cmath>
using namespace std;

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


const int WIDTH = 20;
const int HEIGHT = 20;
const int MAX_STEPS = 1000;
const float WATER_LEVEL_CHANGE = 0.1;
const int HORIZONTAL_OFFSET = 2;
const int VERTICAL_OFFSET = 1;
const int DECIMALS_WATER_HEIGHT = 2;
int step = 0;

bool leftIsLower[WIDTH] = {0};
bool rightIsLower[WIDTH] = {0};

float groundLevel[WIDTH] = {
    10,9,8,7,6,5,4,3,2,1,0,0,0,0,0
}; 

float waterLevel[WIDTH] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0
};


void Physics();
void DrawVertical();
void Draw();
float MaxWaterHeight();
float WaterVolume();
bool Levelled();


int main(int argc, char const *argv[])
{
    initscr();
    cbreak();
    noecho(); 

    if(has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
    }

    float initialWaterVolume = WaterVolume();
    while (true)
    {
        Draw();
        if (step != 1) Physics();

        if (!Levelled() || step == 1)
        {
            sleep_for(20ms);
        }
        else
        {
            erase();
            cout << "Water is levelled";
            break;
        }
        step++;
    }

    getch();
    endwin();
    return 0;
}


// TODO bug: thinks water is level if there is only a small height difference
void Physics()
{
    for (int i = 0; i < WIDTH; i++)
    {   
        leftIsLower[i] = false;
        rightIsLower[i] = false;
    }

    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] <= 0) continue;

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

        waterLevel[i] = std::round(waterLevel[i] * 100) / 100;
        waterLevel[i-1] = std::round(waterLevel[i-1] * 100) / 100;
        waterLevel[i+1] = std::round(waterLevel[i+1] * 100) / 100;
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
            char toPrint = ' ';
            int colorPairIndex = 0;
            int x = i + 2;
            int y = HEIGHT - j + 2;
            if (j < groundLevel[i]) 
            {
                toPrint = 'G';
                colorPairIndex = 2;
            }
            else if (j >= groundLevel[i] && j < waterLevel[i] + groundLevel[i] - 1)
            {
                toPrint = 'W';
                colorPairIndex = 1;
            }
            attrset(COLOR_PAIR(colorPairIndex));
            mvaddch(y, x, toPrint);
            attroff(COLOR_PAIR(colorPairIndex));
        }
    }

    // draw water hight with 2 decimals
    for (int i = 0; i < WIDTH; i++)
    {
        attrset(COLOR_PAIR(0));
        string heightString = to_string(waterLevel[i]);
        mvaddch(HEIGHT + 2, i + HORIZONTAL_OFFSET, heightString[0]);
        mvaddch(HEIGHT + 2 + 1, i + HORIZONTAL_OFFSET, '.');
        for (int j = 2; j <= DECIMALS_WATER_HEIGHT+2; j++)
        {
            mvaddch(HEIGHT + 2 + j, i + HORIZONTAL_OFFSET, heightString[j]);
        }
        attroff(COLOR_PAIR(0));
    }

    move(HEIGHT+4+DECIMALS_WATER_HEIGHT+2, HORIZONTAL_OFFSET);
    printw("Max water height: %f\tWater volume: %f", MaxWaterHeight(), WaterVolume());
    refresh();
}

float MaxTotalHeight()
{
    float maxTotalHeight = 0.0;
    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] + groundLevel[i] > maxTotalHeight) maxTotalHeight = waterLevel[i] + groundLevel[i];
    }
    return maxTotalHeight;
}

float MaxWaterHeight()
{
    float maxWaterHeight = 0.0;
    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] > maxWaterHeight) maxWaterHeight = waterLevel[i];
    }
    return maxWaterHeight;
}

float WaterVolume()
{
    float waterVolume = 0;
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
