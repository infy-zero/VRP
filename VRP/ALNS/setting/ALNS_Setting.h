#pragma once
#include <string>
using namespace std;
enum SolutionFrequency { BEST, ALREADY, NEVER };
enum OperatorType { DESTROY, REPAIR };
static class ALNS_Setting
{
public:
	// 适应层参数
	static constexpr double r = 0.2; // 更新速率
	static constexpr int segment = 3; // 更新频率
	static constexpr double initWeight = 1.0; // 初始权重
	static constexpr double best = 1.2;
	static constexpr double already = 0.8;
	static constexpr double never = 1.0;
	// 迭代参数
	static constexpr double startT = 100;
	static constexpr double endT = 10;
	static constexpr double decay = 0.6;
	static constexpr int decayTimes = 1;
	// static constexpr int additionalTimes = 5;

	static string to_string(enum OperatorType ot);
};