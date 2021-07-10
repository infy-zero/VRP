#pragma once
#include <iostream>
#include <vector>

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
class ALNS
{
private:
	IInformation* information;
	IConstraintManager icom;
	IObjectiveManager iobm;
	IOperatorManager iopm;
	ISolutionManager isom;
	ISolution* curSol;

public:
	ALNS();
	void start();
	IConstraintManager*	getICOM()
	{
		return &icom;
	}
	IObjectiveManager* getIOBM()
	{
		return &iobm;
	}
	IOperatorManager* getIOPM()
	{
		return &iopm;
	}
	ISolutionManager* getISOM()
	{
		return &isom;
	}
	void generateInitialSolution();
	void registerInformation(IInformation* _information);
};

