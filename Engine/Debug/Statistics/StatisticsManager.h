#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "StatisticsObject.h"

class ENGINE_API StatisticsManager : public Singleton<StatisticsManager>
{
	friend class Singleton<StatisticsManager>;

	DECLARE_SINGLETON(StatisticsManager)

private:
	std::map<std::string, StatisticsObject*> _statistics;

public:
	StatisticsObject* GetStatisticsObject (const std::string& name) const;
	void SetStatisticsObject (const std::string&, StatisticsObject*);
private:
	StatisticsManager ();
	StatisticsManager (const StatisticsManager&);
	StatisticsManager& operator=(const StatisticsManager&);
	~StatisticsManager ();
};

#endif