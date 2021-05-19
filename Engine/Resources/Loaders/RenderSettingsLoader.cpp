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
		else if (name == "General") {
			ProcessGeneral (content, settings);
		}
		else if (name == "SSAO") {
			ProcessSSAO (content, settings);
		}
		else if (name == "SSDO") {
			ProcessSSDO (content, settings);
		}
		else if (name == "SSR") {
			ProcessSSR (content, settings);
		}
		else if (name == "TAA") {
			ProcessTAA (content, settings);
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
		else if (name == "TRSM") {
			ProcessTRSM (content, settings);
		}
		else if (name == "LPV") {
			ProcessLPV (content, settings);
		}
		else if (name == "VCT") {
			ProcessVCT (content, settings);
		}
		else if (name == "HGI") {
			ProcessHGI (content, settings);
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

void RenderSettingsLoader::ProcessGeneral (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string indirectDiffuseEnabled = xmlElem->Attribute ("indirectDiffuseEnabled");
	std::string indirectSpecularEnabled = xmlElem->Attribute ("indirectSpecularEnabled");
	std::string subsurfaceScatteringEnabled = xmlElem->Attribute ("subsurfaceScatteringEnabled");

	settings->indirect_diffuse_enabled = Extensions::StringExtend::ToBool (indirectDiffuseEnabled);
	settings->indirect_specular_enabled = Extensions::StringExtend::ToBool (indirectSpecularEnabled);
	settings->subsurface_scattering_enabled = Extensions::StringExtend::ToBool (subsurfaceScatteringEnabled);
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
	std::string temporalFilterEnabled = xmlElem->Attribute ("temporalFilterEnabled");

	settings->ssao_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->ssao_scale = std::stof (scale);
	settings->ssao_samples = std::stoul (samples);
	settings->ssao_noise_size = std::stoul (noiseSize);
	settings->ssao_radius = std::stof (radius);
	settings->ssao_bias = std::stof (bias);
	settings->ssao_blur_enabled = Extensions::StringExtend::ToBool (blurEnabled);
	settings->ssao_temporal_filter_enabled = Extensions::StringExtend::ToBool (temporalFilterEnabled);
}

void RenderSettingsLoader::ProcessSSDO (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string temporalFilterEnabled = xmlElem->Attribute ("temporalFilterEnabled");
	std::string scale = xmlElem->Attribute ("scale");
	std::string samples = xmlElem->Attribute ("samples");
	std::string radius = xmlElem->Attribute ("radius");
	std::string bias = xmlElem->Attribute ("bias");
	std::string samplingScale = xmlElem->Attribute ("samplingScale");
	std::string rayShadow = xmlElem->Attribute ("rayShadow");
	std::string shadowScale = xmlElem->Attribute ("shadowScale");
	std::string shadowStride = xmlElem->Attribute ("shadowStride");
	std::string indirectIntensity = xmlElem->Attribute ("indirectIntensity");
	std::string interpolationEnabled = xmlElem->Attribute ("interpolationEnabled");
	std::string interpolationScale = xmlElem->Attribute ("interpolationScale");
	std::string minInterpolationDistance = xmlElem->Attribute ("minInterpolationDistance");
	std::string minInterpolationAngle = xmlElem->Attribute ("minInterpolationAngle");

	settings->ssdo_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->ssdo_temporal_filter_enabled = Extensions::StringExtend::ToBool (temporalFilterEnabled);
	settings->ssdo_scale = std::stof (scale);
	settings->ssdo_samples = std::stoi (samples);
	settings->ssdo_radius = std::stof (radius);
	settings->ssdo_bias = std::stof (bias);
	settings->ssdo_sampling_scale = std::stof (samplingScale);
	settings->ssdo_ray_shadow = Extensions::StringExtend::ToBool (rayShadow);
	settings->ssdo_shadow_scale = std::stof (shadowScale);
	settings->ssdo_shadow_stride = std::stoul (shadowStride);
	settings->ssdo_indirect_intensity = std::stof (indirectIntensity);
	settings->ssdo_interpolation_enabled = Extensions::StringExtend::ToBool (interpolationEnabled);
	settings->ssdo_interpolation_scale = std::stof (interpolationScale);
	settings->ssdo_min_interpolation_distance = std::stof (minInterpolationDistance);
	settings->ssdo_min_interpolation_angle = std::stof (minInterpolationAngle);
}

void RenderSettingsLoader::ProcessSSR (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");
	std::string scale = xmlElem->Attribute ("scale");
	std::string iterations = xmlElem->Attribute ("iterations");
	std::string roughness = xmlElem->Attribute ("roughness");
	std::string thickness = xmlElem->Attribute ("thickness");
	std::string stride = xmlElem->Attribute ("stride");
	std::string intensity = xmlElem->Attribute ("intensity");

	settings->ssr_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->ssr_scale = std::stof (scale);
	settings->ssr_iterations = std::stoul (iterations);
	settings->ssr_roughness = std::stof (roughness);
	settings->ssr_thickness = std::stof (thickness);
	settings->ssr_stride = std::stoul (stride);
	settings->ssr_intensity = std::stof (intensity);
}

void RenderSettingsLoader::ProcessTAA (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");

	settings->taa_enabled = Extensions::StringExtend::ToBool (enabled);
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
	std::string intensity = xmlElem->Attribute ("intensity");

	settings->lut_enabled = Extensions::StringExtend::ToBool (enabled);
	settings->lut_texture_path = path;
	settings->lut_intensity = std::stof (intensity);
}

void RenderSettingsLoader::ProcessGamma (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string enabled = xmlElem->Attribute ("enabled");

	settings->gamma_enabled = Extensions::StringExtend::ToBool (enabled);
}

void RenderSettingsLoader::ProcessRSM (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string scale = xmlElem->Attribute ("scale");
	std::string samples = xmlElem->Attribute ("samples");
	std::string radius = xmlElem->Attribute ("radius");
	std::string aoEnabled = xmlElem->Attribute ("aoEnabled");
	std::string indirectDiffuseIntensity = xmlElem->Attribute ("indirectDiffuseIntensity");
	std::string indirectSpecularIntensity = xmlElem->Attribute ("indirectSpecularIntensity");
	std::string indirectRefractiveIntensity = xmlElem->Attribute ("indirectRefractiveIntensity");
	std::string iterations = xmlElem->Attribute ("iterations");
	std::string thickness = xmlElem->Attribute ("thickness");
	std::string interpolationScale = xmlElem->Attribute ("interpolationScale");
	std::string minInterpolationDistance = xmlElem->Attribute ("minInterpolationDistance");
	std::string minInterpolationAngle = xmlElem->Attribute ("minInterpolationAngle");

	settings->rsm_scale = std::stof (scale);
	settings->rsm_samples = std::stoi (samples);
	settings->rsm_radius = std::stof (radius);
	settings->rsm_ao_enabled = Extensions::StringExtend::ToBool (aoEnabled);
	settings->rsm_indirect_diffuse_intensity = std::stof (indirectDiffuseIntensity);
	settings->rsm_indirect_specular_intensity = std::stof (indirectSpecularIntensity);
	settings->rsm_indirect_refractive_intensity = std::stof (indirectRefractiveIntensity);
	settings->rsm_iterations = std::stoi (iterations);
	settings->rsm_thickness = std::stof (thickness);
	settings->rsm_interpolation_scale = std::stof (interpolationScale);
	settings->rsm_min_interpolation_distance = std::stof (minInterpolationDistance);
	settings->rsm_min_interpolation_angle = std::stof (minInterpolationAngle);
}

void RenderSettingsLoader::ProcessTRSM (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string samples = xmlElem->Attribute ("samples");
	std::string indirectDiffuseIntensity = xmlElem->Attribute ("indirectDiffuseIntensity");
	std::string temporalFilterEnabled = xmlElem->Attribute ("temporalFilterEnabled");
	std::string blurEnabled = xmlElem->Attribute ("blurEnabled");

	settings->trsm_samples = std::stoi (samples);
	settings->trsm_indirect_diffuse_intensity = std::stof (indirectDiffuseIntensity);
	settings->trsm_temporal_filter_enabled = Extensions::StringExtend::ToBool (temporalFilterEnabled);
	settings->trsm_blur_enabled = Extensions::StringExtend::ToBool (blurEnabled);
}

void RenderSettingsLoader::ProcessLPV (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string volumeSize = xmlElem->Attribute ("volumeSize");
	std::string iterations = xmlElem->Attribute ("iterations");
	std::string injectionBias = xmlElem->Attribute ("injectionBias");
	std::string geometryOcclusion = xmlElem->Attribute ("geometryOcclusion");
	std::string indirectDiffuseIntensity = xmlElem->Attribute ("indirectDiffuseIntensity");
	std::string indirectSpecularIntensity = xmlElem->Attribute ("indirectSpecularIntensity");
	std::string indirectRefractiveIntensity = xmlElem->Attribute ("indirectRefractiveIntensity");
	std::string specularIterations = xmlElem->Attribute ("specularIterations");

	settings->lpv_volume_size = std::stoi (volumeSize);
	settings->lpv_iterations = std::stoi (iterations);
	settings->lpv_injection_bias = std::stof (injectionBias);
	settings->lpv_geometry_occlusion = Extensions::StringExtend::ToBool (geometryOcclusion);
	settings->lpv_indirect_diffuse_intensity = std::stof (indirectDiffuseIntensity);
	settings->lpv_indirect_specular_intensity = std::stof (indirectSpecularIntensity);
	settings->lpv_indirect_refractive_intensity = std::stof (indirectRefractiveIntensity);
	settings->lpv_specular_iterations = std::stoi (specularIterations);
}

void RenderSettingsLoader::ProcessVCT (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string voxelsSize = xmlElem->Attribute ("voxelsSize");
	std::string continuousVoxelization = xmlElem->Attribute ("continuousVoxelization");
	std::string bordering = xmlElem->Attribute ("bordering");
	std::string mipmapLevels = xmlElem->Attribute ("mipmapLevels");
	std::string indirectDiffuseIntensity = xmlElem->Attribute ("indirectDiffuseIntensity");
	std::string indirectSpecularIntensity = xmlElem->Attribute ("indirectSpecularIntensity");
	std::string refractiveIndirectIntensity = xmlElem->Attribute ("refractiveIndirectIntensity");
	std::string diffuseConeDistance = xmlElem->Attribute ("diffuseConeDistance");
	std::string diffuseOriginBias = xmlElem->Attribute ("diffuseOriginBias");
	std::string specularConeRatio = xmlElem->Attribute ("specularConeRatio");
	std::string specularConeDistance = xmlElem->Attribute ("specularConeDistance");
	std::string specularOriginBias = xmlElem->Attribute ("specularOriginBias");
	std::string refractiveConeRatio = xmlElem->Attribute ("refractiveConeRatio");
	std::string refractiveConeDistance = xmlElem->Attribute ("refractiveConeDistance");
	std::string aoEnabled = xmlElem->Attribute ("aoEnabled");
	std::string aoConeRatio = xmlElem->Attribute ("aoConeRatio");
	std::string aoConeDistance = xmlElem->Attribute ("aoConeDistance");
	std::string shadowConeEnabled = xmlElem->Attribute ("shadowConeEnabled");
	std::string shadowConeRatio = xmlElem->Attribute ("shadowConeRatio");
	std::string shadowConeDistance = xmlElem->Attribute ("shadowConeDistance");
	std::string originBias = xmlElem->Attribute ("originBias");
	std::string temporalFilterEnabled = xmlElem->Attribute ("temporalFilterEnabled");

	settings->vct_voxels_size = std::stoi (voxelsSize);
	settings->vct_continuous_voxelization = Extensions::StringExtend::ToBool (continuousVoxelization);
	settings->vct_bordering = Extensions::StringExtend::ToBool (bordering);
	settings->vct_mipmap_levels = std::stoi (mipmapLevels);
	settings->vct_indirect_diffuse_intensity = std::stof (indirectDiffuseIntensity);
	settings->vct_indirect_specular_intensity = std::stof (indirectSpecularIntensity);
	settings->vct_indirect_refractive_intensity = std::stof (refractiveIndirectIntensity);
	settings->vct_diffuse_cone_distance = std::stof (diffuseConeDistance);
	settings->vct_diffuse_origin_bias = std::stof (diffuseOriginBias);
	settings->vct_specular_cone_ratio = std::stof (specularConeRatio);
	settings->vct_specular_cone_distance = std::stof (specularConeDistance);
	settings->vct_specular_origin_bias = std::stof (specularOriginBias);
	settings->vct_refractive_cone_distance = std::stof (refractiveConeDistance);
	settings->vct_refractive_cone_ratio = std::stof (refractiveConeRatio);
	settings->vct_ao_enabled = Extensions::StringExtend::ToBool (aoEnabled);
	settings->vct_ao_cone_ratio = std::stof (aoConeRatio);
	settings->vct_ao_cone_distance = std::stof (aoConeDistance);
	settings->vct_shadow_cone_enabled = Extensions::StringExtend::ToBool (shadowConeEnabled);
	settings->vct_shadow_cone_ratio = std::stof (shadowConeRatio);
	settings->vct_shadow_cone_distance = std::stof (shadowConeDistance);
	settings->vct_origin_bias = std::stof (originBias);
	settings->vct_temporal_filter_enabled = Extensions::StringExtend::ToBool (temporalFilterEnabled);
}

void RenderSettingsLoader::ProcessHGI (TiXmlElement* xmlElem, RenderSettings* settings)
{
	std::string temporalFilterEnabled = xmlElem->Attribute ("temporalFilterEnabled");
	std::string rsmSamples = xmlElem->Attribute ("rsmSamples");
	std::string rsmRadius = xmlElem->Attribute ("rsmRadius");
	std::string ssdoSamples = xmlElem->Attribute ("ssdoSamples");
	std::string ssdoRadius = xmlElem->Attribute ("ssdoRadius");
	std::string ssdoIndirectDiffuseIntensity = xmlElem->Attribute ("ssdoIndirectDiffuseIntensity");
	std::string rsmIndirectDiffuseIntensity = xmlElem->Attribute ("rsmIndirectDiffuseIntensity");
	std::string ssdoSamplingScale = xmlElem->Attribute ("ssdoSamplingScale");
	std::string interpolationEnabled = xmlElem->Attribute ("interpolationEnabled");
	std::string interpolationScale = xmlElem->Attribute ("interpolationScale");
	std::string minInterpolationDistance = xmlElem->Attribute ("minInterpolationDistance");
	std::string minInterpolationAngle = xmlElem->Attribute ("minInterpolationAngle");
	std::string rsmThickness = xmlElem->Attribute ("rsmThickness");
	std::string rsmIndirectSpecularIntensity = xmlElem->Attribute ("rsmIndirectSpecularIntensity");
	std::string ssrIndirectSpecularIntensity = xmlElem->Attribute ("ssrIndirectSpecularIntensity");
	std::string aoEnabled = xmlElem->Attribute ("aoEnabled");
	std::string aoSamples = xmlElem->Attribute ("aoSamples");
	std::string aoRadius = xmlElem->Attribute ("aoRadius");
	std::string aoBias = xmlElem->Attribute ("aoBias");
	std::string aoBlend = xmlElem->Attribute ("aoBlend");

	settings->hgi_temporal_filter_enabled = Extensions::StringExtend::ToBool (temporalFilterEnabled);
	settings->hgi_rsm_samples = std::stoi (rsmSamples);
	settings->hgi_rsm_radius = std::stof (rsmRadius);
	settings->hgi_ssdo_samples = std::stoi (ssdoSamples);
	settings->hgi_ssdo_radius = std::stof (ssdoRadius);
	settings->hgi_rsm_indirect_diffuse_intensity = std::stof (rsmIndirectDiffuseIntensity);
	settings->hgi_ssdo_indirect_diffuse_intensity = std::stof (ssdoIndirectDiffuseIntensity);
	settings->hgi_ssdo_sampling_scale = std::stof (ssdoSamplingScale);
	settings->hgi_interpolation_enabled = Extensions::StringExtend::ToBool (interpolationEnabled);
	settings->hgi_interpolation_scale = std::stof (interpolationScale);
	settings->hgi_min_interpolation_distance = std::stof (minInterpolationDistance);
	settings->hgi_min_interpolation_angle = std::stof (minInterpolationAngle);
	settings->hgi_rsm_thickness = std::stof (rsmThickness);
	settings->hgi_rsm_indirect_specular_intensity = std::stof (rsmIndirectSpecularIntensity);
	settings->hgi_ssr_indirect_specular_intensity = std::stof (ssrIndirectSpecularIntensity);
	settings->hgi_ao_enabled = Extensions::StringExtend::ToBool (aoEnabled);
	settings->hgi_ao_samples = std::stoi (aoSamples);
	settings->hgi_ao_radius = std::stof (aoRadius);
	settings->hgi_ao_bias = std::stof (aoBias);
	settings->hgi_ao_blend = std::stof (aoBlend);
}
