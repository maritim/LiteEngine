#include "StatisticsManager.h"

StatisticsManager::StatisticsManager () :
	_statisticsObjects ()
{

}

StatisticsManager::~StatisticsManager ()
{
	for (auto it : _statisticsObjects) {
		if (it.second == nullptr) {
			continue;
		}

		delete it.second;
	}
}

SPECIALIZE_SINGLETON(StatisticsManager)
