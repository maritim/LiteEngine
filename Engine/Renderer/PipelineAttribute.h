#ifndef PIPELINEATTRIBUTE_H
#define PIPELINEATTRIBUTE_H

#include <vector>
#include <string>

#include "Core/Math/glm/vec3.hpp"
#include "Core/Math/glm/gtc/matrix_transform.hpp"

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
		ATTR_TEXTURE_3D,
		ATTR_TEXTURE_CUBE,
		ATTR_MATRIX_4X4F
	};

	AttrType type;
	std::string name;
	glm::vec3 value;
	glm::mat4 matrix;
};

#endif