#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>
#include "statistics.h"

double Min::calculate(const std::vector<double>& values)
{
    return *std::min_element(values.begin(), values.end());
}

const char* Min::name() const
{
    return "min";
}


double Max::calculate(const std::vector<double>& values)
{
    return *std::max_element(values.begin(), values.end());
}

const char* Max::name() const
{
    return "max";
}


double Mean::calculate(const std::vector<double>& values)
{
    return std::accumulate(values.begin(), values.end(), static_cast<double>(0)) / values.size();
}

const char* Mean::name() const
{
    return "mean";
}


double StdDeviation::calculate(const std::vector<double>& values)
{
    double mean = std::accumulate(values.begin(), values.end(), static_cast<double>(0)) / values.size();
    double stdDeviation = std::accumulate(values.begin(), values.end(), static_cast<double>(0), 
                                          [mean](double x, double y)->double {return x + (y - mean) * (y - mean);});
    stdDeviation = sqrt(stdDeviation / values.size());
    return stdDeviation;
}

const char* StdDeviation::name() const
{
    return "std";
}


Pct::Pct(int pct)
{
    this->pct = std::min(std::max(pct, 1), 100);
    this->pctName = "pct" + std::to_string(this->pct);    
}

double Pct::calculate(const std::vector<double>& values)
{
    if(values.size() > 0)
    {        
        std::vector<double> sortedValues = values;     
        std::sort(sortedValues.begin(), sortedValues.end());        
        size_t pctNum = static_cast<size_t>(std::ceil(sortedValues.size() * pct / 100.0));        
        return sortedValues[pctNum - 1];
    }
    else
        return 0.0;
}

const char* Pct::name() const
{
    return this->pctName.c_str();       
}