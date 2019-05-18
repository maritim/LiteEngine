#ifndef VECTOREXTEND_H
#define VECTOREXTEND_H

#include <glm/vec3.hpp>

namespace Extensions
{
	class ENGINE_API VectorExtend
	{
	public:
		static glm::vec3 Cross (const glm::vec3&, const glm::vec3&, const glm::vec3&);
	};
}

#endif