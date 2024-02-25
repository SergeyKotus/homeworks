#include <vector>
#include <iostream>
#include "statistics.h"

int main()
{ 
    //получение входных данных
    std::cout << "Input values:" << std::endl;
    std::vector<double> values;
    double value;
    while((values.size() < 1000) && (std::cin >> value))
    {
        values.push_back(value);
    }
    if (!std::cin.eof() && !std::cin.good()) 
    {
		std::cerr << "Invalid input data" << std::endl;    
		return 1;
	}    
    
    if(values.size() > 0)
    {
        //расчет статистики
        const size_t statistics_count = 6;
        IStatistics *statistics[statistics_count] = {new Min{}, new Max{}, new Mean{}, new StdDeviation{}, new Pct{90}, new Pct{95}};        

        for(size_t i = 0; i < statistics_count; ++i)
        {
            std::cout << statistics[i]->name() << " = " << statistics[i]->calculate(values) << std::endl;
        }

        for(size_t i = 0; i < statistics_count; ++i)
        {
            delete statistics[i];
        }
    }
    else
        std::cout << "Empty input data" << std::endl;
    return 0;
}