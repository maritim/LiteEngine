#include "AmbientLightVolume.h"

AmbientLightVolume::AmbientLightVolume () :
	_ambientLightIntensity (1.0f),
	_ambientLightColor (Color::Black),
	_ambientOcclusionEnabled (false)
{

}

void AmbientLightVolume::SetAmbientLight (float intensity, const Color& color, bool aoEnabled)
{
	_ambientLightIntensity = intensity;
	_ambientLightColor = color;
	_ambientOcclusionEnabled = aoEnabled;
}

void AmbientLightVolume::BindForReading ()
{
	/*
	 * Nothing
	*/
}

void AmbientLightVolume::BindForWriting ()
{
	/*
	 * Nothing
	*/
}

std::vector<PipelineAttribute> AmbientLightVolume::GetCustomAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute ambientLightIntensity;
	PipelineAttribute ambientLightColor;
	PipelineAttribute ambientOcclusionEnabled;

	ambientLightIntensity.type = PipelineAttribute::AttrType::ATTR_1F;
	ambientLightColor.type = PipelineAttribute::AttrType::ATTR_3F;
	ambientOcclusionEnabled.type = PipelineAttribute::AttrType::ATTR_1I;

	ambientLightIntensity.name = "ambientLightIntensity";
	ambientLightColor.name = "ambientLightColor";
	ambientOcclusionEnabled.name = "ambientOcclusionEnabled";

	ambientLightIntensity.value.x = _ambientLightIntensity;
	ambientLightColor.value = _ambientLightColor.ToVector3 ();
	ambientOcclusionEnabled.value.x = (int) _ambientOcclusionEnabled;

	attributes.push_back (ambientLightIntensity);
	attributes.push_back (ambientLightColor);
	attributes.push_back (ambientOcclusionEnabled);

	return attributes;
}

void AmbientLightVolume::Clear ()
{
	/*
	 * Nothing
	*/
}
