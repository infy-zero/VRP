#include "IOperator.h"
ISolution IOperator::run(ISolution& input)
{
	times++;
	ISolution result = input;
	cout << "\t    当前使用算子 " << toString() << endl;
	return result;
}
enum OperatorType IOperator::getType()
{
	return type;
}
void IOperator::setType(enum OperatorType _type)
{
	type = _type;
}
void IOperator::setIndex(int _index)
{
	index = _index;
}
double IOperator::getScores()
{
	return scores;
}
void IOperator::clearScores()
{
	scores = 0;
}
void IOperator::addScores(enum SolutionFrequency& sf)
{
	switch (sf)
	{
	case BEST:
		scores += ALNS_Setting::best;
		break;
	case ALREADY:
		scores += ALNS_Setting::already;
		break;
	case NEVER:
		scores += ALNS_Setting::never;
		break;
	default:
		throw "错误的sf类型。";
	}
}
double IOperator::getWeight()
{
	return weight;
}
void IOperator::setWeight(double _weight)
{
	weight = _weight;
}
string IOperator::getName()
{
	return name;
}
void IOperator::setName(string _name)
{
	name = _name;
}
string IOperator::toString()
{
	return ALNS_Setting::to_string(type) + "-" + to_string(index) + name;
}
void IOperator::update()
{
	weight = weight * (1 - ALNS_Setting::r) + ALNS_Setting::r * scores / times;
	scores = 0;
	times = 0;
}