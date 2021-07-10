#include "IOperator.h"
void IOperator::run()
{
	cout << "使用当前算子:" << (type == DESTROY ? "destroy" : "repair") << index << endl;
}
enum OperatorType IOperator::getType()
{
	return type;
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
void IOperator::addScores(double addScores)
{
	scores += addScores;
}
double IOperator::getWeight()
{
	return weight;
}
string IOperator::getName()
{
	return name;
}
string IOperator::setName(string _name)
{
	name = _name;
}