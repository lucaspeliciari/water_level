#include <ncurses.h>
#include <string>
#include "checks.h"

using namespace std;


void DrawHorizontalIndex(int height, int horizontal_offset, int i);
void DrawVerticalIndex(int width, int height, int horizontal_offset, int j);
void DrawDebug(int width, int horizontal_offset, int *waterLevel, int *groundLevel, bool *leftIsLower, bool* rightIsLower);


// TODO remove this gigantic amount of arguments?
void Draw(int width, int height, int horizontal_offset, int step, int decimals_water_height, int *waterLevel, int *groundLevel, bool *leftIsLower, bool *rightIsLower)
{
    erase();
    move(1, horizontal_offset);
    printw("WATER LEVEL SIMULATION");
    move(2, horizontal_offset);
    printw("Step: %i", step);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            char charToPrint = ' ';
            int colorPairIndex = 0;
            int x = i + 1+horizontal_offset;
            int y = height - j + 1+horizontal_offset;

            if (j*1000 < groundLevel[i])  colorPairIndex = 2;
            else if (j*1000 >= groundLevel[i] && j*1000 < waterLevel[i] + groundLevel[i]) colorPairIndex = 1;  

            // draw the direction of the flow (both, left only or right only)
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

            DrawHorizontalIndex(height, horizontal_offset, i);
        }

        DrawVerticalIndex(width, height, horizontal_offset, j);
    }

    move(height+4+decimals_water_height+2, horizontal_offset);
    int maxWaterHeight = MaxWaterHeight(width, waterLevel, groundLevel);
    int waterVolume = WaterVolume(width, waterLevel, groundLevel);
    printw("Max water height: %i\tWater volume: %i", maxWaterHeight, waterVolume);

    DrawDebug(width, horizontal_offset, waterLevel, groundLevel, leftIsLower, rightIsLower);

    refresh();
}


// void DrawGround()
// void DrawWater()
// void DrawParticles()


void DrawHorizontalIndex(int height, int horizontal_offset, int i)
{
    // attrset(COLOR_PAIR(0));
    if (i < 10)
        mvaddch(height+4 , i+1+horizontal_offset, '0'+i);
    else
    {
        string str = to_string(i);
        mvaddch(height+4 , i+1+horizontal_offset, str[0]);
        mvaddch(height+5 , i+1+horizontal_offset, str[1]);
    }
    // attroff(COLOR_PAIR(0));
}

void DrawVerticalIndex(int width, int height, int horizontal_offset, int j)
{
    // attrset(COLOR_PAIR(0));
    if (j < 10)
        mvaddch(height+2-j , width+3, '0'+j);
    else
    {
        string str = to_string(j);
        mvaddch(height+2-j , width+1+horizontal_offset, str[0]);
        mvaddch(height+2-j , width+2+horizontal_offset, str[1]);
    }
    // attroff(COLOR_PAIR(0));
}

void DrawDebug(int width, int horizontal_offset, int *waterLevel, int *groundLevel, bool *leftIsLower, bool* rightIsLower)
{
    for (int i = 0; i < width; i++)
    {
        string waterLevelStr = to_string(waterLevel[i]);
        string groundLevelStr = to_string(groundLevel[i]);
        move(i, width + 10 + horizontal_offset);
        // printw("Index %i\t\tWater level: %i\t\tGround level: %i\t\tL: %i\t\tR: %i", i, waterLevel[i], groundLevel[i], leftIsLower[i], rightIsLower[i]);
        printw("Index %i", i);
        move(i, width + 10 + horizontal_offset + 15);
        printw("Water level: %i", waterLevel[i]);
        move(i, width + 10 + horizontal_offset + 39);
        printw("Ground level: %i", groundLevel[i]);
        move(i, width + 10 + horizontal_offset + 64);
        printw("Flowing left: %i", leftIsLower[i]);
        move(i, width + 10 + horizontal_offset + 85);
        printw("Flowing right: %i", rightIsLower[i]);
    }

    move(30, 2);
    printw("40\tLevel: %i", waterLevel[40]+groundLevel[40]);
    move(31, 2);
    printw("41\tLevel: %i", waterLevel[41]+groundLevel[41]);
    move(32, 2);
    printw("40 > 41: %i", waterLevel[40]+groundLevel[40] > waterLevel[41]+groundLevel[41]);
}
