#include "StdAfx.h"
#include "TimePeriod.h"
#include <math.h>
std::vector<std::pair<int32,int32>> weatherPair;


int32 DAY_COUNT = -1;
int32 PERIOD_GAME_HOUR = -1;

int32 REAL_MIN_ONE_DAY = -1;
int32 GAME_MIN_ONE_DAY = -1;
int32 GAME_MIN_ONE_HOUR = -1;
int32 GAME_MIN_ONE_PERIOD = -1;

TimePeriod::TimePeriod(void)
{
}


TimePeriod::~TimePeriod(void)
{
}

bool TimePeriod::Calcuate()
{
	//weatherPair.push_back(std::make_pair( 4, 2));
	weatherPair.push_back(std::make_pair( 8, 4));

	for (int32 iPair = 0; iPair < weatherPair.size(); ++iPair)
	{
		DAY_COUNT = weatherPair[iPair].first;
		PERIOD_GAME_HOUR = weatherPair[iPair].second;

		REAL_MIN_ONE_DAY = (60*24);
		GAME_MIN_ONE_DAY = (REAL_MIN_ONE_DAY/DAY_COUNT);

		GAME_MIN_ONE_HOUR = (GAME_MIN_ONE_DAY/24);
		GAME_MIN_ONE_PERIOD	= (GAME_MIN_ONE_DAY*PERIOD_GAME_HOUR/24);

		printf("DAY_COUNT:%d,PERIOD_GAME_HOUR:%d,REAL_MIN_ONE_DAY:%d,\nGAME_MIN_ONE_DAY:%d,GAME_MIN_ONE_HOUR:%d,GAME_MIN_ONE_PERIOD:%d\n",
			DAY_COUNT, PERIOD_GAME_HOUR, REAL_MIN_ONE_DAY, GAME_MIN_ONE_DAY, GAME_MIN_ONE_HOUR, GAME_MIN_ONE_PERIOD);

		for (int32 nRealDay = 0; nRealDay < 2; ++nRealDay)
		{
			printf("####################Day %d##################\n", nRealDay + 1);
			for (int32 nDay = 0; nDay < DAY_COUNT; ++nDay)
			{
				for (f32 fTodTime = 0.0f; fTodTime < 24; fTodTime+= 0.2f)
				{
					int32 nNear = int32(fTodTime/PERIOD_GAME_HOUR);

					for (int32 i = 0; i < (24/PERIOD_GAME_HOUR); ++i)
					{
						int32 nStart = nDay*GAME_MIN_ONE_DAY + (nNear + i)*GAME_MIN_ONE_PERIOD;
						int32 nEnd = nDay*GAME_MIN_ONE_DAY + (nNear + i + 1)*GAME_MIN_ONE_PERIOD;

						m_start.push_back(nStart);
						m_end.push_back(nEnd);

						m_totalStart.push_back(nRealDay*REAL_MIN_ONE_DAY + nStart);
						m_totalEnd.push_back(nRealDay*REAL_MIN_ONE_DAY + nEnd);
					}

					if(!Verify())
						return false;

					m_start.clear();
					m_end.clear();
				}
			}
		}

		Verify2();
	}

	return true;
}

bool TimePeriod::Verify()
{
	for (int32 i = 0; i < m_start.size() && i < m_end.size(); ++i)
	{
		int32 nStart = m_start[i];
		int32 nEnd = m_end[i];

		int32 nSHour, nSMin, nEHour, nEMin;

		nSHour = int32(nStart/60) % 24;
		nSMin = nStart%60;

		nEHour = int32(nEnd/60) % 24;
		nEMin = nEnd%60;

		// 验证时间有效性
		assert(nSHour >= 0 && nSHour < 24 && nSMin >= 0 && nSMin < 60);
		assert(nSHour >= 0 && nSHour < 24 && nEMin >= 0 && nEMin < 60);

		// 验证分钟取值正确性
		assert( nSMin == GAME_MIN_ONE_PERIOD || nSMin == GAME_MIN_ONE_PERIOD*2 || nSMin == GAME_MIN_ONE_PERIOD*3 || 
			nSMin == GAME_MIN_ONE_PERIOD*4 || nSMin == GAME_MIN_ONE_PERIOD*5 || nSMin == (GAME_MIN_ONE_PERIOD*6)%60);

		assert( nEMin == GAME_MIN_ONE_PERIOD || nEMin == GAME_MIN_ONE_PERIOD*2 || nEMin == GAME_MIN_ONE_PERIOD*3 || 
			nEMin == GAME_MIN_ONE_PERIOD*4 || nEMin == GAME_MIN_ONE_PERIOD*5 || nEMin == (GAME_MIN_ONE_PERIOD*6)%60);

		// 验证时间间隔是否正确
		//printf("%d %d %d %d\n", nSHour, nSMin, nEHour, nEMin);

		if(nSHour == 23 && nEHour == 0)
			assert ( abs( (nSHour*60 + nSMin - 24*60 - nEMin)) == GAME_MIN_ONE_PERIOD); 
		else
			assert( abs(nSHour*60+nSMin - (nEHour*60 + nEMin) ) == GAME_MIN_ONE_PERIOD );		


		// 输出
		printf("%02d:%02d-%02d:%02d\n", nSHour, nSMin, nEHour, nEMin);
	}

	cout<<"---------------"<<endl;

	return true;
}

bool TimePeriod::Verify2()
{
	int32 nTimeStamp = 1452674477;
	int32 nLastTime = 60*60*24;

	//printf( "%d %02d:%02d:%02d\n", nTimeStamp, (nTimeStamp%(3600*24)/3600 + 8)%24, nTimeStamp%3600/60, nTimeStamp%60);

	int32 nBuffHour = (nTimeStamp%(3600*24)/3600 + 8)%24;
	int32 nBuffMin = nTimeStamp%3600/60;
	int32 nBuffCreateMin = nBuffHour*60 + nBuffMin;
	int32 nBuffEndMin = nBuffCreateMin + nLastTime;

	int32 nItemCount = (int32)ceil(nLastTime / (GAME_MIN_ONE_PERIOD*1.0f*60));
	printf("LastTime:%d Count:%d\n", nLastTime, nItemCount);

	for (int32 nTime = nLastTime; nTime > 0; nTime-=5.0f)
	{
		for (int i = 0; i < m_totalStart.size() && i < m_totalEnd.size() && i < nItemCount; ++i)
		{
			int32 nStart = m_totalStart[i];
			int32 nEnd = m_totalEnd[i];

			int32 nSHour, nSMin, nEHour, nEMin;

			nSHour = int32(nStart/60) % 24;
			nSMin = nStart%60;

			nEHour = int32(nEnd/60) % 24;
			nEMin = nEnd%60;

			printf("%02d:%02d-%02d:%02d Count:%d\n", nSHour, nSMin, nEHour, nEMin, i+1);

			if(i % 6 == 0 && i != 0)
				printf("------------------\n");
		}
	}

	return true;
}
