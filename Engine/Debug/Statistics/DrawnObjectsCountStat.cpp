#include "DrawnObjectsCountStat.h"

DrawnObjectsCountStat::DrawnObjectsCountStat () :
	_drawnObjectsCount (0)
{

}

DrawnObjectsCountStat::DrawnObjectsCountStat (std::size_t drawnObjectsCount) :
	_drawnObjectsCount (drawnObjectsCount)
{
	
}

std::size_t DrawnObjectsCountStat::GetDrawnObjectsCount () const
{
	return _drawnObjectsCount;
}

void DrawnObjectsCountStat::SetDrawnObjectsCount (std::size_t drawnObjectsCount)
{
	_drawnObjectsCount = drawnObjectsCount;
}