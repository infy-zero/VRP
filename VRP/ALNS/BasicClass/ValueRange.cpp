#include "ALNS/BasicClass/ValueRange.h"
int ValueRange::getOperatorNum()
{
	return operatorNum;
}
void ValueRange::setOperatorNum(int _operatorNum)
{
	operatorNum = _operatorNum;
}
double ValueRange::getMinValue()
{
	return minValue;
}
void ValueRange::setMinValue(double _minValue)
{
	minValue = _minValue;
}
double ValueRange::getCurValue()
{
	return curValue;
}
void ValueRange::setCurValue(double _curValue)
{
	curValue = _curValue;
}
double ValueRange::getMaxValue()
{
	return maxValue;
}
void ValueRange::setMaxValue(double _maxValue)
{
	maxValue = _maxValue;
}