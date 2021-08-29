#pragma once
#define fts FerryTaskSetting::
static class FerryTaskSetting
{
public:
	static constexpr int firstBefore = 60 * 5;// 第一辆车应该提前五分钟到达
	static constexpr int waitingTime = 60 * 3;// 每辆车的等待时间窗
	static constexpr int timeWindow = 60 * 3;// 每辆车的等待时间窗
	static constexpr int boardingTime = 60 * 1;// 乘客上车时间
	static constexpr int alightingTime = 60 * 2;// 乘客下车时间
	static constexpr int maxDeltaTime = 60 * 2;// 两辆摆渡车间隔时间
	static constexpr int relaxingTime = 60 * 10;// 10 min for vehicle to arrive the first task

	static constexpr int t1 = 60 * 0;// 到达航班：第一辆车在ETA前的时间：标准（20）
	static constexpr int t2 = -60 * 10;// 到达航班：最后一辆车在ETA前的时间：标准（10）
	static constexpr int t3 = 60 * 0;// 出发航班：第一辆车在ETA前的时间：标准（60）
	static constexpr int t4 = -60 * 20;// 出发航班：最后一辆车在ETA前的时间：标准（40）
	static constexpr int t50 = (firstBefore+boardingTime+alightingTime);// 到达航班-第一辆车服务时间：等待时间+上车时间+下车时间:35
	static constexpr int t51 = boardingTime + alightingTime;// 到达航班-最后一辆车服务时间：等待时间+上车时间+下车时间:25
	static constexpr int t60 = (firstBefore + boardingTime + alightingTime);// 出发航班-第一辆车服务时间：等待时间+上车时间+下车时间:20
	static constexpr int t61 = boardingTime + alightingTime;// 出发航班-最后一辆车服务时间：等待时间+上车时间+下车时间:15
	
};

