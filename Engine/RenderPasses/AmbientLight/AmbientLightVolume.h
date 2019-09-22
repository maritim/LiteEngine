#ifndef AMBIENTLIGHTVOLUME_H
#define AMBIENTLIGHTVOLUME_H

#include "Renderer/RenderVolumeI.h"

#include "Utils/Color/Color.h"

class AmbientLightVolume : public RenderVolumeI
{
protected:
	float _ambientLightIntensity;
	Color _ambientLightColor;
	bool _ambientOcclusionEnabled;

public:
	AmbientLightVolume ();

	void SetAmbientLight (float, const Color&, bool);

	void BindForReading ();
	void BindForWriting ();
	std::vector<PipelineAttribute> GetCustomAttributes () const;

	void Clear ();
};

#endif
