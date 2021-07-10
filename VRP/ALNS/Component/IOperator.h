#pragma once
#include <iostream>
#include "ALNS/Component/ISolution.h"
using namespace std;
enum OperatorType{DESTROY,REPAIR};
class IOperator
{
private:
	ISolution* solution;
	enum OperatorType type;
	string name;
	int index;
	double scores	 = 0;
	double weight    = 1;
public:
	virtual void run();
	enum OperatorType getType();
	void setIndex(int _index);
	double getScores();
	void clearScores();
	void addScores(double addScores);
	double getWeight();
	string getName();
	string setName(string _name);
};

