#include "NoiseMapVolume.h"

#include <glm/vec3.hpp>

#include "Wrappers/OpenGL/GL.h"

#include "Core/Random/Random.h"

NoiseMapVolume::NoiseMapVolume () :
	_noiseMap (0),
	_noiseWidth (0),
	_noiseHeight (0)
{

}

NoiseMapVolume::~NoiseMapVolume ()
{

}

bool NoiseMapVolume::Init (std::size_t width, std::size_t height)
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

void NoiseMapVolume::BindForReading ()
{
	GL::ActiveTexture (GL_TEXTURE10);
	GL::BindTexture (GL_TEXTURE_2D, _noiseMap);
}

void NoiseMapVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> NoiseMapVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute noiseMap;
	PipelineAttribute noiseSize;

	noiseMap.type = PipelineAttribute::AttrType::ATTR_1I;
	noiseSize.type = PipelineAttribute::AttrType::ATTR_2F;

	noiseMap.name = "noiseMap";
	noiseSize.name = "noiseSize";

	noiseMap.value.x = 10;
	noiseSize.value = glm::vec3 (_noiseWidth, _noiseHeight, 0.0f);

	attributes.push_back (noiseMap);
	attributes.push_back (noiseSize);

	return attributes;
}

unsigned int NoiseMapVolume::GetColorBuffer () const
{
	return _noiseMap;
}

glm::ivec2 NoiseMapVolume::GetSize () const
{
	return glm::ivec2 (_noiseWidth, _noiseHeight);
}

void NoiseMapVolume::Clear ()
{
	/*
	 * Delete noise texture
	*/

	GL::DeleteTextures (1, &_noiseMap);
}
