#pragma once
#include <vector>
#include <random>
#include "ALNS/Component/IOperator.h"

using namespace std;
static class Roulette
{
private:
	
public:
	static IOperator* getRouletteIOperator(vector<IOperator>* operators);
	static double getRandom();
};