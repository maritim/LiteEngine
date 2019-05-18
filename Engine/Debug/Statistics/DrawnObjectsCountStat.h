#ifndef DRAWNOBJECTSCOUNTSTAT_H
#define DRAWNOBJECTSCOUNTSTAT_H

#include "StatisticsObject.h"

#include <cstddef>

class ENGINE_API DrawnObjectsCountStat : public StatisticsObject
{
private:
	std::size_t _drawnObjectsCount;

public:
	DrawnObjectsCountStat ();
	DrawnObjectsCountStat (std::size_t drawnObjectsCount);

	std::size_t GetDrawnObjectsCount () const;

	void SetDrawnObjectsCount (std::size_t drawnObjectsCount);
};

#endif