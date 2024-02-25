#include <vector>

class IStatistics 
{
public:
	virtual ~IStatistics() {}
	virtual double calculate(const std::vector<double>& values) = 0;	
	virtual const char* name() const = 0;
};


class Min : public IStatistics
{
public:
	virtual double calculate(const std::vector<double>& values) override;
	virtual const char* name() const override;
};

class Max : public IStatistics
{
public:
    virtual double calculate(const std::vector<double>& values) override;	
	virtual const char* name() const override;
};

class Mean : public IStatistics
{
public:
    virtual double calculate(const std::vector<double>& values) override;	
	virtual const char* name() const override;
};

class StdDeviation : public IStatistics
{
public:    
    virtual double calculate(const std::vector<double>& values) override;	
	virtual const char* name() const override;
};

class Pct : public IStatistics
{
	int pct;
	std::string pctName;
public:    
	Pct(int pct);
    virtual double calculate(const std::vector<double>& values) override;	
	virtual const char* name() const override;
};

