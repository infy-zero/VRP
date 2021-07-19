#pragma once
static class FerryTaskSetting
{
public:
	static constexpr int firstBefore = 60 * 5;// the first ferry vehicle should arrive at its location 5min before rdy.:5
	static constexpr int waitingTime = 60 * 3;
	static constexpr int timeWindow = 60 * 3;// before arriving , 3 min time window are prepared
	static constexpr int boardingTime = 60 * 1;// 1 min for customers to board the vehicle
	static constexpr int alightingTime = 60 * 2;
	static constexpr int maxDeltaTime = 60 * 2;// next vehicle should arrive before 2min after the former vehicle leaving.
	static constexpr int relaxingTime = 60 * 10;// 10 min for vehicle to arrive the first task

	static constexpr int t1 = 60 * 0;// arrive flight: first ferry vehicle before ready time:20
	static constexpr int t2 = -60 * 10;// arrive flight: last ferry vehicle before ready time:10
	static constexpr int t3 = 60 * 0;// depart flight: first ferry vehicle before ready time:60
	static constexpr int t4 = -60 * 20;// depart flight: last ferry vehicle before ready time:40
	static constexpr int t50 = (firstBefore+boardingTime*2);// arrive flight: first ferry vehicle service time including waiting time, boarding time, alighting time:35
	static constexpr int t51 = boardingTime * 2;// arrive flight: last ferry vehicle service time including waiting time, boarding time, alighting time:25
	static constexpr int t60 = (firstBefore + boardingTime * 2);// depart flight: first ferry vehicle service time including waiting time, boarding time, alighting time:20
	static constexpr int t61 = boardingTime * 2;// depart flight: last ferry vehicle service time including waiting time, boarding time, alighting time:15
	
};

