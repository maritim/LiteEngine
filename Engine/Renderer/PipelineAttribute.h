#ifndef PIPELINEATTRIBUTE_H
#define PIPELINEATTRIBUTE_H

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

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
		ATTR_TEXTURE_VIEW_DEPTH,
		ATTR_MATRIX_4X4F,
		ATTR_BLOCK,
		ATTR_SSBO_BLOCK
	};

	AttrType type;
	std::string name;
	glm::vec3 value;
	glm::mat4 matrix;
};

#endif