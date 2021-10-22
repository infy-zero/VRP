#pragma once

#define fts FerryTaskSetting::

static class FerryTaskSetting
{
public:
	static constexpr double FTS_firstBefore = 60 * 5;						// ��һ����Ӧ����ǰ����ӵ���
	static constexpr double FTS_timewindow = 60 * 3;						// ÿ�����ķ���ʱ�䴰���������ʱ�� - �������ʱ�䣩
	// TODO(Lvning): �ȴ�ʱ��Ӧ�ú͵���ʱ���
	static constexpr double FTS_arrive_before_start_timewindow = 60 * 3;	// ÿ����������ǰ��ʼʱ���ʱ�䣨���ڸõط��ȴ�ʱ�䣩
	static constexpr double FTS_boardingTime = 60 * 2;						// �˿��ϳ�ʱ��
	static constexpr double FTS_alightingTime = 60 * 2;					// �˿��³�ʱ��
	static constexpr double FTS_max_delta_time = 60 * 2;					// ����ͬһ�ܷɻ��������ڶɳ����ʱ��
	static constexpr double FTS_max_time_window = 60 * 5;					// ���೵���ȴ�ʱ�䴰

	static constexpr double FTS_t1 = 60 * 0;								// ���ﺽ�ࣺ��һ������ETAǰ��ʱ�䣺��׼��20��
	static constexpr double FTS_t2 = -60 * 0;								// ���ﺽ�ࣺ���һ������ETAǰ��ʱ�䣺��׼��10��
	static constexpr double FTS_t3 = 60 * 0;								// �������ࣺ��һ������ETAǰ��ʱ�䣺��׼��60��
	static constexpr double FTS_t4 = -60 * 0;								// �������ࣺ���һ������ETAǰ��ʱ�䣺��׼��40��

	static constexpr double FTS_t50 = (FTS_firstBefore+FTS_boardingTime+FTS_alightingTime);// ���ﺽ��-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:35
	static constexpr double FTS_t51 = FTS_boardingTime + FTS_alightingTime;// ���ﺽ��-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:25
	static constexpr double FTS_t60 = (FTS_firstBefore + FTS_boardingTime + FTS_alightingTime);// ��������-��һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:20
	static constexpr double FTS_t61 = FTS_boardingTime + FTS_alightingTime;// ��������-���һ��������ʱ�䣺�ȴ�ʱ��+�ϳ�ʱ��+�³�ʱ��:15
	
};

