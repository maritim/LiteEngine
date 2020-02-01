#include "TRSMSamplesVolume.h"

#include <cstring>

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Sequences/HaltonGenerator.h"

bool TRSMSamplesVolume::Init (std::size_t samplesCount)
{
	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	HaltonGenerator haltonGenerator (2, 3);

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec2 sample = haltonGenerator.Next () * 2.0f - 1.0f;

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	return true;
}
