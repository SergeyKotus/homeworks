#pragma once

#include "GameResult.h"


inline bool TestRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}

void Play(GameResult& curResult);