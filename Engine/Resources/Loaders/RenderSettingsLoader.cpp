#include "RenderSettingsLoader.h"

#include "Core/Console/Console.h"

#include "Utils/Extensions/StringExtend.h"

Object* RenderSettingsLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		Console::LogError (filename + " has error in its syntax. Could not preceed further.");
		return nullptr;
	}

	TiXmlElement* root = doc.FirstChildElement ("RenderSettings");

	if (root == nullptr) {
		return nullptr;
	}

	RenderSettings* settings = new RenderSettings ();

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "RenderMode") {
			ProcessRenderMode (content, settings);
		}
		else if (name == "SSAO") {
			ProcessSSAO (content, settings);
		}
		else if (name == "SSR") {
			ProcessSSR (content, settings);
		}
		else if (name == "Bloom") {
			ProcessBloom (content, settings);
		}
		else if (name == "HDR") {
			ProcessHDR (content, settings);
		}
		else if (name == "LUT") {
			ProcessLUT (content, settings);
		}
		else if (name == "Gamma") {
			ProcessGamma (content, settings);
		}
		else if (name == "RSM") {
			ProcessRSM (content, settings);
		}
		else if (name == "VCT") {
			ProcessVCT (content, settings);
		}
		else if (name == "SSDO") {
			ProcessSSDO (content, settings);
		}

		content = content->NextSiblingElement ();
	}

	settings->name = filename;

	doc.Clear ();

	return settings;
}

void RenderSettingsLoader::ProcessRenderMode (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string renderMode = xmlElem->Attribute ("mode");

	settings->renderMode = renderMode;
}

void RenderSettingsLoader::ProcessSSAO (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string scale = xmlElem->Attribute ("scale");
	std::string samples = xmlElem->Attribute ("samples");
	std::string noiseSize = xmlElem->Attribute ("noiseSize");
	std::string radius = xmlElem->Attribute ("radius");
	std::string bias = xmlElem->Attribute ("bias");
	std::string blurEnabled = xmlElem->Attribute ("blurEnabled");

	settings->ssao_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->ssao_scale = std::stof (scale);
	settings->ssao_samples = std::stoul (samples);
	settings->ssao_noise_size = std::stoul (noiseSize);
	settings->ssao_radius = std::stof (radius);
	settings->ssao_bias = std::stof (bias);
	settings->ssao_blur_enabled = Extensions::StringExtend::ToBool (blurEnabled);
}

void RenderSettingsLoader::ProcessSSR (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string scale = xmlElem->Attribute ("scale");
	std::string iterations = xmlElem->Attribute ("iterations");
	std::string roughness = xmlElem->Attribute ("roughness");
	std::string sampleSkip = xmlElem->Attribute ("sampleSkip");
	std::string spatialBias = xmlElem->Attribute ("spatialBias");
	std::string intensity = xmlElem->Attribute ("intensity");

	settings->ssr_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->ssr_scale = std::stof (scale);
	settings->ssr_iterations = std::stoul (iterations);
	settings->ssr_roughness = std::stof (roughness);
	settings->ssr_sample_skip = std::stof (sampleSkip);
	settings->ssr_spatial_bias = std::stof (spatialBias);
	settings->ssr_intensity = std::stof (intensity);
}

void RenderSettingsLoader::ProcessBloom (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string scale = xmlElem->Attribute ("scale");
	std::string threshold = xmlElem->Attribute ("threshold");
	std::string intensity = xmlElem->Attribute ("intensity");

	settings->bloom_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->bloom_scale = std::stof (scale);
	settings->bloom_threshold = std::stof (threshold);
	settings->bloom_intensity = std::stof (intensity);
}

void RenderSettingsLoader::ProcessHDR (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string exposure = xmlElem->Attribute ("exposure");

	settings->hdr_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->hdr_exposure = std::stof (exposure);
}

void RenderSettingsLoader::ProcessLUT (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string path = xmlElem->Attribute ("path");

	settings->lut_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->lut_texture_path = path;
}

void RenderSettingsLoader::ProcessGamma (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");

	settings->gamma_enabled = Extensions::StringExtend::ToBool (enabled);
}

void RenderSettingsLoader::ProcessRSM (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string resolution = xmlElem->Attribute ("resolution");
	std::string bias = xmlElem->Attribute ("bias");
	std::string samples = xmlElem->Attribute ("samples");
	std::string radius = xmlElem->Attribute ("radius");
	std::string intensity = xmlElem->Attribute ("intensity");
	std::string caching = xmlElem->Attribute ("caching");

	auto vresolution = Extensions::StringExtend::Split (resolution, ",");

	settings->rsm_resolution = glm::ivec2 (std::stoi (vresolution [0]), std::stoi (vresolution [1]));
	settings->rsm_bias = std::stof (bias);
	settings->rsm_samples = std::stoi (samples);
	settings->rsm_radius = std::stof (radius);
	settings->rsm_intensity = std::stof (intensity);
	settings->rsm_caching = Extensions::StringExtend::ToBool (caching);
}

void RenderSettingsLoader::ProcessVCT (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string voxelsSize = xmlElem->Attribute ("voxelsSize");
	std::string continuousVoxelization = xmlElem->Attribute ("continuousVoxelization");
	std::string bordering = xmlElem->Attribute ("bordering");
	std::string mipmapLevels = xmlElem->Attribute ("mipmapLevels");
	std::string indirectIntensity = xmlElem->Attribute ("indirectIntensity");
	std::string diffuseConeDistance = xmlElem->Attribute ("diffuseConeDistance");
	std::string specularConeDistance = xmlElem->Attribute ("specularConeDistance");
	std::string shadowConeRatio = xmlElem->Attribute ("shadowConeRatio");
	std::string shadowConeDistance = xmlElem->Attribute ("shadowConeDistance");

	settings->vct_voxels_size = std::stoi (voxelsSize);
	settings->vct_continuous_voxelization = Extensions::StringExtend::ToBool (continuousVoxelization);
	settings->vct_bordering = Extensions::StringExtend::ToBool (bordering);
	settings->vct_mipmap_levels = std::stoi (mipmapLevels);
	settings->vct_indirect_intensity = std::stof (indirectIntensity);
	settings->vct_diffuse_cone_distance = std::stof (diffuseConeDistance);
	settings->vct_specular_cone_distance = std::stof (specularConeDistance);
	settings->vct_shadow_cone_ratio = std::stof (shadowConeRatio);
	settings->vct_shadow_cone_distance = std::stof (shadowConeDistance);
}

void RenderSettingsLoader::ProcessSSDO (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string scale = xmlElem->Attribute ("scale");
	std::string samples = xmlElem->Attribute ("samples");
	std::string radius = xmlElem->Attribute ("radius");
	std::string bias = xmlElem->Attribute ("bias");
	std::string indirectIntensity = xmlElem->Attribute ("indirectIntensity");

	settings->ssdo_scale = std::stof (scale);
	settings->ssdo_samples = std::stoi (samples);
	settings->ssdo_radius = std::stof (radius);
	settings->ssdo_bias = std::stof (bias);
	settings->ssdo_indirect_intensity = std::stof (indirectIntensity);
}