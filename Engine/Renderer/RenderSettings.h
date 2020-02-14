#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

#include "Core/Interfaces/Object.h"

#include <string>
#include <glm/vec2.hpp>

struct Framebuffer
{
	std::size_t width;
	std::size_t height;
};

struct Viewport
{
	std::size_t x;
	std::size_t y;
	std::size_t width;
	std::size_t height;
};

struct RenderSettings : public Object
{
	std::string name;

	std::string renderMode;
	Framebuffer framebuffer;
	Viewport viewport;

	bool ssao_enabled;
	float ssao_scale;
	std::size_t ssao_samples;
	std::size_t ssao_noise_size;
	float ssao_radius;
	float ssao_bias;
	bool ssao_blur_enabled;

	bool ssr_enabled;
	float ssr_scale;
	std::size_t ssr_iterations;
	float ssr_roughness;
	float ssr_thickness;
	std::size_t ssr_stride;
	float ssr_intensity;

	bool taa_enabled;

	bool bloom_enabled;
	float bloom_scale;
	float bloom_threshold;
	float bloom_intensity;

	bool hdr_enabled;
	float hdr_exposure;

	bool lut_enabled;
	std::string lut_texture_path;
	float lut_intensity;

	bool gamma_enabled;

	float rsm_scale;
	std::size_t rsm_samples;
	float rsm_radius;
	float rsm_intensity;

	bool trsm_temporal_filter_enabled;
	bool trsm_blur_enabled;

	std::size_t lpv_volume_size;
	std::size_t lpv_iterations;
	float lpv_injection_bias;
	float lpv_intensity;

	std::size_t vct_voxels_size;
	bool vct_continuous_voxelization;
	bool vct_bordering;
	std::size_t vct_mipmap_levels;
	float vct_indirect_intensity;
	float vct_indirect_refractive_intensity;
	float vct_diffuse_cone_distance;
	float vct_specular_cone_distance;
	float vct_refractive_cone_ratio;
	float vct_refractive_cone_distance;
	float vct_shadow_cone_ratio;
	float vct_shadow_cone_distance;
	bool vct_debug_show_voxels;
	std::size_t vct_debug_volume_mipmap_level;

	float ssdo_scale;
	float ssdo_shadow_scale;
	std::size_t ssdo_samples;
	float ssdo_radius;
	float ssdo_bias;
	std::size_t ssdo_shadow_stride;
	float ssdo_indirect_intensity;
};

#endif