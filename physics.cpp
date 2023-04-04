#include "physics.h"


void Physics(int width, int *waterLevel, int *groundLevel, bool *leftIsLower, bool *rightIsLower, int water_flow)
{
    for (int i = 0; i < width; i++)
    {   
        leftIsLower[i] = false;
        rightIsLower[i] = false;
    }

    // apply gravity
    for (int i = 0; i < width; i++)
    {
        if (waterLevel[i] < water_flow) continue;

        leftIsLower[i] = false;
        rightIsLower[i] = false;

        int currentHeight = groundLevel[i] + waterLevel[i];
        int leftHeight = -1;
        int rightHeight = -1;
        if (i > 0) leftHeight = groundLevel[i-1] + waterLevel[i-1];
        if (i < width) rightHeight = groundLevel[i+1] + waterLevel[i+1];

        if (leftHeight > -1 && currentHeight > leftHeight) leftIsLower[i] = true;
        if (rightHeight > -1 && currentHeight > rightHeight) rightIsLower[i] = true;

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

    // drain water
    for (int i = 0; i < width; i++)
    {
        if (groundLevel[i] == 0 && waterLevel[i] > 0)
        {
            waterLevel[i] -= water_flow;
        }

    }
}