#include <iostream>
#include <string>
#include <cstdlib>

#include "settings.h"
#include "GameResult.h"
#include "Play.h"
#include "Records.h"


//Параметры запуска
#define _PARAM_TABLE        "-table"  //только показ таблицы рекордов
#define _PARAM_LEVEL        "-level"  //задание уровня сложности [1, 3]
#define _PARAM_MAX          "-max"    //задание максимального значения угадываемого числа [1, 999]


int main(int argc, char** argv)
{
    bool bPlayGame = true;        
    GameResult curResult;
    
    if(argc == 1)
    {
        curResult.maxValue = _MAX_VALUE_DEFAULT;
    }
    else if((argc == 2) && (std::string(argv[1]) == _PARAM_TABLE))
    {
        bPlayGame = false;
    }    
    else if((argc == 3) && (std::string(argv[1]) == _PARAM_MAX) && TestRange(std::atoi(argv[2]), 1, 999))
    {
        curResult.maxValue = std::atoi(argv[2]);
    }
    else if((argc == 3) && (std::string(argv[1]) == _PARAM_LEVEL) && TestRange(std::atoi(argv[2]), 1, 3))
    {        
        int maxValueByLevel[] = {_MAX_VALUE_L1, _MAX_VALUE_L2, _MAX_VALUE_L3};
        int level = std::atoi(argv[2]);
        curResult.maxValue = maxValueByLevel[level - 1];        
    }
    else
    {
        std::cout << "Invalid argument:" << std::endl;
        std::cout << "Usage:     1. guess_the_number" << std::endl;
        std::cout << "           2. guess_the_number -table" << std::endl;
        std::cout << "           3. guess_the_number -level [1..3]" << std::endl;
        std::cout << "           4. guess_the_number -max [1..999]" << std::endl; 
        return -1;
    }
    
    if(bPlayGame)    
        Play(curResult);
    RecordTable(curResult);        
    return 0;
}