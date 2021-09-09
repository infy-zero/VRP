#include "ALNS_Setting.h"

std::string ALNS_Setting::to_string(enum OperatorType ot)
{
	if (ot == REPAIR)
		return "repair";
	if (ot == DESTROY)
		return "destroy";
	return "NULL";
}
