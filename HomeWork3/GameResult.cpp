#include <iostream>
#include <iomanip>
#include "GameResult.h"


std::ostream& operator<<(std::ostream& stream, GameResult& result)
{
    if(&stream == &std::cout)
    {
        std::cout << result.name << " | " << std::right << std::setw(8) << result.maxValue << " | " 
                  << std::setw(8) << static_cast<int>(result.attempts);

    }
    else
    {    
        stream.write(result.name.c_str(), result.name.length());
        stream.write((char*) &result.maxValue, sizeof(result.maxValue));
        stream.write((char*) &result.attempts, sizeof(result.attempts));
        stream.write((char*) &result.time, sizeof(result.time));
        stream.write((char*) &result.offset, sizeof(result.offset));
    }
    return stream;
}


std::istream& operator>>(std::istream& stream, GameResult& result)//чтение из файла
{    
    char buff[_NAME_LENGHT + 1];
    stream.read(buff, _NAME_LENGHT);
    buff[_NAME_LENGHT] = '\0';
    result.name = buff;
    stream.read((char*) &result.maxValue, sizeof(result.maxValue));
    stream.read((char*) &result.attempts, sizeof(result.attempts));
    stream.read((char*) &result.time, sizeof(result.time));
    stream.read((char*) &result.offset, sizeof(result.offset));    
    return stream;
}
