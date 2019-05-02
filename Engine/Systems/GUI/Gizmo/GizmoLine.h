#ifndef GIZMOLINE_H
#define GIZMOLINE_H

#include <glm/vec3.hpp>

struct GizmoLine
{
	glm::vec3 first;
	glm::vec3 last;
	glm::vec3 color;
	bool depthTest;
};

#endif