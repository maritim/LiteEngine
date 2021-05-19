#include "EditorRenderingSettings.h"

#include <experimental/filesystem>
#include <glm/vec2.hpp>
#include "Systems/GUI/ImGui/imgui.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

#include "Managers/RenderSettingsManager.h"

#include "Resources/Resources.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "RenderPasses/FramebufferRenderVolume.h"

#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/MathExtend.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RenderPasses/DeferredStatisticsObject.h"
#include "RenderPasses/ReflectiveShadowMapping/RSMStatisticsObject.h"
#include "RenderPasses/TemporalReflectiveShadowMapping/TRSMStatisticsObject.h"
#include "RenderPasses/LightPropagationVolumes/LPVStatisticsObject.h"
#include "RenderPasses/VoxelConeTracing/VCTStatisticsObject.h"
#include "RenderPasses/ScreenSpaceAmbientOcclusion/SSAOStatisticsObject.h"
#include "RenderPasses/ScreenSpaceDirectionalOcclusion/SSDOStatisticsObject.h"
#include "RenderPasses/ScreenSpaceReflections/SSRStatisticsObject.h"
#include "RenderPasses/ScreenSpaceSubsurfaceScattering/SSSubsurfaceScatteringStatisticsObject.h"
#include "RenderPasses/HybridGlobalIllumination/HGIStatisticsObject.h"
#include "RenderPasses/TemporalAntialiasing/TAAStatisticsObject.h"

namespace fs = std::experimental::filesystem;

EditorRenderingSettings::EditorRenderingSettings () :
	_lutTexture (nullptr),
	_lutTextureView (nullptr),
	_continuousVoxelizationReset (false),
	_dialog (ImGuiFs::Dialog ())
{

}

void EditorRenderingSettings::Show ()
{
	bool visible = SettingsManager::Instance ()->GetValue<bool> ("menu_show_rendering_settings", false);

	if (visible) {
		std::string path = SettingsManager::Instance ()->GetValue<std::string> ("menu_show_rendering_settings_path", "");

		if (path == std::string ()) {
			return;
		}

		_settings = RenderSettingsManager::Instance ()->GetRenderSettings (path);

		if (_settings == nullptr) {
			_settings = Resources::LoadRenderSettings (path);
			RenderSettingsManager::Instance ()->AddRenderSettings (_settings);
		}

		if (_settings == nullptr) {
			return;
		}

		ShowRenderingSettingsWindow ();
	}
}

