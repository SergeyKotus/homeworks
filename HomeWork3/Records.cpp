#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

#include "Records.h"


bool GetResultsFromFile(std::vector<GameResult>& results)
{
    bool ret = true;
    std::ifstream file(_FILE_RECORDS, std::ios_base::binary);    
    if(file.is_open())
    {
        GameResult result;
        while(true)
        {
            file >> result;
            if(!file.fail())
                results.push_back(result);
            else
                break;    
        } 
    }
    else if(std::filesystem::exists(_FILE_RECORDS))
    {
        std::cout << "Error! Failed to get list of records!" << std::endl;
        ret = false;
    }    
    return ret;
}


bool SaveResultInFile(GameResult& result)
{    
    bool ret = false;
    std::ofstream file;
    if(std::filesystem::exists(_FILE_RECORDS))
        file.open(_FILE_RECORDS, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);
    else
        file.open(_FILE_RECORDS, std::ios_base::binary | std::ios_base::app);
    if(file.is_open())
    {        
        if(result.offset == -1)
        {            
            result.offset = static_cast<int>(file.tellp());            
        }
        else
        {
            file.seekp(result.offset);
        }        
        file << result;
        ret = true;
    }
    else
        std::cout << "Error! Failed to save your result!" << std::endl;
    return ret;
}


void ShowColorRecordRec(GameResult& result, Color recColor)
{    
    if(recColor == Color::standart)
    {        
        std::cout << result << std::endl;
    }
    else
    {
#ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(hConsole, &csbi);
        SetConsoleTextAttribute(hConsole, (recColor == Color::green ? FOREGROUND_GREEN : FOREGROUND_RED));                
        std::cout << result << std::endl;
        SetConsoleTextAttribute(hConsole, csbi.wAttributes);
#else
        std::cout << (recColor == Color::green ? _CC_GREEN : _CC_RED) << result << _CC_RESET << std::endl;        
#endif
    }
}


void ShowRecordTable(std::vector<GameResult>& results, GameResult& curResult, Color recColor)
{
    if(results.size() > 0)
    {    
        std::cout << "-----------------------------------------------------" << std::endl;
        std::cout << "Name                           | MaxValue | Attempts " << std::endl;
        std::cout << "-----------------------------------------------------" << std::endl;    
        for(GameResult result : results)
        {
            ShowColorRecordRec(result, (result.name == curResult.name ? recColor : Color::standart));        
        }
        std::cout << "-----------------------------------------------------" << std::endl;
    }
    else
        std::cout << "Record table is empty." << std::endl;
}


bool isNewRecord(std::vector<GameResult>& results, GameResult& curResult)
{
    using ResultIterator = std::vector<GameResult>::iterator;
    bool bRecord = false;
    ResultIterator iter = std::find(results.begin(), results.end(), curResult.name);
    if(iter != results.end())//пользователь уже в таблице рекордов
    {
        if(curResult < (*iter))//текущий результат лучше прошлого (сравниваем maxValue, attempts, time)
        {                    
            curResult.offset = (*iter).offset;
            *iter = curResult;
            std::cout << "Congratulation with new personal record!" << std::endl;                        
            bRecord = true;
        }
        else
        {                    
            std::cout << "Your previous result was better or equal. Try again." << std::endl;                        
        }
    }
    else if(results.size() < _MAX_FILE_REC)//добавляем пользователя в таблицу
    {
        results.push_back(curResult);
        std::cout << "Congratulation with personal record!" << std::endl;        
        bRecord = true;
    }
    else//таблица рекордов заполнена
    {
        ResultIterator worstResult = std::max_element(results.begin(), results.end());        
        if(curResult < (*worstResult))//текущий результат лучше худшего в таблице (сравниваем maxValue, attempts, time)
        {
            curResult.offset = (*worstResult).offset;
            (*worstResult) = curResult;
            std::cout << "Congratulation with personal record!" << std::endl;                        
            bRecord = true;
        }
        else
        {
            std::cout << "Your result is not good enough to be included in the record table. Try again." << std::endl;            
        }
    }
    return bRecord;
}


void RecordTable(GameResult& curResult)
{    
    std::vector<GameResult> results;
    if(GetResultsFromFile(results))
    {
        Color recColor = Color::red;//цвет старого рекорда
        if((curResult.attempts > 0) && isNewRecord(results, curResult))//выиграли и рекорд попал в таблицу
        {
            recColor = Color::green;//цвет нового рекорда
            SaveResultInFile(curResult);
        }
        std::sort(results.begin(), results.end());
        ShowRecordTable(results, curResult, recColor);
    }
}