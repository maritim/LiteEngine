#include "RSMDirectionalLightVolume.h"

#include "Renderer/Pipeline.h"
#include "Core/Console/Console.h"

RSMDirectionalLightVolume::RSMDirectionalLightVolume () :
	_shadowMapBuffers (nullptr)
{

}

RSMDirectionalLightVolume::~RSMDirectionalLightVolume ()
{

}


bool RSMDirectionalLightVolume::Init (const glm::ivec2& size, std::size_t cascadedLevels)
{
	/*
	 * Initialize resolution
	*/

	_size = size;

	/*
	 * Initialize
	*/

	bool result = Init (cascadedLevels);

	return result;
}

bool RSMDirectionalLightVolume::Init (std::size_t cascadedLevels)
{
	/*
	 * Initialize cascaded levels
	*/

	_cascadedLevels = cascadedLevels;

	/*
	 * Allocate memory for cascaded levels
	*/

	_shadowMapBuffers = new RSMBuffer* [_cascadedLevels];

	/*
	 * Initialize reflective shadow map buffers
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapBuffers [index] = new RSMBuffer();

		if (_shadowMapBuffers [index]->Init (_size) == false) {
			return false;
		}
	}

	return true;
}

void RSMDirectionalLightVolume::BindForShadowMapCatch (std::size_t cascadedLevel)
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return;
	}

	/*
	 * Change resolution on viewport as shadow map size
	*/

	glm::ivec2 size = _shadowMapBuffers [cascadedLevel]->GetSize ();
	GL::Viewport (0, 0, size.x, size.y);

	/*
	 * Bind reflective shadow map buffer
	*/
	
	_shadowMapBuffers [cascadedLevel]->BindForWriting ();
}

void RSMDirectionalLightVolume::EndDrawing ()
{
	Pipeline::UnlockShader ();

	GL::DrawBuffer(GL_NONE);
	GL::ReadBuffer(GL_NONE);

	GL::BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RSMDirectionalLightVolume::BindForReading ()
{
	/*
	 * Bind every depth buffer for reading
	 * TODO: Extend this to cascaded reflective shadow mapping
	*/

	_shadowMapBuffers [0]->BindForReading ();
}

RSMBuffer* RSMDirectionalLightVolume::GetCascade (std::size_t cascadedLevel) const
{
	/*
	 * Check if cascaded level excedes the maximum level
	*/

	if (cascadedLevel >= _cascadedLevels) {
		Console::LogWarning ("There is not level " + std::to_string (cascadedLevel) + " on directional shadow map");
		return nullptr;
	}

	return _shadowMapBuffers [cascadedLevel];
}

glm::ivec2 RSMDirectionalLightVolume::GetSize () const
{
	return _size;
}

void RSMDirectionalLightVolume::Clear ()
{
	/*
	 * Clear shadow map buffers
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		_shadowMapBuffers [index]->Clear ();
	}

	/*
	 * Delete shadow map buffers
	*/

	for (std::size_t index = 0; index < _cascadedLevels; index ++) {
		delete _shadowMapBuffers [index];
	}

	delete[] _shadowMapBuffers;
}
