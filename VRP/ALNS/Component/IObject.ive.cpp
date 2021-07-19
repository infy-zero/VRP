#include "IObjective.h"

double IObjective::calObjective(ISolution& solution)
{
	return 0;
}
void IObjective::setLevel(int _level)
{
	level = _level;
}
int IObjective::getLevel()
{
	return level;
}
