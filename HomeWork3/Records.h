#pragma once

#include <iostream>

#include "settings.h"
#include "GameResult.h"

//файл
#define _FILE_RECORDS       "records.txt" //файл таблицы рекордов

//Цвета
#define _CC_RED      "\033[0;31m"
#define _CC_GREEN    "\033[0;32m"
#define _CC_RESET    "\033[0m"

enum class Color {standart, red, green};


void RecordTable(GameResult& curResult);