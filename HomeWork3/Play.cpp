#include <iostream>
#include <string>
#include <algorithm>

#include "Play.h"


int GetRandomValue(int maxValue)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
	return std::rand() % maxValue + 1;
}


std::string GetUserName()
{
    std::string userName;
    std::cout << "Hi! Enter your name, please (" << _NAME_LENGHT << " symbols):" << std::endl;    
    do
    {
        std::getline(std::cin, userName);        
    }
    while (!userName.length());
    return userName;
}


void Play(GameResult& curResult)
{    
    curResult.name = GetUserName();
    curResult.name.resize(_NAME_LENGHT, ' ');    
    const int targetValue = GetRandomValue(curResult.maxValue);
    int attempts = 0;
    int currentValue;    
    
    //std::cout << "targetValue = " << targetValue << std::endl;
    std::cout << "Enter your guess (number from 1 to " << curResult.maxValue << "):" << std::endl;
	do 
    {
        attempts++;
		std::cin >> currentValue;
        if(std::cin.fail() || !TestRange(currentValue, 1, curResult.maxValue))
        {
            std::cout << "Bad value!" << std::endl;            
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');            
        }
		else if (currentValue > targetValue) {
			std::cout << "less than " << currentValue << std::endl;
		}
		else if (currentValue < targetValue) {
			std::cout << "greater than " << currentValue << std::endl;
		}
		else {
			std::cout << "you win! attempts = " << attempts << std::endl;            
            curResult.attempts = attempts;
            curResult.time = std::time(nullptr);
			break;
		}
        
        if(attempts == _MAX_ATTEMPTS)
        {
            std::cout << "you loose! attempts = " << attempts << std::endl;
            break;
        }        
	} 
    while(true);
}