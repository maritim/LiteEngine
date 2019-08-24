#include "EditorRenderingSettings.h"

#include <glm/vec2.hpp>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

#include "Managers/RenderSettingsManager.h"

#include "Resources/Resources.h"

#include "Renderer/RenderSystem.h"
#include "Renderer/RenderManager.h"

#include "Utils/Extensions/MathExtend.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/RSMStatisticsObject.h"

EditorRenderingSettings::EditorRenderingSettings () :
	_lutTexture (nullptr),
	_lutTextureView (nullptr),
	_continuousVoxelizationReset (false)
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

	int lastRenderMode = renderModes [_settings->renderMode];
	int renderMode = lastRenderMode;

	const char* items[] = { "Direct Light", "Voxel Cone Tracing", "Reflective Shadow Mapping"};
	ImGui::Combo("Render Module", &renderMode, items, 3);

	const char* srenderModes[] = { "SceneRenderModule", "VoxelConeTracingRenderModule", "ReflectiveShadowMappingRenderModule"};

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

        ImGui::Separator();

		ImGui::SliderFloat ("Diffuse Cone Distance", &_settings->vct_diffuse_cone_distance, 0.0f, 1.0f);

        ImGui::Separator();

		ImGui::SliderFloat ("Specular Cone Distance", &_settings->vct_specular_cone_distance, 0.0f, 1.0f);

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

		glm::ivec2 lastRSMResolution = _settings->rsm_resolution;
		int rsmResolution [2] = { lastRSMResolution.x, lastRSMResolution.y };
		ImGui::InputInt2 ("Resolution", rsmResolution);
		_settings->rsm_resolution = glm::ivec2 (rsmResolution [0], rsmResolution [1]);

		std::size_t limit1 = 1, limit2 = 200;
		ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->rsm_samples, &limit1, &limit2);

		ImGui::SliderFloat ("Shadow Bias", &_settings->rsm_bias, 0.0001, 0.2, "%5f");
		ImGui::SliderFloat ("Sample Radius", &_settings->rsm_radius, 0.001, 0.2);
		ImGui::SliderFloat ("Indirect Light Intensity", &_settings->rsm_intensity, 0, 5);
		ImGui::Checkbox ("Caching", &_settings->rsm_caching);

		if (ImGui::TreeNode ("Debug")) {
			if (ImGui::TreeNode ("Reflective Shadow Map")) {

				StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("RSMStatisticsObject");
				RSMStatisticsObject* rsmStat = nullptr;

				if (stat != nullptr) {
					rsmStat = dynamic_cast<RSMStatisticsObject*> (stat);
				}

				if (rsmStat != nullptr) {

					int windowWidth = ImGui::GetWindowWidth() * 0.65f;

					ImGui::Text ("Position Map");
					ImGui::Image((void*)(intptr_t) rsmStat->rsmPosMapID, ImVec2(windowWidth, windowWidth), ImVec2 (0, 1), ImVec2 (1, 0));

					ImGui::Text ("Normal Map");
					ImGui::Image((void*)(intptr_t) rsmStat->rsmNormalMapID, ImVec2(windowWidth, windowWidth), ImVec2 (0, 1), ImVec2 (1, 0));

					ImGui::Text ("Flux Map");
					ImGui::Image((void*)(intptr_t) rsmStat->rsmFluxMapID, ImVec2(windowWidth, windowWidth), ImVec2 (0, 1), ImVec2 (1, 0));

					ImGui::Text ("Cache Map");
					ImGui::Image((void*)(intptr_t) rsmStat->rsmCacheMapID, ImVec2(windowWidth, windowWidth), ImVec2 (0, 1), ImVec2 (1, 0));
				}

				ImGui::TreePop();
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

			std::size_t limit1 = 0, limit2 = 64;
			ImGui::SliderScalar ("Samples Size", ImGuiDataType_U32, &_settings->ssao_samples, &limit1, &limit2);
			
			ImGui::InputScalar ("Noise Size", ImGuiDataType_U32, &_settings->ssao_noise_size);
			ImGui::InputFloat ("Radius", &_settings->ssao_radius, 0.1f);
			ImGui::InputFloat ("Bias", &_settings->ssao_bias, 0.1f);
			ImGui::Checkbox ("Blur Enabled", &_settings->ssao_blur_enabled);

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

			ImGui::InputFloat ("Spatial Sample Skip", &_settings->ssr_sample_skip, 0.1f);
			ImGui::InputFloat ("Spatial Bias", &_settings->ssr_spatial_bias, 0.1f);
			ImGui::InputFloat ("Intensity", &_settings->ssr_intensity, 0.1f);

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

			static ImGuiFs::Dialog dialog = ImGuiFs::Dialog ();
			const char* path = dialog.chooseFileDialog(lastLoadTexture);

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

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Shadow Mapping")) {

		int lastCascadesCount = SettingsManager::Instance ()->GetValue<int> ("sm_cascades", 0);
		int cascadesCount = lastCascadesCount;
		ImGui::SliderInt ("Cascades Count", &cascadesCount, 1, 4);

		glm::vec2 lastResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("sm_resolution", glm::vec2 (0, 0));
		int resolution [2] { (int) lastResolution.x, (int) lastResolution.y };
		ImGui::InputInt2 ("Resolution", resolution);

		if (lastCascadesCount != cascadesCount) {
			SettingsManager::Instance ()->SetValue ("sm_cascades", std::to_string (cascadesCount));
		}

		if (lastResolution.x != resolution [0] || lastResolution.y != resolution [1]) {
			SettingsManager::Instance ()->SetValue ("sm_resolution",
				std::to_string (resolution [0]) + "," + std::to_string (resolution [1]));
		}

		if (ImGui::TreeNode ("Exponential Shadow Mapping")) {
			int lastExponential = SettingsManager::Instance ()->GetValue<int> ("esm_exponential", 0);
			int exponential = lastExponential;
			ImGui::SliderInt ("Exponential", &exponential, 0, 200);

			if (lastExponential != exponential) {
				SettingsManager::Instance ()->SetValue ("esm_exponential", std::to_string (exponential));
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}
