#ifdef _MSC_VER
	#define _USE_MATH_DEFINES
#endif

#include "ReflectiveShadowMapSamplesVolume.h"

#include "Core/Random/Random.h"

ReflectiveShadowMapSamplesVolume::ReflectiveShadowMapSamplesVolume ()
	: _samples ()
{
}

bool ReflectiveShadowMapSamplesVolume::Init (std::size_t samplesCount)
{
	_samples.resize (samplesCount);

	float sampleRadius = 0.5f;

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec2 sample;

		float radius = std::sqrt (Random::Instance ()->RangeF (0, 1.0f));
		float angle = Random::Instance ()->RangeF (0, 2 * M_PI);

		sample.x = sampleRadius * radius * cos (angle);
		sample.y = sampleRadius * radius * sin (angle);

		_samples.push_back (sample);
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

std::vector<PipelineAttribute> ReflectiveShadowMapSamplesVolume::GetCustomAttributes () const
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