#include "ReflectiveShadowMapSamplesVolume.h"

#include "Core/Random/Random.h"

ReflectiveShadowMapSamplesVolume::ReflectiveShadowMapSamplesVolume ()
	: _samples ()
{
}

bool ReflectiveShadowMapSamplesVolume::Init (std::size_t samplesCount)
{
	_samples.resize (samplesCount);

	float radius = 0.5f;

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec2 sample;

		while (true) {
			sample = glm::vec2(
				Random::Instance()->RangeF(0.0f, 1.0f),
				Random::Instance()->RangeF(0.0f, 1.0f)
			);

			float sampleRadius = glm::distance (glm::vec2 (0.5f, 0.5f), sample);

			if (sampleRadius < radius) {
				break;
			}
		}

		_samples [index] = sample;
	}

	return true;
}

void ReflectiveShadowMapSamplesVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void ReflectiveShadowMapSamplesVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> ReflectiveShadowMapSamplesVolume::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute rsmSamplesCount;

	rsmSamplesCount.type = PipelineAttribute::AttrType::ATTR_1I;

	rsmSamplesCount.name = "rsmSamplesCount";

	rsmSamplesCount.value.x = _samples.size ();

	attributes.push_back (rsmSamplesCount);

	for (std::size_t index = 0; index < _samples.size(); index++) {
		PipelineAttribute rsmSample;

		rsmSample.type = PipelineAttribute::AttrType::ATTR_2F;

		rsmSample.name = "rsmSample[" + std::to_string (index) + "]";

		rsmSample.value = glm::vec3 (_samples [index], 0.0f);

		attributes.push_back (rsmSample);
	}

	return attributes;
}