#ifndef HALTONGENERATOR_H
#define HALTONGENERATOR_H

#include "Core/Interfaces/Object.h"

#include <glm/vec2.hpp>

class HaltonGenerator : public Object
{
protected:
	std::size_t _baseX;
	std::size_t _baseY;
	std::size_t _currentIndex;

public:
	HaltonGenerator (std::size_t baseX, std::size_t baseY);

	glm::vec2 Next ();
protected:
	float Corput (std::size_t index, std::size_t base);
};

#endif