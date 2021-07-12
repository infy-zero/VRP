#pragma once
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>

#include "ALNS/BasicClass/IInformation.h"

#include "ALNS/Manager/IConstraintManager.h";
#include "ALNS/Manager/IObjectiveManager.h"
#include "ALNS/Manager/IOperatorManager.h"
#include "ALNS/Manager/ISolutionManager.h"

#include "BasicClass/ALNSNode.h"
#include "ALNS/Component/ISolution.h"
#include "ALNS/Component/IConstraint.h"
#include "ALNS/Component/IObjective.h"
#include "ALNS/Component/IOperator.h"
#include "ALNS/Util/ICriterion.h"

using namespace std;
class ALNS
{
private:
	IInformation* information;
	IConstraintManager icom;
	IObjectiveManager iobm;
	IOperatorManager iopm;
	ISolutionManager isom;
	ISolution* curSol;
	ICriterion criterion;

public:
	ALNS();
	void start();
	void generateInitialSolution();
	void setInformation(IInformation* _information);
	void setSolution(ISolution* _solution);
	void initialize();
	

	// »ù±¾²Ù×÷
	IConstraintManager* getICOM();
	IObjectiveManager* getIOBM();
	IOperatorManager* getIOPM();
	ISolutionManager* getISOM();
};

