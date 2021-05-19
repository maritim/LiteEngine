#include "SSAOSamplesVolume.h"

#include <cstring>

#include "Core/Random/Random.h"

#include "Wrappers/OpenGL/GL.h"

#include "Utils/Sequences/HaltonGenerator.h"

SSAOSamplesVolume::SSAOSamplesVolume (std::size_t samplesCount) :
	_samplesUBO (0)
{
	/*
	 * Create samples
	*/

	std::memset (&_samples, 0, sizeof (_samples));

	_samples.samplesCount = samplesCount;

	HaltonGenerator haltonGenerator (2, 3);

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec3 sample = glm::vec3 (
			haltonGenerator.Next () * 2.0f - 1.0f,
			haltonGenerator.Next ().x
		);

		/*
		 * Normalize and multiply with random value in [0, 1) to
		 * keep the value inside the hemisphere
		*/

		sample = glm::normalize (sample);
		sample *= haltonGenerator.Next ().x;

		// float scale = (float) index / samplesCount;
		// scale = 0.1f + 0.9f * scale * scale;
		// sample *= scale;

		_samples.samples [index * 4] = sample.x;
		_samples.samples [index * 4 + 1] = sample.y;
		_samples.samples [index * 4 + 2] = sample.z;
	}

	GL::GenBuffers (1, &_samplesUBO);
	GL::BindBuffer (GL_UNIFORM_BUFFER, _samplesUBO);
	GL::BufferData (GL_UNIFORM_BUFFER, sizeof (_samples), &_samples, GL_STATIC_DRAW);
	GL::BindBuffer (GL_UNIFORM_BUFFER, 0);

	/*
	 * Create attributes
	*/

	PipelineAttribute ssaoSamples;

	ssaoSamples.type = PipelineAttribute::AttrType::ATTR_BLOCK;

	ssaoSamples.name = "ssaoSamples";

	ssaoSamples.value.x = _samplesUBO;

	_attributes.push_back (ssaoSamples);
}

SSAOSamplesVolume::~SSAOSamplesVolume ()
{
	GL::DeleteBuffers (1, &_samplesUBO);
}

const std::vector<PipelineAttribute>& SSAOSamplesVolume::GetCustomAttributes () const
{
	return _attributes;
}

std::size_t SSAOSamplesVolume::GetSamplesCount () const
{
	return _samples.samplesCount;
}
