#pragma once
const class FerryTaskSetting
{
public:
	const int firstBefore = 60 * 5;// the first ferry vehicle should arrive at its location 5min before rdy.:5
	const int waitingTime = 60 * 3;
	const int timeWindow = 60 * 3;// before arriving , 3 min time window are prepared
	const int boardingTime = 60 * 1;// 1 min for customers to board the vehicle
	const int alightingTime = 60 * 2;
	const int maxDeltaTime = 60 * 2;// next vehicle should arrive before 2min after the former vehicle leaving.
	const int relaxingTime = 60 * 10;// 10 min for vehicle to arrive the first task

	const int t1 = 60 * 0;// arrive flight: first ferry vehicle before ready time:20
	const int t2 = -60 * 10;// arrive flight: last ferry vehicle before ready time:10
	const int t3 = 60 * 0;// depart flight: first ferry vehicle before ready time:60
	const int t4 = -60 * 20;// depart flight: last ferry vehicle before ready time:40
	const int t50 = (firstBefore+boardingTime*2);// arrive flight: first ferry vehicle service time including waiting time, boarding time, alighting time:35
	const int t51 = boardingTime * 2;// arrive flight: last ferry vehicle service time including waiting time, boarding time, alighting time:25
	const int t60 = (firstBefore + boardingTime * 2);// depart flight: first ferry vehicle service time including waiting time, boarding time, alighting time:20
	const int t61 = boardingTime * 2;// depart flight: last ferry vehicle service time including waiting time, boarding time, alighting time:15
	
};

