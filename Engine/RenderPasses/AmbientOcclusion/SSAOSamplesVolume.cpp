#include "SSAOSamplesVolume.h"

#include "Core/Random/Random.h"

#include "Utils/Extensions/MathExtend.h"

SSAOSamplesVolume::SSAOSamplesVolume () :
	_samples ()
{

}

bool SSAOSamplesVolume::Init (std::size_t samplesCount)
{
	/*
	 * Initialize samples
	*/

	_samples.reserve (samplesCount);

	for (std::size_t index = 0; index < samplesCount; index++) {
		glm::vec3 sample (
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			Random::Instance ()->RangeF (0.0f, 1.0f)
		);

		/*
		 * Normalize and multiply with random value in [0, 1) to
		 * keep the value inside the hemisphere
		*/

		sample = glm::normalize (sample);
		sample *= Random::Instance ()->RangeF (0.0f, 1.0f);

		// float scale = (float) index / samplesCount;
		// scale = 0.1f + 0.9f * scale * scale;
		// sample *= scale;

		_samples.push_back (sample);
	}

	return true;
}

void SSAOSamplesVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void SSAOSamplesVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> SSAOSamplesVolume::GetCustomAttributes () const
{
	/*
	 * TODO: change this to ssbo
	*/

	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssaoSamplesCount;

	ssaoSamplesCount.type = PipelineAttribute::AttrType::ATTR_1I;

	ssaoSamplesCount.name = "ssaoSamplesCount";

	ssaoSamplesCount.value.x = _samples.size ();

	attributes.push_back (ssaoSamplesCount);

	for (std::size_t index = 0; index < _samples.size(); index++) {
		PipelineAttribute ssaoSample;

		ssaoSample.type = PipelineAttribute::AttrType::ATTR_3F;

		ssaoSample.name = "ssaoSample[" + std::to_string (index) + "]";

		ssaoSample.value = _samples [index];

		attributes.push_back (ssaoSample);
	}

	return attributes;
}

void SSAOSamplesVolume::Clear ()
{
	_samples.clear ();
}