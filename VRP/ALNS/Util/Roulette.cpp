#include "Roulette.h"
IOperator* Roulette::getRouletteIOperator(vector<IOperator>* operators)
{
	double totalWeight = 0;
	for (int i=0;i<operators->size();i++)
		totalWeight += operators->at(i).getWeight();
	double selectedWeight = getRandom() * totalWeight;
	for (int i = 0; i < operators->size(); i++)
	{
		selectedWeight -= operators->at(i).getWeight();
		if (selectedWeight <= 0)
			return &(operators->at(i));
	}
	return &(operators->at(operators->size()-1));
}
double Roulette::getRandom()
{
	static random_device rd;  //Will be used to obtain a seed for the random number engine
	mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	uniform_real_distribution<> dis(0.0, 1.0);
	return dis(gen);
}