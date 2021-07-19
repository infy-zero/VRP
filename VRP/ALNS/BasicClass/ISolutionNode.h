#pragma once
#include <vector>
#include "ValueRange.h"
#include "FerryVehicleTask.h"

using namespace std;
class ISolutionNode
{
private:
	FerryVehicleTask*			fvt;
	vector<ValueRange>			valueRanges;
public:
	ISolutionNode();
	ISolutionNode(FerryVehicleTask* _fvt);
	vector<ValueRange> getValueRanges();
	void setValueRanges(vector<ValueRange> _valueRanges);
};