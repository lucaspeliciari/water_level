/* 
    WATER LEVEL - 31/03/2023
    TODO use something better than just a cmd or powershell terminal, like ncurses

    g++ main.cpp -o water -lncurses
*/



#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <cmath>
using namespace std;


const int WIDTH = 20;
const int HEIGHT = 20;
const int MAX_STEPS = 1000;
const float WATER_LEVEL_CHANGE = 1;
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
    WINDOW * win = newwin(8,15,1,1);
    wrefresh(win);

    float initialWaterVolume = WaterVolume();
    // while (step++ < MAX_STEPS)
    while (true)
    {
        // DrawVertical();
        Draw();
        if (step != 1) Physics();

        if (!Levelled() || step == 1)
        {
            if (step == 1) sleep(3);
            else sleep(1);
        }
        else
        {
            // cout << "Water is levelled";
            break;
        }
        step++;
    }

    // getch();  // get input, to wait until user presses key to end program
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
    erase();  // cls

    if(has_colors())
    {
        start_color();
        init_pair(1, COLOR_BLUE, COLOR_BLUE);
        init_pair(2, COLOR_WHITE, COLOR_WHITE);
    }

    printw("WATER LEVEL SIMULATION\nStep: %i", step);
    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            if (j < groundLevel[i]) 
            {
                attrset(COLOR_PAIR(2));
                mvaddch(HEIGHT-j+3, i+3,'G');
                attroff(COLOR_PAIR(2));
            }
            else if (j >= groundLevel[i] && j < waterLevel[i] + groundLevel[i] - 1)
            {
                attrset(COLOR_PAIR(1));
                mvaddch(HEIGHT-j+3, i+3,'W');
                attroff(COLOR_PAIR(1));
            }
            else 
            {
                attrset(COLOR_PAIR(3));
                mvaddch(HEIGHT-j+3, i+3,' ');
                attroff(COLOR_PAIR(3));
            }
            attrset(COLOR_PAIR(3));
            mvaddch(HEIGHT-j+3, i+3,' ');
            attroff(COLOR_PAIR(3));
        }
    }

    

    printw("Max water height: %f\tWater volume: %f", MaxWaterHeight(), WaterVolume());
    refresh();
}

float MaxWaterHeight()
{
    float maxWaterHeight = 0;
    for (int i = 0; i < WIDTH; i++)
    {
        if (waterLevel[i] > maxWaterHeight) maxWaterHeight = waterLevel[i];
    }
    return maxWaterHeight;
}

// total amount of water in simulation, for debugging
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
