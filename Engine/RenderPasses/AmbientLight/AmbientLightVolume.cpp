#include "AmbientLightVolume.h"

void AmbientLightVolume::SetAmbientLight (float intensity, const Color& color, bool aoEnabled)
{
	/*
	 * Update attributes
	*/

	_attributes.clear ();

	PipelineAttribute ambientLightIntensity;
	PipelineAttribute ambientLightColor;
	PipelineAttribute ambientOcclusionEnabled;

	ambientLightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	ambientOcclusionEnabled.type = PipelineAttribute::AttrType::ATTR_1I;

	ambientLightIntensity.name = "ambientLightIntensity";
	ambientLightColor.name = "ambientLightColor";
	ambientOcclusionEnabled.name = "ambientOcclusionEnabled";

	ambientLightIntensity.value.x = intensity;
	ambientLightColor.value = color.ToVector3 ();
	ambientOcclusionEnabled.value.x = (int) aoEnabled;

	_attributes.push_back (ambientLightIntensity);
	_attributes.push_back (ambientLightColor);
	_attributes.push_back (ambientOcclusionEnabled);
}

const std::vector<PipelineAttribute>& AmbientLightVolume::GetCustomAttributes () const
{
	return _attributes;
}
