#include "HaltonGenerator.h"

HaltonGenerator::HaltonGenerator (std::size_t baseX, std::size_t baseY) :
	_baseX (baseX),
	_baseY (baseY),
	_currentIndex (1)
{

}

glm::vec2 HaltonGenerator::Next ()
{
	glm::vec2 x = glm::vec2 (
		Corput (_currentIndex, _baseX),
		Corput (_currentIndex, _baseY)
	);

	++ _currentIndex;

	return x;
}

float HaltonGenerator::Corput (std::size_t index, std::size_t base)
{
	float q = 0;
	float bk = (float) 1 / base;

	int n = index;

	while (n > 0) {
		q += (n % base) * bk;
		n /= base;
		bk /= base;
	}

	return q;
}
