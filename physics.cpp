#include "physics.h"


void Physics(int width, int *waterLevel, int *groundLevel, bool *leftIsLower, bool *rightIsLower, int water_flow)
{
    // apply gravity
    for (int i = 0; i < width; i++)
    {
        if (waterLevel[i] < water_flow) continue;

        // reset values
        leftIsLower[i] = false;
        rightIsLower[i] = false;

        int currentHeight = groundLevel[i] + waterLevel[i];
        int leftHeight = 0;
        int rightHeight = 0;
        if (i > 0) leftHeight = groundLevel[i-1] + waterLevel[i-1];
        if (i < width - 1) rightHeight = groundLevel[i+1] + waterLevel[i+1];

        if (currentHeight > leftHeight) leftIsLower[i] = true;
        if (currentHeight > rightHeight) rightIsLower[i] = true;

        if (leftIsLower[i] && rightIsLower[i])
        {
            waterLevel[i] -= water_flow;
            waterLevel[i-1] += water_flow / 2;
            waterLevel[i+1] += water_flow / 2;
        }
        else if (leftIsLower[i])
        {
            waterLevel[i] -= water_flow;
            waterLevel[i-1] += water_flow;
        }
        else if (rightIsLower[i])
        {
            waterLevel[i] -= water_flow;
            waterLevel[i+1] += water_flow;
        }
        else continue;
    }

    // drain water from the bottom
    for (int i = 0; i < width; i++)
        if (waterLevel[i] >= water_flow && groundLevel[i] == 0 && waterLevel[i] > 0)
            waterLevel[i] -= water_flow;
}