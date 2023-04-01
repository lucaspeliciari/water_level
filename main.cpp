/* 
    WATER LEVEL - 31/03/2023
    TODO use something better than just a cmd or powershell terminal, like ncurses
*/



#include <ncurses.h>
#include <iostream>
#include <unistd.h>
// #include <windows.h> 
#include <cmath>
using namespace std;


// HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int WIDTH = 30;
const int HEIGHT = 50;
const int MAX_STEPS = 1000;
const float WATER_LEVEL_CHANGE = 0.12;
int step = 0;

bool leftIsLower[WIDTH] = {0};
bool rightIsLower[WIDTH] = {0};

float groundLevel[WIDTH] = {
    2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 1, 2, 3, 4, 5, 14
}; 

float waterLevel[WIDTH] = {
    25, 20, 4, 12, 4, 4, 7, 4, 8, 0, 3, 1, 25, 8, 7, 0, 40
};


void Physics();
void DrawVertical();
void Draw();
float MaxWaterHeight();
float WaterVolume();
bool Levelled();


int main(int argc, char const *argv[])
{
    float initialWaterVolume = WaterVolume();
    while (step++ < MAX_STEPS)
    {
        DrawVertical();
        // Draw();
        if (step != 1) Physics();

        cout << endl << endl << "Max water height: " << MaxWaterHeight() << endl << "Water volume: ";
        float currentWaterVolume = WaterVolume();
        // if (initialWaterVolume != currentWaterVolume) SetConsoleTextAttribute(hConsole, 12);
        // else SetConsoleTextAttribute(hConsole, 7);
        cout << currentWaterVolume << endl;


        if (!Levelled() || step == 1)
        {
            if (step == 1) sleep(2);
            else sleep(0.55);
        }
        else
        {
            cout << "Water is levelled";
            break;
        }

    }

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
        // bool leftIsLower = false;
        // bool rightIsLower = false;

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

// does not work
void Draw()
{
    if (system("cls")) system("clear");

    cout << "Step: " << step << endl;

    char toDraw[HEIGHT][WIDTH]; for (int j = 0; j < HEIGHT; j++) for (int i = 0; i < WIDTH; i++) toDraw[j][i] = ' ';
    
    for (int j = 0; j < HEIGHT; j++)
        for (int i = 0; i < WIDTH; i++)
            if (j < groundLevel[i]) toDraw[j][i] = '#';
            else if (j > groundLevel[i] && j < waterLevel[i] + 1 + groundLevel[i]) toDraw[j][i] = 'A';
            else toDraw[j][i] = ' ';

    for (int j = 0; j < HEIGHT; j++)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            if (toDraw[j][i] == '#')
            {
                // SetConsoleTextAttribute(hConsole, 7+7*16);
                cout << "#";
            }
            else if (toDraw[j][i] == 'A')
            {
                // SetConsoleTextAttribute(hConsole, 3+3*16);
                cout << "A";
            }
        }
        cout << endl;
        // SetConsoleTextAttribute(hConsole, 7);
    }
    cout << "||||||||||||||";
}

void DrawVertical()
{
    if (system("cls")) system("clear");

    // SetConsoleTextAttribute(hConsole, 7);
    cout << "Step: " << step << endl;
    for (int i = 0; i < WIDTH; i++)
    {
        cout << i << "\tH" << waterLevel[i] << "\t1";
        for (int j = 0; j < HEIGHT; j++)
        {
            if (j < groundLevel[i])
            {
            //    SetConsoleTextAttribute(hConsole, 7+7*16);
               cout << "#";
            } 
            else if (j > groundLevel[i] && j < waterLevel[i] + 1 + groundLevel[i]) 
            {
                // SetConsoleTextAttribute(hConsole, 3+3*16);
                cout << "A";
            }
            // SetConsoleTextAttribute(hConsole, 7);
        }
        cout << "\t\t\t\t\t\t\tL" << leftIsLower[i] << " R" << rightIsLower[i] << endl;
    }
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
