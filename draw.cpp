#include <ncurses.h>
#include "checks.h"
// #include "particle.cpp"

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
            int x = i + 2;
            int y = height - j + 2;

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
        }
    }

    move(height+4+decimals_water_height+2, horizontal_offset);
    printw("Max water height: %i\tWater volume: %i", MaxWaterHeight(width, waterLevel, groundLevel), WaterVolume(width, waterLevel, groundLevel));
    refresh();
}

void DrawParticles(Particle *particles, int step)
{
    erase();
    move(1, 2);
    printw("WATER LEVEL SIMULATION");
    move(2, 2);
    printw("Step: %i", step);

    for (int i = 0; i < 100; i++)  // should be number of particles instead of 100
    {
        if (particles[i] == NULL)
            continue;
        
        attrset(COLOR_PAIR(1));
        mvaddch(particles[i].y, particles[i].x, ' ');
        attroff(COLOR_PAIR(1));
    }
}
