#ifndef LIGHTMAPMODEL_H
#define LIGHTMAPMODEL_H

#include "Model.h"

class LightMapModel : public Model
{
protected:
	bool _haveLightMapUV;
	std::vector<glm::vec2> _lmTexcoords;

public:
	LightMapModel ();

	bool HaveLightMapUV () const;

	std::size_t LightMapTexcoordsCount () const;

	void AddLightMapTexcoord (const glm::vec2& lmTexcoord);
	glm::vec2 GetLightMapTexcoord (std::size_t position) const;
};

#endif