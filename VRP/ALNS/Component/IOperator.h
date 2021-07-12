#pragma once
#include <iostream>
#include "ALNS/setting/ALNS_Setting.h"
#include "ALNS/Component/ISolution.h"
using namespace std;

class IOperator
{
private:
	enum OperatorType type;
	string name;
	int index;
	int times		 = 0;
	double scores	 = 0;
	double weight    = 1;
public:
	virtual ISolution* run(ISolution* Input);
	enum OperatorType getType();
	void setType(enum OperatorType _type);
	void setIndex(int _index);
	double getScores();
	void clearScores();
	void addScores(enum SolutionFrequency& sf);
	double getWeight();
	void setWeight(double _weight);
	string getName();
	void setName(string _name);
	string toString();
	void update();
};

