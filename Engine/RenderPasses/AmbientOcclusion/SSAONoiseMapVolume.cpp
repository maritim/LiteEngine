#include "SSAONoiseMapVolume.h"

#include <glm/vec3.hpp>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Random/Random.h"

SSAONoiseMapVolume::SSAONoiseMapVolume () :
	_noiseMap (0),
	_noiseWidth (0),
	_noiseHeight (0)
{

}

SSAONoiseMapVolume::~SSAONoiseMapVolume ()
{

}

bool SSAONoiseMapVolume::Init (std::size_t width, std::size_t height)
{
	/*
	 * Keep new current noise map size
	*/

	_noiseWidth = width;
	_noiseHeight = height;

	/*
	 * Generate noise values
	*/

	std::vector<glm::vec3> ssaoNoise;

	std::size_t pixelsCount = _noiseWidth * _noiseHeight;

	for (std::size_t pixelIndex = 0; pixelIndex < pixelsCount; pixelIndex ++) {
		glm::vec3 noise (
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			Random::Instance ()->RangeF (-1.0f, 1.0f),
			0.0f
		);

		ssaoNoise.push_back (noise);
	}

	/*
	 * Create noise texture to keep the random vector directions
	*/

	GL::GenTextures (1, &_noiseMap);
	GL::BindTexture (GL_TEXTURE_2D, _noiseMap);
	GL::TexImage2D (GL_TEXTURE_2D, 0, GL_RGB16F, _noiseWidth, _noiseHeight, 0, GL_RGB, GL_FLOAT, ssaoNoise.data ());
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return true;
}

void SSAONoiseMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE10);
	GL::BindTexture (GL_TEXTURE_2D, _noiseMap);
}

void SSAONoiseMapVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> SSAONoiseMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ssaoNoiseMap;
	PipelineAttribute ssaoNoiseSize;

	ssaoNoiseMap.type = PipelineAttribute::AttrType::ATTR_1I;
	ssaoNoiseSize.type = PipelineAttribute::AttrType::ATTR_2F;

	ssaoNoiseMap.name = "ssaoNoiseMap";
	ssaoNoiseSize.name = "ssaoNoiseSize";

	ssaoNoiseMap.value.x = 10;
	ssaoNoiseSize.value = glm::vec3 (_noiseWidth, _noiseHeight, 0.0f);

	attributes.push_back (ssaoNoiseMap);
	attributes.push_back (ssaoNoiseSize);

	return attributes;
}

void SSAONoiseMapVolume::Clear ()
{
	/*
	 * Delete noise texture
	*/

	GL::DeleteTextures (1, &_noiseMap);
}
