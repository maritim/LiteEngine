#ifndef STATISTICSMANAGER_H
#define STATISTICSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Debug/Statistics/StatisticsObject.h"

class ENGINE_API StatisticsManager : public Singleton<StatisticsManager>
{
	friend class Singleton<StatisticsManager>;

	DECLARE_SINGLETON(StatisticsManager)

private:
	std::map<std::string, StatisticsObject*> _statisticsObjects;

public:
	template <class T>
	T* GetStatisticsObject ();
private:
	StatisticsManager ();
	StatisticsManager (const StatisticsManager&);
	StatisticsManager& operator=(const StatisticsManager&);
	~StatisticsManager ();
};

template <class T>
T* StatisticsManager::GetStatisticsObject ()
{
	auto it = _statisticsObjects.find (T::GetName ());

	if (it == _statisticsObjects.end ()) {
		T* statisticsObject = new T ();

		_statisticsObjects [T::GetName ()] = statisticsObject;

		return statisticsObject;
	}

	return (T*) it->second;
}

#endif