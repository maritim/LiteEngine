#ifndef PIPELINEATTRIBUTE_H
#define PIPELINEATTRIBUTE_H

#include <vector>
#include <string>

#include "Core/Math/Vector3.h"

struct PipelineAttribute
{
	enum AttrType { 
		ATTR_1I, 
		ATTR_2I, 
		ATTR_3I, 
		ATTR_1F, 
		ATTR_2F,
		ATTR_3F,
		ATTR_TEXTURE_2D,
		ATTR_TEXTURE_CUBE
	};

	AttrType type;
	std::string name;
	Vector3 value;
};

#endif