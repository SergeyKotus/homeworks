#pragma once

#include <iostream>
#include <string>

#include "settings.h"
#include "GameResult.h"


struct GameResult
{
    std::string name = "";          //имя пользователя    
    int maxValue = 1000;            //заданное максимальное значение в текущей игре
    unsigned char attempts = 0;     //кол-во попыток угадать число в выигранной игре
    time_t time = 0;                //время победы
    int offset = -1;                //смещение для записи результат в таблице рекордов (-1 - добавление результата в конец файла)
};


inline bool operator==(const GameResult& result, const std::string& name)
{
    return (result.name == name);    
}


inline bool operator<(const GameResult& result1, const GameResult& result2)
{
    if(result1.maxValue == result2.maxValue)
    {
        if(result1.attempts == result2.attempts)
            return result1.time < result2.time;
        else
            return result1.attempts < result2.attempts;
    }
    else
        return result1.maxValue > result2.maxValue;    
}


std::ostream& operator<<(std::ostream& stream, GameResult& result);//вывод в консоль или в файл
std::istream& operator>>(std::istream& stream, GameResult& result);//чтение из файла