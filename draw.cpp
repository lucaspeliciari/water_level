#include <ncurses.h>
#include <string>
#include "checks.h"

using namespace std;


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
            else if (j*1000 >= groundLevel[i] && j*1000 < waterLevel[i] + groundLevel[i] - 1) colorPairIndex = 1;

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

            attrset(COLOR_PAIR(0));
            if (i < 10)
                mvaddch(height+3 , i+1+horizontal_offset, '0'+i);
            else
            {
                string str = to_string(i);
                mvaddch(height+3 , i+1+horizontal_offset, str[0]);
                mvaddch(height+4 , i+1+horizontal_offset, str[1]);
            }
            attroff(COLOR_PAIR(0));
        }

        attrset(COLOR_PAIR(0));
        if (j < 10)
            mvaddch(height+2-j , width+3, '0'+j);
        else
        {
            string str = to_string(j);
            mvaddch(height+2-j , width+1+horizontal_offset, str[0]);
            mvaddch(height+2-j , width+2+horizontal_offset, str[1]);
        }
        attroff(COLOR_PAIR(0));
    }

    move(height+4+decimals_water_height+2, horizontal_offset);
    printw("Max water height: %i\tWater volume: %i", MaxWaterHeight(width, waterLevel, groundLevel), WaterVolume(width, waterLevel, groundLevel));
    refresh();
}