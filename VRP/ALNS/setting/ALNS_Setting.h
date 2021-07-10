#pragma once
enum SolutionFrequency { BEST, ALREADY, NEVER };
class ALNS_Setting
{
	const double r		 = 0.2; // 更新速率
	const int segment	 = 100; // 更新频率
	const double best    = 1.2;
	const double already = 0.8;
	const double never   = 1.0;
};