#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <unordered_set>
#include "IInformation.h"
#include "ALNS/Component/ISolution.h"
#include "BasicClass/ISolutionNode.h"
#include "ALNS/Util/ICriterion.h"


using namespace std;
 
typedef shared_ptr<ISolutionNode> spn;	// node µÄ¹²ÏíÖ¸Õë

class ALNS
{
private:
	ISolution* curSol;
	ICriterion criterion;
	unordered_set<string> solutionSpace;
	double bestVal = -1;

public:
	ALNS();
	void start();
	void generateInitialSolution();
};

