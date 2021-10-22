#pragma once

#define fts FerryTaskSetting::

static class FerryTaskSetting
{
public:
	static constexpr double FTS_firstBefore = 60 * 5;						// 第一辆车应该提前五分钟到达
	static constexpr double FTS_timewindow = 60 * 3;						// 每辆车的服务时间窗（最早服务时间 - 最晚服务时间）
	// TODO(Lvning): 等待时间应该和到达时间绑定
	static constexpr double FTS_arrive_before_start_timewindow = 60 * 3;	// 每辆车最早提前开始时间的时间（能在该地方等待时间）
	static constexpr double FTS_boardingTime = 60 * 2;						// 乘客上车时间
	static constexpr double FTS_alightingTime = 60 * 2;					// 乘客下车时间
	static constexpr double FTS_max_delta_time = 60 * 2;					// 服务同一架飞机的两辆摆渡车间隔时间
	static constexpr double FTS_max_time_window = 60 * 5;					// 其余车辆等待时间窗

	static constexpr double FTS_t1 = 60 * 0;								// 到达航班：第一辆车在ETA前的时间：标准（20）
	static constexpr double FTS_t2 = -60 * 0;								// 到达航班：最后一辆车在ETA前的时间：标准（10）
	static constexpr double FTS_t3 = 60 * 0;								// 出发航班：第一辆车在ETA前的时间：标准（60）
	static constexpr double FTS_t4 = -60 * 0;								// 出发航班：最后一辆车在ETA前的时间：标准（40）

	static constexpr double FTS_t50 = (FTS_firstBefore+FTS_boardingTime+FTS_alightingTime);// 到达航班-第一辆车服务时间：等待时间+上车时间+下车时间:35
	static constexpr double FTS_t51 = FTS_boardingTime + FTS_alightingTime;// 到达航班-最后一辆车服务时间：等待时间+上车时间+下车时间:25
	static constexpr double FTS_t60 = (FTS_firstBefore + FTS_boardingTime + FTS_alightingTime);// 出发航班-第一辆车服务时间：等待时间+上车时间+下车时间:20
	static constexpr double FTS_t61 = FTS_boardingTime + FTS_alightingTime;// 出发航班-最后一辆车服务时间：等待时间+上车时间+下车时间:15
	
};

