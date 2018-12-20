#include "IterateOverRenderVolumeCollection.h"

#include "Lighting/LightsManager.h"

IterateOverRenderVolumeCollection::IterateOverRenderVolumeCollection (std::size_t iterations) :
	_iterations (iterations),
	_iterationsElapsed (0),
	_idleRenderVolume (new IdleRenderVolume ())
{

}

IterateOverRenderVolumeCollection::~IterateOverRenderVolumeCollection ()
{
	delete _idleRenderVolume;
}

void IterateOverRenderVolumeCollection::Reset ()
{
	_iterationsElapsed = 0;
}

RenderVolumeI* IterateOverRenderVolumeCollection::GetNextVolume ()
{
	if (_iterationsElapsed == _iterations) {
		return nullptr;
	}

	/*
	 * Iterate over idle render volume one more time
	*/

	_iterationsElapsed ++;

	return _idleRenderVolume;
}
