#include "LightMapModel.h"

LightMapModel::LightMapModel () :
	Model (),
	_haveLightMapUV (false),
	_lmTexcoords ()
{

}

bool LightMapModel::HaveLightMapUV () const
{
	return _haveLightMapUV;
}

std::size_t LightMapModel::LightMapTexcoordsCount () const
{
	return _lmTexcoords.size ();
}

void LightMapModel::AddLightMapTexcoord (const glm::vec2& lmTexcoord)
{
	_lmTexcoords.push_back (lmTexcoord);

	_haveLightMapUV = true;
}

glm::vec2 LightMapModel::GetLightMapTexcoord (std::size_t position) const
{
	if (position >= _lmTexcoords.size()) {
		Console::LogWarning ("Light Map Texcoord index exceed texcoords count. \
			You are searching for " + std::to_string (position) + " and the size is " +
			std::to_string (_lmTexcoords.size ()));

		return glm::vec2 ();
	}

	return _lmTexcoords [position];
}