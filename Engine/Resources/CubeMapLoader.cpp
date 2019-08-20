#include "CubeMapLoader.h"

#include "Texture/CubeMap.h"

#include "Texture/CubeMapFace.h"

#include "Resources/Resources.h"

Object* CubeMapLoader::Load(const std::string& filename)
{
	CubeMap* cubeMap = new CubeMap (filename);

	for (std::size_t faceIndex = 0; faceIndex < CUBE_MAP_FACE_COUNT; faceIndex ++) {
		Resource<Texture> texture = Resources::LoadTexture (_filenames [faceIndex]);

		cubeMap->AddTexture (texture);
	}

	return cubeMap;
}

void CubeMapLoader::SetFilenames(std::vector<std::string> filenames)
{
	_filenames = filenames;
}