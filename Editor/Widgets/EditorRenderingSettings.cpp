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

#include "Utils/Files/FileSystem.h"

#include "Utils/Extensions/MathExtend.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"
#include "Debug/Statistics/SSDOStatisticsObject.h"
#include "Debug/Statistics/SSAOStatisticsObject.h"
#include "Debug/Statistics/SSRStatisticsObject.h"

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
	renderModes ["SceneRenderModule"] = 0;
	renderModes ["VoxelConeTracingRenderModule"] = 1;
	renderModes ["ReflectiveShadowMappingRenderModule"] = 2;
	renderModes ["LightPropagationVolumesModule"] = 3;
	renderModes ["ScreenSpaceDirectionalOcclusionRenderModule"] = 4;

	int lastRenderMode = renderModes [_settings->renderMode];
	int renderMode = lastRenderMode;

	const char* items[] = { "Direct Light", "Voxel Cone Tracing", "Reflective Shadow Mapping", "Light Propagation Volumes", "Screen Space Global Illumination"};
	ImGui::Combo("Render Module", &renderMode, items, 5);

	const char* srenderModes[] = {
		"SceneRenderModule",
		"VoxelConeTracingRenderModule",
		"ReflectiveShadowMappingRenderModule",
		"LightPropagationVolumesModule",
		"ScreenSpaceDirectionalOcclusionRenderModule"
	};

	if (lastRenderMode != renderMode) {
		_settings->renderMode = srenderModes [renderMode];
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
		ImGui::Checkbox ("Voxel Volume Bordering", &_settings->vct_bordering);

		std::size_t speed = 1;
		ImGui::InputScalar ("Volume Mipmap Levels", ImGuiDataType_U32, &_settings->vct_mipmap_levels, &speed);
		_settings->vct_mipmap_levels = Extensions::MathExtend::Clamp (
			_settings->vct_mipmap_levels, (std::size_t) 1,
			(std::size_t) std::log2 (_settings->vct_voxels_size));

		ImGui::InputFloat ("Indirect Light Intensity", &_settings->vct_indirect_intensity, 0.1f);
		ImGui::InputFloat ("Refractive Indirect Light Intensity", &_settings->vct_indirect_refractive_intensity, 0.1f);

        ImGui::Separator();

		ImGui::SliderFloat ("Diffuse Cone Distance", &_settings->vct_diffuse_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Specular Cone Distance", &_settings->vct_specular_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Refractive Cone Ratio", &_settings->vct_refractive_cone_ratio, 0.0f, 1.0f, "%3f", 10.0f);
		ImGui::SliderFloat ("Refractive Cone Distance", &_settings->vct_refractive_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Shadow Cone Ratio", &_settings->vct_shadow_cone_ratio, 0.0f, 1.0f, "%3f", 10.0f);
		ImGui::SliderFloat ("Shadow Cone Distance", &_settings->vct_shadow_cone_distance, 0.0f, 1.0f);

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

			std::size_t speed = 1;
			ImGui::InputScalar ("Mipmap Level", ImGuiDataType_U32, &_settings->vct_debug_volume_mipmap_level, &speed);
			_settings->vct_debug_volume_mipmap_level = Extensions::MathExtend::Clamp (
				_settings->vct_debug_volume_mipmap_level, (std::size_t) 0,
				(std::size_t) _settings->vct_mipmap_levels - 1);

			ImGui::TreePop();
		}
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Reflective Shadow Mapping")) {

		float scale = _settings->rsm_scale;
		ImGui::InputFloat ("Scale", &scale);
		if (scale > 0) {
			_settings->rsm_scale = scale;
		}

		glm::ivec2 lastRSMResolution = _settings->rsm_resolution;
		int rsmResolution [2] = { lastRSMResolution.x, lastRSMResolution.y };
		ImGui::InputInt2 ("Resolution", rsmResolution);
		_settings->rsm_resolution = glm::ivec2 (rsmResolution [0], rsmResolution [1]);

		std::size_t limit1 = 1, limit2 = 200;
		ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->rsm_samples, &limit1, &limit2);

		ImGui::SliderFloat ("Shadow Bias", &_settings->rsm_bias, 0.0001, 0.2, "%5f");
		ImGui::SliderFloat ("Sample Radius", &_settings->rsm_radius, 0.001, 0.2);
		ImGui::SliderFloat ("Indirect Light Intensity", &_settings->rsm_intensity, 0, 5);

		ImGui::Separator();

		ImGui::Checkbox ("Noise Enabled", &_settings->rsm_noise_enabled);
		ImGui::InputScalar ("Noise Size", ImGuiDataType_U32, &_settings->rsm_noise_size);

		ImGui::Separator();

		ImGui::Checkbox ("Blur Enabled", &_settings->rsm_blur_enabled);

		ImGui::Separator();

		if (ImGui::TreeNode ("Debug")) {

			StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
			RSMStatisticsObject* rsmStat = nullptr;

			if (stat != nullptr) {
				rsmStat = dynamic_cast<RSMStatisticsObject*> (stat);
			}

			if (ImGui::TreeNode ("Reflective Shadow Map")) {

				if (rsmStat != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.65f;

					ImGui::Text ("Depth Map");
					ShowImage (rsmStat->rsmDepthMapID, glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Position Map");
					ShowImage (rsmStat->rsmPosMapID, glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Normal Map");
					ShowImage (rsmStat->rsmNormalMapID, glm::ivec2 (windowWidth, windowWidth));

					ImGui::Text ("Flux Map");
					ShowImage (rsmStat->rsmFluxMapID, glm::ivec2 (windowWidth, windowWidth));
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode ("Indirect Light")) {

				if (rsmStat != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FrameBuffer2DVolume* rsmIndirectMapVolume = rsmStat->rsmIndirectMapVolume;

					glm::ivec2 rsmMapSize = rsmIndirectMapVolume->GetSize ();

					int rsmMapWidth = windowWidth;
					int rsmMapHeight = ((float) rsmMapSize.y / rsmMapSize.x) * rsmMapWidth;

					ImGui::Text ("Indirect Light Map");
					ShowImage (rsmIndirectMapVolume->GetColorTextureID (), glm::ivec2 (rsmMapWidth, rsmMapHeight));
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}

	ImGui::Spacing ();

	if (ImGui::CollapsingHeader ("Light Propagation Volumes")) {

	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Screen Space Global Illumination")) {

		float scale = _settings->ssdo_scale;
		ImGui::InputFloat ("Scale", &scale);
		if (scale > 0) {
			_settings->ssdo_scale = scale;
		}

		float shadowScale = _settings->ssdo_shadow_scale;
		ImGui::InputFloat ("Shadow Scale", &shadowScale);
		if (shadowScale > 0) {
			_settings->ssdo_shadow_scale = shadowScale;
		}

		std::size_t limit1 = 1, limit2 = 200;
		ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->ssdo_samples, &limit1, &limit2);

		ImGui::InputFloat ("Radius", &_settings->ssdo_radius, 0.1f);
		ImGui::InputFloat ("Bias", &_settings->ssdo_bias, 0.1f);

		std::size_t strideStep = 1;
		ImGui::InputScalar ("Shadow Stride", ImGuiDataType_U32, &_settings->ssdo_shadow_stride, &strideStep);

		ImGui::InputFloat ("Indirect Light Intensity", &_settings->ssdo_indirect_intensity, 0.1f);

		ImGui::Separator ();

		if (ImGui::TreeNode ("Debug")) {
			StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSDOStatisticsObject");
			SSDOStatisticsObject* ssdoStat = nullptr;

			if (stat != nullptr) {
				ssdoStat = dynamic_cast<SSDOStatisticsObject*> (stat);
			}

			if (ssdoStat != nullptr) {

				int windowWidth = ImGui::GetWindowWidth() * 0.9f;

				FrameBuffer2DVolume* ssdoMapVolume = ssdoStat->ssdoMapVolume;

				glm::ivec2 size = ssdoMapVolume->GetSize ();

				int width = windowWidth;
				int height = ((float) size.y / size.x) * width;

				ImGui::Text ("SSDO Map");
				ShowImage (ssdoMapVolume->GetColorTextureID (), glm::ivec2 (width, height));

				FrameBuffer2DVolume* ssdoShadowVolume = ssdoStat->ssdoShadowVolume;

				ImGui::Text ("SSDO Shadow Map");
				ShowImage (ssdoShadowVolume->GetColorTextureID (), glm::ivec2 (width, height));
			}

			ImGui::TreePop();
		}
	}

    ImGui::Spacing();

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
			ImGui::Checkbox ("Blur Enabled", &_settings->ssao_blur_enabled);

			ImGui::Separator ();

			if (ImGui::TreeNode ("Debug")) {
				StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSAOStatisticsObject");
				SSAOStatisticsObject* ssaoStat = nullptr;

				if (stat != nullptr) {
					ssaoStat = dynamic_cast<SSAOStatisticsObject*> (stat);
				}

				if (ssaoStat != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FrameBuffer2DVolume* ssaoMapVolume = ssaoStat->ssaoMapVolume;

					glm::ivec2 ssaoMapSize = ssaoMapVolume->GetSize ();

					int ssaoMapWidth = windowWidth;
					int ssaoMapHeight = ((float) ssaoMapSize.y / ssaoMapSize.x) * ssaoMapWidth;

					ImGui::Text ("SSAO Map");
					ShowImage (ssaoMapVolume->GetColorTextureID (), glm::ivec2 (ssaoMapWidth, ssaoMapHeight));

					ImGui::Text ("SSAO Noise Map");
					ShowImage (ssaoStat->noiseMapID, glm::ivec2 (windowWidth, windowWidth));
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
				StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("SSRStatisticsObject");
				SSRStatisticsObject* ssrStat = nullptr;

				if (stat != nullptr) {
					ssrStat = dynamic_cast<SSRStatisticsObject*> (stat);
				}

				if (ssrStat != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.95f;

					FrameBuffer2DVolume* ssrPositionMapVolume = ssrStat->ssrPositionMapVolume;

					glm::ivec2 ssrMapSize = ssrPositionMapVolume->GetSize ();

					int ssrMapWidth = windowWidth;
					int ssrMapHeight = ((float) ssrMapSize.y / ssrMapSize.x) * ssrMapWidth;

					ImGui::Text ("SSR Position Map");
					ShowImage (ssrPositionMapVolume->GetColorTextureID (), glm::ivec2 (ssrMapWidth, ssrMapHeight));
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

			auto texSize = _lutTexture->GetSize ();
			ImGui::Image((void*)(intptr_t) _lutTextureView->GetGPUIndex (), ImVec2(texSize.width, texSize.height));

			ImGui::SameLine ();

			bool lastLoadTexture = ImGui::Button ("Load", ImVec2 (48, 18));

			const char* path = _dialog.chooseFileDialog(lastLoadTexture);

			if (strlen (path) > 0) {
				_settings->lut_texture_path = path;				
			}

			ImGui::TreePop ();
		}

		if (ImGui::TreeNode ("Gamma Correction")) {

			ImGui::Checkbox ("Enabled", &_settings->gamma_enabled);

			ImGui::TreePop();
		}
	}

	ImGui::End();
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
