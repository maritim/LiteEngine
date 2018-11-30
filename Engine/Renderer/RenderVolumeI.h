#ifndef RENDERVOLUME_H
#define RENDERVOLUME_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include "PipelineAttribute.h"

class RenderVolumeI : public Object
{
public:
	virtual void BindForReading () = 0;
	virtual void BindForWriting () = 0;
	virtual std::vector<PipelineAttribute> GetCustomAttributes () const = 0;
};

#endif