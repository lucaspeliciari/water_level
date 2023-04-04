#include "checks.h"


int MaxTotalHeight(int width, int *waterLevel, int *groundLevel)
{
    int maxTotalHeight = 0;
    for (int i = 0; i < width; i++)
    {
        if (waterLevel[i] + groundLevel[i] > maxTotalHeight) maxTotalHeight = waterLevel[i] + groundLevel[i];
    }
    return maxTotalHeight;
}

int MaxWaterHeight(int width, int *waterLevel, int *groundLevel)
{
    int maxWaterHeight = 0;
    for (int i = 0; i < width; i++)
    {
        if (waterLevel[i] > maxWaterHeight) maxWaterHeight = waterLevel[i];
    }
    return maxWaterHeight;
}

int WaterVolume(int width, int *waterLevel, int *groundLevel)
{
    int waterVolume = 0;
    for (int i = 0; i < width; i++)
    {
        waterVolume += waterLevel[i];
    }
    return waterVolume;
}

// return true if water is perfectly levelled (what should happen after an infinite amount of time)
// but it never gets perfectly levelled so this is always false
bool Levelled(int width, bool *leftIsLower, bool *rightIsLower)
{
    for (int i = 0; i < width; i++)
    {
        if (leftIsLower[i] || rightIsLower[i]) return false;
    }
    return true;
}