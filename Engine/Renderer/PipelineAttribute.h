#ifndef PIPELINEATTRIBUTE_H
#define PIPELINEATTRIBUTE_H

#include <vector>
#include <string>

#include "Core/Math/Vector3.h"
#include "Core/Math/glm/glm.hpp"

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
		ATTR_TEXTURE_CUBE,
		ATTR_MATRIX_4X4F
	};

	AttrType type;
	std::string name;
	Vector3 value;
	glm::mat4 matrix;
};

#endif