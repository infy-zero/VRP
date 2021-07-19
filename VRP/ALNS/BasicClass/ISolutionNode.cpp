#include "ISolutionNode.h"

ISolutionNode::ISolutionNode()
{

}
ISolutionNode::ISolutionNode(FerryVehicleTask* _fvt):fvt(_fvt)
{
}

vector<ValueRange> ISolutionNode::getValueRanges()
{
	return valueRanges;
}
void ISolutionNode::setValueRanges(vector<ValueRange> _valueRanges)
{
	valueRanges = _valueRanges;
}