void EditorRenderingSettings::ShowRenderingSettingsWindow ()
{
	std::size_t windowHeight = Window::GetHeight ();

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(350, windowHeight - 30), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags windowFlags = 0;

	if (!ImGui::Begin("Rendering Settings", NULL, windowFlags)) {
		ImGui::End();

		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	std::map<std::string, int> renderModes;
	renderModes ["DirectLightingRenderModule"] = 0;
	renderModes ["ReflectiveShadowMappingRenderModule"] = 1;
	renderModes ["LightPropagationVolumesRenderModule"] = 2;
	renderModes ["VoxelConeTracingRenderModule"] = 3;
	renderModes ["ScreenSpaceDirectionalOcclusionRenderModule"] = 4;
	renderModes ["TemporalReflectiveShadowMappingRenderModule"] = 5;
	renderModes ["HybridGlobalIlluminationRenderModule"] = 6;

	int lastRenderMode = renderModes [_settings->renderMode];
	int renderMode = lastRenderMode;

	const char* items[] = { "Direct Light",
		"Reflective Shadow Mapping",
		"Light Propagation Volumes",
		"Voxel Cone Tracing",
		"Screen Space Global Illumination",
		"Temporal Reflective Shadow Mapping",
		"Hybrid Global Illumiantion"
	};

	ImGui::Combo("Render Module", &renderMode, items, 7);

	const char* srenderModes[] = {
		"DirectLightingRenderModule",
		"ReflectiveShadowMappingRenderModule",
		"LightPropagationVolumesRenderModule",
		"VoxelConeTracingRenderModule",
		"ScreenSpaceDirectionalOcclusionRenderModule",
		"TemporalReflectiveShadowMappingRenderModule",
		"HybridGlobalIlluminationRenderModule"
	};

	if (lastRenderMode != renderMode) {
		_settings->renderMode = srenderModes [renderMode];
	}

	ImGui::Separator ();

	ImGui::Checkbox ("Enable Indirect Specular", &_settings->indirect_specular_enabled);
	ImGui::Checkbox ("Enable Subsurface Scattering", &_settings->subsurface_scattering_enabled);

	ImGui::PushID ("DeferredDebug");

	if (ImGui::TreeNode ("Debug")) {

		auto deferredStat = StatisticsManager::Instance ()->GetStatisticsObject <DeferredStatisticsObject> ();

		if (deferredStat->geometryBuffer != nullptr) {

			FramebufferRenderVolume* geometryBuffer = deferredStat->geometryBuffer;

			auto geometryBufferSize = geometryBuffer->GetFramebuffer ()->GetTexture (0)->GetSize ();

			int windowWidth = ImGui::GetWindowWidth() * 0.95f;

			int geometryBufferWidth = windowWidth;
			int geometryBufferHeight = ((float) geometryBufferSize.height / geometryBufferSize.width) * geometryBufferWidth;

			glm::ivec2 geometryBufferWindowSize = glm::ivec2 (geometryBufferWidth, geometryBufferHeight);

			ImGui::Text ("Depth Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetDepthTextureView ()->GetGPUIndex (), geometryBufferWindowSize);

			ImGui::Text ("Position Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), geometryBufferWindowSize);

			ImGui::Text ("Normal Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetTextureView (1)->GetGPUIndex (), geometryBufferWindowSize);

			ImGui::Text ("Diffuse Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetTextureView (2)->GetGPUIndex (), geometryBufferWindowSize);

			ImGui::Text ("Specular Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetTextureView (3)->GetGPUIndex (), geometryBufferWindowSize);

			ImGui::Text ("Emissive Map");
			ShowImage (geometryBuffer->GetFramebufferView ()->GetTextureView (4)->GetGPUIndex (), geometryBufferWindowSize);
		}

		ImGui::TreePop();
	}

	ImGui::PopID ();

	ImGui::Separator ();

    // ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Reflective Shadow Mapping")) {

		float scale = _settings->rsm_scale;
		ImGui::InputFloat ("Scale", &scale);
		if (scale > 0) {
			_settings->rsm_scale = scale;
		}

		std::size_t limit1 = 1, limit2 = 500;
		ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->rsm_samples, &limit1, &limit2);

		ImGui::SliderFloat ("Sample Radius", &_settings->rsm_radius, 0.001, 1.0);

		ImGui::Separator();

		ImGui::Checkbox ("Enable Ambient Occlusion", &_settings->rsm_ao_enabled);

		ImGui::Separator();

		ImGui::PushID ("RSMIndirect Light Intensity");
		ImGui::InputFloat ("Indirect Diffuse Light Intensity", &_settings->rsm_indirect_diffuse_intensity, 0.1);
		ImGui::InputFloat ("Indirect Specular Light Intensity", &_settings->rsm_indirect_specular_intensity, 0.1);
		ImGui::InputFloat ("Indirect Refractive Light Intensity", &_settings->rsm_indirect_refractive_intensity, 0.1);
		ImGui::PopID ();

		ImGui::Separator();

		std::size_t step = 1;
		ImGui::InputScalar ("Sample Iterations", ImGuiDataType_U32, &_settings->rsm_iterations, &step);

		ImGui::InputFloat ("Thickness", &_settings->rsm_thickness, 0.1);

		ImGui::Separator();

		ImGui::Checkbox ("Shown Non Interpolated Pixels", &_settings->rsm_debug_interpolation);

		float interpolationScale = _settings->rsm_interpolation_scale;
		ImGui::InputFloat ("Interpolation Scale", &interpolationScale);
		if (interpolationScale > 0) {
			_settings->rsm_interpolation_scale = interpolationScale;
		}

		ImGui::InputFloat ("Min Interpolation Distance", &_settings->rsm_min_interpolation_distance, 0.1);
		ImGui::InputFloat ("Min Interpolation Angle (deg)", &_settings->rsm_min_interpolation_angle, 0.1);

		ImGui::Separator();

		ImGui::PushID ("RSMDebug");

		if (ImGui::TreeNode ("Debug")) {

			auto rsmStat = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();

			if (ImGui::TreeNode ("Reflective Shadow Map")) {
				if (rsmStat->rsmVolume != nullptr) {

					FramebufferRenderVolume* rsmVolume = rsmStat->rsmVolume;

					int windowWidth = ImGui::GetWindowWidth() * 0.65f;

					ImGui::Text ("Depth Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetDepthTextureView ()->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Position Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Normal Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (1)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Flux Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (2)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode ("Indirect Light")) {
				if (rsmStat->rsmIndirectDiffuseMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* rsmIndirectDiffuseMapVolume = rsmStat->rsmIndirectDiffuseMapVolume;
					FramebufferRenderVolume* rsmIndirectSpecularMapVolume = rsmStat->rsmIndirectSpecularMapVolume;
					FramebufferRenderVolume* rsmSubsurfaceScatteringMapVolume = rsmStat->rsmSubsurfaceScatteringMapVolume;
					FramebufferRenderVolume* rsmAmbientOcclusionMapVolume = rsmStat->rsmAmbientOcclusionMapVolume;

					FramebufferRenderVolume* rsmInterpolatedIndirectDiffuseMapVolume = rsmStat->rsmInterpolatedIndirectDiffuseMapVolume;

					auto rsmMapSize = rsmIndirectDiffuseMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int rsmMapWidth = windowWidth;
					int rsmMapHeight = ((float) rsmMapSize.height / rsmMapSize.width) * rsmMapWidth;

					ImGui::Text ("Screen Space Interpolation Indirect Diffuse Light Map");
					ShowImage (rsmInterpolatedIndirectDiffuseMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (rsmMapWidth, rsmMapHeight));

					ImGui::Text ("Indirect Diffuse Light Map");
					ShowImage (rsmIndirectDiffuseMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (rsmMapWidth, rsmMapHeight));

					ImGui::Text ("Indirect Specular Light Map");
					ShowImage (rsmIndirectSpecularMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (rsmMapWidth, rsmMapHeight));

					ImGui::Text ("Subsurface Scattering Map");
					ShowImage (rsmSubsurfaceScatteringMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (rsmMapWidth, rsmMapHeight));

					ImGui::Text ("Ambient Occlusion Map");
					ShowImage (rsmAmbientOcclusionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (rsmMapWidth, rsmMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::PopID ();
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Light Propagation Volumes")) {

		ImGui::InputScalar ("Volume Size", ImGuiDataType_U32, &_settings->lpv_volume_size);
		ImGui::InputScalar ("Iterations", ImGuiDataType_U32, &_settings->lpv_iterations);

		ImGui::InputFloat ("Light Injection Bias", &_settings->lpv_injection_bias, 0.1);

		ImGui::Checkbox ("Geometry Occlusion", &_settings->lpv_geometry_occlusion);

		ImGui::Separator();

		ImGui::PushID ("LPVIndirect Light Intensity");
		ImGui::InputFloat ("Indirect Diffuse Light Intensity", &_settings->lpv_indirect_diffuse_intensity, 0.1);
		ImGui::InputFloat ("Indirect Specular Light Intensity", &_settings->lpv_indirect_specular_intensity, 0.1);
		ImGui::InputFloat ("Indirect Refractive Intensity", &_settings->lpv_indirect_refractive_intensity, 0.1);
		ImGui::PopID ();

		ImGui::InputScalar ("Specular Iterations", ImGuiDataType_U32, &_settings->lpv_specular_iterations);

		ImGui::Separator();

		ImGui::PushID ("LPVDebug");

		if (ImGui::TreeNode ("Debug")) {

			auto lpvStat = StatisticsManager::Instance ()->GetStatisticsObject <LPVStatisticsObject> ();

			if (lpvStat->lpvIndirectDiffuseMapVolume != nullptr) {

				int windowWidth = ImGui::GetWindowWidth() * 0.95f;

				FramebufferRenderVolume* lpvIndirectDiffuseMapVolume = lpvStat->lpvIndirectDiffuseMapVolume;
				FramebufferRenderVolume* lpvIndirectSpecularMapVolume = lpvStat->lpvIndirectSpecularMapVolume;
				FramebufferRenderVolume* lpvSubsurfaceScatteringMapVolume = lpvStat->lpvSubsurfaceScatteringMapVolume;
				FramebufferRenderVolume* lpvAmbientOcclusionMapVolume = lpvStat->lpvAmbientOcclusionMapVolume;

				auto lpvMapSize = lpvIndirectDiffuseMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

				int lpvMapWidth = windowWidth;
				int lpvMapHeight = ((float) lpvMapSize.height / lpvMapSize.width) * lpvMapWidth;

				ImGui::Text ("Indirect Diffuse Light Map");
				ShowImage (lpvIndirectDiffuseMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (lpvMapWidth, lpvMapHeight));

				ImGui::Text ("Indirect Specular Light Map");
				ShowImage (lpvIndirectSpecularMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (lpvMapWidth, lpvMapHeight));

				ImGui::Text ("Subsurface Scattering Map");
				ShowImage (lpvSubsurfaceScatteringMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (lpvMapWidth, lpvMapHeight));

				ImGui::Text ("Subsurface Scattering Map");
				ShowImage (lpvAmbientOcclusionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (lpvMapWidth, lpvMapHeight));
			}

			ImGui::TreePop();
		}

		ImGui::PopID ();
	}

	ImGui::Spacing ();

	if (ImGui::CollapsingHeader ("Voxel Cone Tracing")) {
		if (_continuousVoxelizationReset == true) {
			_settings->vct_continuous_voxelization = _lastContinuousVoxelization;
			_continuousVoxelizationReset = false;
		}

		std::size_t lastVoxelVolumeSize = _settings->vct_voxels_size;
		bool lastVoxelBordering = _settings->vct_bordering;
		std::size_t lastVolumeMipmapLevels = _settings->vct_mipmap_levels;

		ImGui::InputScalar ("Voxel Volume Size", ImGuiDataType_U32, &_settings->vct_voxels_size);
		ImGui::Checkbox ("Continuous Voxelization", &_settings->vct_continuous_voxelization);
		// ImGui::Checkbox ("Voxel Volume Bordering", &_settings->vct_bordering);

		std::size_t speed = 1;
		ImGui::InputScalar ("Volume Mipmap Levels", ImGuiDataType_U32, &_settings->vct_mipmap_levels, &speed);
		_settings->vct_mipmap_levels = Extensions::MathExtend::Clamp (
			_settings->vct_mipmap_levels, (std::size_t) 1,
			(std::size_t) std::log2 (_settings->vct_voxels_size));

		ImGui::Separator ();

		ImGui::PushID ("Voxel Indirect Light Intensity");
		ImGui::InputFloat ("Indirect Diffuse Light Intensity", &_settings->vct_indirect_diffuse_intensity, 0.1f);
		ImGui::InputFloat ("Indirect Specular Light Intensity", &_settings->vct_indirect_specular_intensity, 0.1f);
		ImGui::InputFloat ("Indirect Refractive Light Intensity", &_settings->vct_indirect_refractive_intensity, 0.1f);
		ImGui::PopID ();

        ImGui::Separator();

		ImGui::SliderFloat ("Diffuse Cone Distance", &_settings->vct_diffuse_cone_distance, 0.0f, 1.0f);
		ImGui::SliderFloat ("Diffuse Origin Bias", &_settings->vct_diffuse_origin_bias, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Specular Cone Ratio", &_settings->vct_specular_cone_ratio, 0.0f, 1.0f, "%3f", 10.0f);
		ImGui::SliderFloat ("Specular Cone Distance", &_settings->vct_specular_cone_distance, 0.0f, 1.0f);
		ImGui::SliderFloat ("Specular Origin Bias", &_settings->vct_specular_origin_bias, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Refractive Cone Ratio", &_settings->vct_refractive_cone_ratio, 0.0f, 1.0f, "%3f", 10.0f);
		ImGui::SliderFloat ("Refractive Cone Distance", &_settings->vct_refractive_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::Checkbox ("Enable Ambient Occlusion", &_settings->vct_ao_enabled);
		ImGui::SliderFloat ("Ambient Occlusion Cone Ratio", &_settings->vct_ao_cone_ratio, 0.0f, 1.0f);
		ImGui::SliderFloat ("Ambient Occlusion Cone Distance", &_settings->vct_ao_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::Checkbox ("Shadow Cone Cast", &_settings->vct_shadow_cone_enabled);
		ImGui::SliderFloat ("Shadow Cone Ratio", &_settings->vct_shadow_cone_ratio, 0.0f, 1.0f, "%3f", 10.0f);
		ImGui::SliderFloat ("Shadow Cone Distance", &_settings->vct_shadow_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

        ImGui::Checkbox ("Temporal Filter Enabled", &_settings->vct_temporal_filter_enabled);

		// ImGui::SliderFloat ("Origin Bias", &_settings->vct_origin_bias, 0.0f, 1.0f, "%5f", 10.0f);

		if (lastVoxelVolumeSize != _settings->vct_voxels_size ||
			lastVoxelBordering != _settings->vct_bordering ||
			lastVolumeMipmapLevels != _settings->vct_mipmap_levels) {
			_lastContinuousVoxelization = _settings->vct_continuous_voxelization;
			_continuousVoxelizationReset = true;

			_settings->vct_continuous_voxelization = true;
		}

        ImGui::Separator();

		if (ImGui::TreeNode ("Debug")) {

			ImGui::Checkbox ("Show Voxels", &_settings->vct_debug_show_voxels);

			if (_settings->vct_debug_show_voxels && _settings->renderMode == "VoxelConeTracingRenderModule") {
				_settings->renderMode = "VoxelRayTracingRenderModule";
			}
			else if (!_settings->vct_debug_show_voxels && _settings->renderMode == "VoxelRayTracingRenderModule") {
				_settings->renderMode = "VoxelConeTracingRenderModule";
			}

			std::size_t speed = 1;
			ImGui::InputScalar ("Mipmap Level", ImGuiDataType_U32, &_settings->vct_debug_volume_mipmap_level, &speed);
			_settings->vct_debug_volume_mipmap_level = Extensions::MathExtend::Clamp (
				_settings->vct_debug_volume_mipmap_level, (std::size_t) 0,
				(std::size_t) _settings->vct_mipmap_levels - 1);

			auto vctStat = StatisticsManager::Instance ()->GetStatisticsObject <VCTStatisticsObject> ();

			ImGui::Separator ();

			if (ImGui::TreeNode ("Indirect Light")) {

				int windowWidth = ImGui::GetWindowWidth() * 0.95f;

				FramebufferRenderVolume* vctIndirectDiffuseMapVolume = vctStat->vctIndirectDiffuseMapVolume;
				FramebufferRenderVolume* vctIndirectSpecularMapVolume = vctStat->vctIndirectSpecularMapVolume;
				FramebufferRenderVolume* vctAmbientOcclusionMapVolume = vctStat->vctAmbientOcclusionMapVolume;
				FramebufferRenderVolume* vctSubsurfaceScatteringMapVolume = vctStat->vctSubsurfaceScatteringMapVolume;
				FramebufferRenderVolume* vctShadowVolume = vctStat->vctShadowVolume;

				if (vctIndirectDiffuseMapVolume != nullptr) {
					auto vctMapSize = vctIndirectDiffuseMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int vctMapWidth = windowWidth;
					int vctMapHeight = ((float) vctMapSize.height / vctMapSize.width) * vctMapWidth;

					ImGui::Text ("Indirect Diffuse Light Map");
					ShowImage (vctIndirectDiffuseMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (vctMapWidth, vctMapHeight));

					ImGui::Text ("Indirect Specular Light Map");
					ShowImage (vctIndirectSpecularMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (vctMapWidth, vctMapHeight));

					ImGui::Text ("Subsurface Scattering Map");
					ShowImage (vctSubsurfaceScatteringMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (vctMapWidth, vctMapHeight));

					ImGui::Text ("Ambient Occlusion Map");
					ShowImage (vctAmbientOcclusionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (vctMapWidth, vctMapHeight));

					ImGui::Text ("Shadow");
					ShowImage (vctShadowVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (vctMapWidth, vctMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Temporal Reflective Shadow Mapping")) {

		ImGui::PushID ("Temporal Reflective Shadow Mapping");
		std::size_t limit1 = 0, limit2 = 500;
		ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->trsm_samples, &limit1, &limit2);

		ImGui::InputFloat ("Indirect Diffuse Light Intensity ", &_settings->trsm_indirect_diffuse_intensity, 0.1f);
		ImGui::PopID ();

		ImGui::Separator ();

		ImGui::Checkbox ("Temporal Filter Enabled", &_settings->trsm_temporal_filter_enabled);
		ImGui::Checkbox ("Blur Enabled", &_settings->trsm_blur_enabled);

		ImGui::Separator();

		ImGui::PushID ("TRSMDebug");
		if (ImGui::TreeNode ("Debug")) {

			auto rsmStat = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();
			auto trsmStat = StatisticsManager::Instance ()->GetStatisticsObject <TRSMStatisticsObject> ();

			if (ImGui::TreeNode ("Reflective Shadow Map")) {
				if (rsmStat->rsmVolume != nullptr) {

					FramebufferRenderVolume* rsmVolume = rsmStat->rsmVolume;

					int windowWidth = ImGui::GetWindowWidth() * 0.65f;

					ImGui::Text ("Depth Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetDepthTextureView ()->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Position Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (1)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Normal Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (2)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Flux Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (3)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode ("Indirect Light")) {
				if (trsmStat->trsmIndirectDiffuseMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* trsmIndirectDiffuseMapVolume = trsmStat->trsmIndirectDiffuseMapVolume;
					FramebufferRenderVolume* trsmTemporalFilterMapVolume = trsmStat->trsmTemporalFilterMapVolume;
					FramebufferRenderVolume* rsmIndirectSpecularMapVolume = rsmStat->rsmIndirectSpecularMapVolume;
					FramebufferRenderVolume* rsmSubsurfaceScatteringMapVolume = rsmStat->rsmSubsurfaceScatteringMapVolume;
					FramebufferRenderVolume* rsmAmbientOcclusionMapVolume = rsmStat->rsmAmbientOcclusionMapVolume;

					auto trsmMapSize = trsmIndirectDiffuseMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int trsmMapWidth = windowWidth;
					int trsmMapHeight = ((float) trsmMapSize.height / trsmMapSize.width) * trsmMapWidth;

					ImGui::Text ("Indirect Diffuse Light Map");
					ShowImage (trsmIndirectDiffuseMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

					ImGui::Text ("Temporal Filter Map");
					ShowImage (trsmTemporalFilterMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

					ImGui::Text ("Indirect Specular Light Map");
					ShowImage (rsmIndirectSpecularMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

					ImGui::Text ("Subsurface Scattering Map");
					ShowImage (rsmSubsurfaceScatteringMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

					ImGui::Text ("Ambient Occlusion Map");
					ShowImage (rsmAmbientOcclusionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::PopID ();
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Hybrid Global Illumination")) {

		ImGui::Checkbox ("Temporal Filter Enabled", &_settings->hgi_temporal_filter_enabled);

		ImGui::Separator ();

		std::size_t limit1 = 1, limit2 = 500;
		ImGui::SliderScalar ("RSM Samples Size", ImGuiDataType_U32, &_settings->hgi_rsm_samples, &limit1, &limit2);

		ImGui::InputFloat ("RSM Sample Radius", &_settings->hgi_rsm_radius, 0.1);

		ImGui::Separator();

		ImGui::SliderScalar ("SSDO Samples Size", ImGuiDataType_U32, &_settings->hgi_ssdo_samples, &limit1, &limit2);

		ImGui::InputFloat ("SSDO Sample Radius", &_settings->hgi_ssdo_radius, 0.1);

		ImGui::InputFloat ("SSDO Sampling Scale", &_settings->hgi_ssdo_sampling_scale, 0.1);

		ImGui::Separator();

		ImGui::InputFloat ("Thickness", &_settings->hgi_rsm_thickness, 0.1);

		ImGui::Separator();

		ImGui::PushID ("HGIIndirect Light Intensity");
		ImGui::InputFloat ("RSM Indirect Diffuse Light Intensity", &_settings->hgi_rsm_indirect_diffuse_intensity, 0.1);
		ImGui::InputFloat ("SSDO Indirect Diffuse Light Intensity", &_settings->hgi_ssdo_indirect_diffuse_intensity, 0.1);
		ImGui::InputFloat ("RSM Indirect Specular Light Intensity", &_settings->hgi_rsm_indirect_specular_intensity, 0.1);
		ImGui::InputFloat ("SSR Indirect Specular Light Intensity", &_settings->hgi_ssr_indirect_specular_intensity, 0.1);
		// ImGui::InputFloat ("Indirect Refractive Intensity", &_settings->lpv_indirect_refractive_intensity, 0.1);
		ImGui::PopID ();

		ImGui::Separator ();

		ImGui::Checkbox ("Enable Interpolation", &_settings->hgi_interpolation_enabled);

		ImGui::Checkbox ("Shown Non Interpolated Pixels", &_settings->hgi_debug_interpolation);

		float interpolationScale = _settings->hgi_interpolation_scale;
		ImGui::InputFloat ("Interpolation Scale", &interpolationScale);
		if (interpolationScale > 0) {
			_settings->hgi_interpolation_scale = interpolationScale;
		}

		ImGui::InputFloat ("Min Interpolation Distance", &_settings->hgi_min_interpolation_distance, 0.1);
		ImGui::InputFloat ("Min Interpolation Angle (deg)", &_settings->hgi_min_interpolation_angle, 0.1);

		ImGui::Separator();

		ImGui::Checkbox ("AO Enabled", &_settings->hgi_ao_enabled);

		limit1 = 0, limit2 = 200;
		ImGui::SliderScalar ("AO Samples Size", ImGuiDataType_U32, &_settings->hgi_ao_samples, &limit1, &limit2);

		ImGui::InputFloat ("AO Radius", &_settings->hgi_ao_radius, 0.1f);
		ImGui::InputFloat ("AO Bias", &_settings->hgi_ao_bias, 0.1f);
		ImGui::SliderFloat ("AO Blend", &_settings->hgi_ao_blend, 0.0f, 1.0f);

		ImGui::Separator();

		ImGui::PushID ("HGIDebug");
		if (ImGui::TreeNode ("Debug")) {

			auto rsmStat = StatisticsManager::Instance ()->GetStatisticsObject <RSMStatisticsObject> ();
			auto hgiStat = StatisticsManager::Instance ()->GetStatisticsObject <HGIStatisticsObject> ();

			if (ImGui::TreeNode ("Reflective Shadow Map")) {
				if (rsmStat->rsmVolume != nullptr) {

					FramebufferRenderVolume* rsmVolume = rsmStat->rsmVolume;

					int windowWidth = ImGui::GetWindowWidth() * 0.65f;

					ImGui::Text ("Depth Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetDepthTextureView ()->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Position Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (1)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Normal Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (2)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Flux Map");
					ShowImage (rsmVolume->GetFramebufferView ()->GetTextureView (3)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode ("Indirect Light")) {

				// FramebufferRenderVolume* rsmIndirectSpecularMapVolume = rsmStat->rsmIndirectSpecularMapVolume;
				// FramebufferRenderVolume* rsmSubsurfaceScatteringMapVolume = rsmStat->rsmSubsurfaceScatteringMapVolume;
				// FramebufferRenderVolume* rsmAmbientOcclusionMapVolume = rsmStat->rsmAmbientOcclusionMapVolume;

				ShowImage ("Direct Light Map", hgiStat->hgiDirectMapVolume);
				ShowImage ("RSM Interpolated Indirect Diffuse Light Map", hgiStat->hgiRSMInterpolatedIndirectDiffuseMapVolume);
				ShowImage ("RSM Indirect Diffuse Light Map", hgiStat->hgiRSMIndirectDiffuseMapVolume);
				ShowImage ("SSDO Interpolated Indirect Diffuse Light Map", hgiStat->hgiSSDOInterpolatedIndirectDiffuseMapVolume);
				ShowImage ("SSDO Indirect Diffuse Light Map", hgiStat->hgiSSDOIndirectDiffuseMapVolume);
				ShowImage ("Indirect Diffuse Map", hgiStat->hgiIndirectDiffuseMapVolume);
				ShowImage ("Indirect Diffuse Temporal Filter Map", hgiStat->hgiTemporalFilterMapVolume);
				ShowImage ("SSR Position Map", hgiStat->hgiSSRPositionMapVolume);
				ShowImage ("Indirect Specular Light Map", hgiStat->hgiIndirectSpecularMapVolume);
				ShowImage ("RSM Ambient Occlusion Map", hgiStat->hgiRSMAmbientOcclusionMapVolume);
				ShowImage ("Ambient Occlusion Map", hgiStat->hgiAmbientOcclusionMapVolume);
				ShowImage ("AO Blur Map", hgiStat->hgiAOBlurMapVolume);
				ShowImage ("Temporal Filter AO Map", hgiStat->hgiAOTemporalFilterMapVolume);

				// ImGui::Text ("Indirect Specular Light Map");
				// ShowImage (rsmIndirectSpecularMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

				// ImGui::Text ("Subsurface Scattering Map");
				// ShowImage (rsmSubsurfaceScatteringMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

				// ImGui::Text ("Ambient Occlusion Map");
				// ShowImage (rsmAmbientOcclusionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (trsmMapWidth, trsmMapHeight));

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
		ImGui::PopID ();
	}

	ImGui::Spacing ();

	if (ImGui::CollapsingHeader ("Post Processing")) {
		if (ImGui::TreeNode ("Screen Space Ambient Occlusion")) {

			ImGui::Checkbox ("Enabled", &_settings->ssao_enabled);

			float scale = _settings->ssao_scale;
			ImGui::InputFloat ("Scale", &scale);
			if (scale > 0) {
				_settings->ssao_scale = scale;
			}

			std::size_t limit1 = 0, limit2 = 200;
			ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->ssao_samples, &limit1, &limit2);
			
			ImGui::InputScalar ("Noise Size", ImGuiDataType_U32, &_settings->ssao_noise_size);
			ImGui::InputFloat ("Radius", &_settings->ssao_radius, 0.1f);
			ImGui::InputFloat ("Bias", &_settings->ssao_bias, 0.1f);

			ImGui::Separator ();

			ImGui::Checkbox ("Blur Enabled", &_settings->ssao_blur_enabled);
			ImGui::Checkbox ("Temporal Filter Enabled", &_settings->ssao_temporal_filter_enabled);

			ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {

				auto ssaoStat = StatisticsManager::Instance ()->GetStatisticsObject <SSAOStatisticsObject> ();

				if (ssaoStat->ssaoMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* ssaoMapVolume = ssaoStat->ssaoMapVolume;
					FramebufferRenderVolume* ssaoBlurMapVolume = ssaoStat->ssaoBlurMapVolume;
					FramebufferRenderVolume* ssaoTemporalFilterMapVolume = ssaoStat->ssaoTemporalFilterMapVolume;
					TextureRenderVolume* ssaoNoiseMapVolume = ssaoStat->ssaoNoiseMapVolume;

					auto ssaoMapSize = ssaoMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int ssaoMapWidth = windowWidth;
					int ssaoMapHeight = ((float) ssaoMapSize.height / ssaoMapSize.width) * ssaoMapWidth;

					ImGui::Text ("SSAO Map");
					ShowImage (ssaoMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssaoMapWidth, ssaoMapHeight));

					ImGui::Text ("SSAO Blur Map");
					ShowImage (ssaoBlurMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssaoMapWidth, ssaoMapHeight));

					ImGui::Text ("SSAO Temporal Filter Map");
					ShowImage (ssaoTemporalFilterMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssaoMapWidth, ssaoMapHeight));

					ImGui::Text ("SSAO Noise Map");
					ShowImage (ssaoNoiseMapVolume->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (windowWidth, windowWidth));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Screen Space Directional Occlusion")) {

			ImGui::Checkbox ("Enabled", &_settings->ssdo_enabled);
			ImGui::Checkbox ("Temporal Filter Enabled", &_settings->ssdo_temporal_filter_enabled);

			float scale = _settings->ssdo_scale;
			ImGui::InputFloat ("Scale", &scale);
			if (scale > 0) {
				_settings->ssdo_scale = scale;
			}

			std::size_t limit1 = 1, limit2 = 500;
			ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->ssdo_samples, &limit1, &limit2);

			ImGui::InputFloat ("Radius", &_settings->ssdo_radius, 0.1f);
			ImGui::InputFloat ("Bias", &_settings->ssdo_bias, 0.1f);

			float samplingScale = _settings->ssdo_sampling_scale;
			ImGui::InputFloat ("Sampling Scale", &samplingScale, 0.1);
			if (samplingScale > 0) {
				_settings->ssdo_sampling_scale = samplingScale;
			}

			ImGui::InputFloat ("Indirect Light Intensity", &_settings->ssdo_indirect_intensity, 0.1f);

			ImGui::Separator ();

			ImGui::Checkbox ("Enable Interpolation", &_settings->ssdo_interpolation_enabled);
			ImGui::Checkbox ("Shown Non Interpolated Pixels", &_settings->ssdo_debug_interpolation);

			float interpolationScale = _settings->ssdo_interpolation_scale;
			ImGui::InputFloat ("Interpolation Scale", &interpolationScale);
			if (interpolationScale > 0) {
				_settings->ssdo_interpolation_scale = interpolationScale;
			}

			ImGui::InputFloat ("Min Interpolation Distance", &_settings->ssdo_min_interpolation_distance, 0.1);
			ImGui::InputFloat ("Min Interpolation Angle (deg)", &_settings->ssdo_min_interpolation_angle, 0.1);

			ImGui::Separator ();

			ImGui::Checkbox ("Shadow 2D Ray Cast", &_settings->ssdo_ray_shadow);

			float shadowScale = _settings->ssdo_shadow_scale;
			ImGui::InputFloat ("Shadow Scale", &shadowScale);
			if (shadowScale > 0) {
				_settings->ssdo_shadow_scale = shadowScale;
			}

			std::size_t strideStep = 1;
			ImGui::InputScalar ("Shadow Stride", ImGuiDataType_U32, &_settings->ssdo_shadow_stride, &strideStep);

			ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {
				auto ssdoStat = StatisticsManager::Instance ()->GetStatisticsObject <SSDOStatisticsObject> ();

				if (ssdoStat->ssdoMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.9f;

					FramebufferRenderVolume* ssdoMapVolume = ssdoStat->ssdoMapVolume;
					FramebufferRenderVolume* ssdoTemporalFilterMapVolume = ssdoStat->ssdoTemporalFilterMapVolume;

					FramebufferRenderVolume* ssdoInterpolatedMapVolume = ssdoStat->ssdoInterpolatedMapVolume;

					auto size = ssdoMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int width = windowWidth;
					int height = ((float) size.height / size.width) * width;

					ShowImage ("SSDO Interpolated Map", ssdoInterpolatedMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (width, height));

					ShowImage ("SSDO Map", ssdoMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (width, height));

					ShowImage ("SSDO Temporal Filter Map", ssdoTemporalFilterMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (width, height));

					// FramebufferRenderVolume* ssdoShadowVolume = ssdoStat->ssdoShadowVolume;

					// ImGui::Text ("SSDO Shadow Map");
					// ShowImage (ssdoShadowVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (width, height));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Screen Space Reflection")) {

			ImGui::Checkbox ("Enabled", &_settings->ssr_enabled);

			float scale = _settings->ssr_scale;
			ImGui::InputFloat ("Scale", &scale);
			if (scale > 0) {
				_settings->ssr_scale = scale;
			}

			ImGui::SliderFloat ("Roughness", &_settings->ssr_roughness, 0.0f, 1.0f);

			std::size_t step = 1;
			ImGui::InputScalar ("Sample Iterations", ImGuiDataType_U32, &_settings->ssr_iterations, &step);

			ImGui::SliderFloat ("Sample Thickness", &_settings->ssr_thickness, 0.0f, 10.0f);

			std::size_t strideStep = 1;
			ImGui::InputScalar ("Stride", ImGuiDataType_U32, &_settings->ssr_stride, &strideStep);

			ImGui::InputFloat ("Intensity", &_settings->ssr_intensity, 0.1f);

			ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {
				auto ssrStat = StatisticsManager::Instance ()->GetStatisticsObject <SSRStatisticsObject> ();

				if (ssrStat->ssrPositionMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* ssrPositionMapVolume = ssrStat->ssrPositionMapVolume;
					FramebufferRenderVolume* ssrMapVolume = ssrStat->ssrMapVolume;

					auto ssrMapSize = ssrPositionMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int ssrMapWidth = windowWidth;
					int ssrMapHeight = ((float) ssrMapSize.height / ssrMapSize.width) * ssrMapWidth;

					ImGui::Text ("SSR Position Map");
					ShowImage (ssrPositionMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssrMapWidth, ssrMapHeight));

					ImGui::Text ("SSR Map");
					ShowImage (ssrMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssrMapWidth, ssrMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Screen Space Subsurface Scattering")) {

			// ImGui::Checkbox ("Enabled", &_settings->ssr_enabled);

			// float scale = _settings->ssr_scale;
			// ImGui::InputFloat ("Scale", &scale);
			// if (scale > 0) {
			// 	_settings->ssr_scale = scale;
			// }

			// ImGui::SliderFloat ("Roughness", &_settings->ssr_roughness, 0.0f, 1.0f);

			// std::size_t step = 1;
			// ImGui::InputScalar ("Sample Iterations", ImGuiDataType_U32, &_settings->ssr_iterations, &step);

			// ImGui::SliderFloat ("Sample Thickness", &_settings->ssr_thickness, 0.0f, 10.0f);

			// std::size_t strideStep = 1;
			// ImGui::InputScalar ("Stride", ImGuiDataType_U32, &_settings->ssr_stride, &strideStep);

			// ImGui::InputFloat ("Intensity", &_settings->ssr_intensity, 0.1f);

			// ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {
				auto ssrStat = StatisticsManager::Instance ()->GetStatisticsObject <SSSubsurfaceScatteringStatisticsObject> ();

				if (ssrStat->ssrMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* ssrMapVolume = ssrStat->ssrMapVolume;

					auto ssrMapSize = ssrMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int ssrMapWidth = windowWidth;
					int ssrMapHeight = ((float) ssrMapSize.height / ssrMapSize.width) * ssrMapWidth;

					ImGui::Text ("SSR Map");
					ShowImage (ssrMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (ssrMapWidth, ssrMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Temporal Anti-aliasing")) {

			ImGui::Checkbox ("Enabled", &_settings->taa_enabled);

			ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {
				auto taaStat = StatisticsManager::Instance ()->GetStatisticsObject <TAAStatisticsObject> ();

				if (taaStat->taaMapVolume != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FramebufferRenderVolume* taaMapVolume = taaStat->taaMapVolume;

					auto taaMapSize = taaMapVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

					int taaMapWidth = windowWidth;
					int taaMapHeight = ((float) taaMapSize.height / taaMapSize.width) * taaMapWidth;

					ImGui::Text ("TAA Map");
					ShowImage (taaMapVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), glm::ivec2 (taaMapWidth, taaMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Bloom")) {

			ImGui::Checkbox ("Enabled", &_settings->bloom_enabled);
			ImGui::SliderFloat ("Threshold", &_settings->bloom_threshold, 0.0f, 10.0f);
			ImGui::InputFloat ("Intensity", &_settings->bloom_intensity, 0.1f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("High Dynamic Range")) {

			ImGui::Checkbox ("Enabled", &_settings->hdr_enabled);
			ImGui::InputFloat ("Exposure", &_settings->hdr_exposure, 0.1f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("LUT Texture")) {

			ImGui::Checkbox ("Enabled", &_settings->lut_enabled);

			ImGui::Spacing ();
			ImGui::Spacing ();

			std::string lutTexturePath = _settings->lut_texture_path;

			ImGui::Text ("Path: %s", lutTexturePath.c_str ());

			if (_lutTexture == nullptr || _lastLUTTexturePath != lutTexturePath) {
				_lutTexture = Resources::LoadTexture (lutTexturePath);
				_lutTextureView = RenderSystem::LoadTexture (_lutTexture);
				_lastLUTTexturePath = lutTexturePath;
			}

			int windowWidth = ImGui::GetWindowWidth() * 0.6f;

			auto imageSize = _lutTexture->GetSize ();
			imageSize.height = windowWidth * ((float) imageSize.height / imageSize.width);
			imageSize.width = windowWidth;
			ImGui::Image((void*)(intptr_t) _lutTextureView->GetGPUIndex (), ImVec2(imageSize.width, imageSize.height));

			ImGui::SameLine ();

			bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

			const char* path = _dialog.chooseFileDialog(lastLoadTexture);

			if (strlen (path) > 0) {
				_settings->lut_texture_path = path;				
			}

			ImGui::SliderFloat ("Intensity", &_settings->lut_intensity, 0.0f, 1.0f);

			ImGui::TreePop ();
		}

		if (ImGui::TreeNode ("Gamma Correction")) {

			ImGui::Checkbox ("Enabled", &_settings->gamma_enabled);

			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void EditorRenderingSettings::ShowImage (const std::string& name,
	unsigned int textureID, const glm::ivec2& size)
{
	if (ImGui::TreeNodeEx (name.c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
		ShowImage (textureID, size);

		ImGui::TreePop();
	}
}

void EditorRenderingSettings::ShowImage (unsigned int textureID, const glm::ivec2& size)
{
	ImGui::Image((void*)(intptr_t) textureID, ImVec2(size.x, size.y), ImVec2 (0, 1), ImVec2 (1, 0));

    bool saveVolume = false;

    ImGui::PushID (textureID);

    if (ImGui::BeginPopupContextItem(std::to_string (textureID).c_str ())) {
		saveVolume = ImGui::MenuItem ("Save");

	    ImGui::EndPopup();
    }

	std::string volumePath = _dialog.saveFileDialog(saveVolume, "", "image.png", ".png");

	if (volumePath != std::string ()) {

		volumePath = FileSystem::Relative (volumePath, fs::current_path ().string ());

		Resource<TextureView> textureView (new TextureView (), "temp");
		textureView->SetGPUIndex (textureID);

		Resource<Texture> texture = RenderSystem::SaveTexture (textureView);

		textureView->SetGPUIndex (0);

		Resources::SaveTexture (texture, volumePath);
	}

	ImGui::PopID ();
}


void EditorRenderingSettings::ShowImage (const std::string& name, FramebufferRenderVolume* renderVolume)
{
	if (renderVolume == nullptr
		|| renderVolume->GetFramebuffer () == nullptr
		|| renderVolume->GetFramebuffer ()->GetTexture (0) == nullptr) {

		/*
		 *
		*/

		if (ImGui::TreeNodeEx (name.c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Text ("Unavailable");

			ImGui::TreePop();
		}

		return;
	}

	auto mapSize = renderVolume->GetFramebuffer ()->GetTexture (0)->GetSize ();

	int mapWidth = ImGui::GetWindowWidth() * 0.95f;
	int mapHeight = ((float) mapSize.height / mapSize.width) * mapWidth;

	glm::ivec2 imageSize = glm::ivec2 (mapWidth, mapHeight);

	ShowImage (name, renderVolume->GetFramebufferView ()->GetTextureView (0)->GetGPUIndex (), imageSize);
}

