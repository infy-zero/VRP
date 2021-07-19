#pragma once
class ValueRange
{
private:
	int operatorNum;
	double minValue;
	double curValue;
	double maxValue;
public:
	int getOperatorNum();
	void setOperatorNum(int _operatorNum);
	double getMinValue();
	void setMinValue(double _minValue);
	double getCurValue();
	void setCurValue(double _curValue);
	double getMaxValue();
	void setMaxValue(double _maxValue);
};

