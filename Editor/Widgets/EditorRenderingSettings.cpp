#include "EditorRenderingSettings.h"

#include <sstream>
#include <iomanip>
#include <glm/vec2.hpp>
#include "Systems/GUI/ImGui/imgui.h"
#include "Systems/GUI/imguifilesystem/imguifilesystem.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"

#include "Managers/TextureManager.h"
#include "Managers/RenderSettingsManager.h"

#include "Resources/Resources.h"

#include "Renderer/RenderManager.h"

#include "Debug/Logger/Logger.h"

EditorRenderingSettings::EditorRenderingSettings () :
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

	int lastRenderModule = SettingsManager::Instance ()->GetValue<int> ("render_module", 0);
	int renderModule = lastRenderModule;

	const char* items[] = { "Direct Light", "Voxel Cone Trace", "Reflective Shadow Map", "Voxel Ray Trace" };
	ImGui::Combo("Render Module", &renderModule, items, 4);

	// if (lastRenderModule != renderModule) {
	// 	SettingsManager::Instance ()->SetValue ("render_module", std::to_string (renderModule));

	// 	if (renderModule == 0) {
	// 		RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_DIRECT_LIGHTING);
	// 		SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (_lastAmbientOcclusionEnabled));
	// 	}

	// 	if (renderModule == 1) {
	// 		RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_VOXEL_CONE_TRACE);

	// 		_lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
	// 		SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (false));
	// 	}

	// 	if (renderModule == 2) {
	// 		RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_REFLECTIVE_SHADOW_MAP);

	// 		_lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
	// 		SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (false));
	// 	}

	// 	if (renderModule == 3) {
	// 		RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_VOXELIZATION);
	// 	}
	// }

	ImGui::Spacing ();

	if (ImGui::CollapsingHeader ("Voxel Cone Trace")) {
		if (_continuousVoxelizationReset == true) {
			SettingsManager::Instance ()->SetValue ("vct_continuous_voxelization", std::to_string (_lastContinuousVoxelization));
			_continuousVoxelizationReset = false;
		}

		int lastVoxelVolumeSize = SettingsManager::Instance ()->GetValue<int> ("vct_voxels_size", 0);
		int voxelVolumeSize = lastVoxelVolumeSize;
		ImGui::InputInt ("Voxel Volume Size", &voxelVolumeSize);

		bool lastRevoxelization = SettingsManager::Instance ()->GetValue<bool> ("vct_continuous_voxelization", false);
		bool revoxelization = lastRevoxelization;
		ImGui::Checkbox ("Continuous Voxelization", &revoxelization);

		bool lastVoxelBordering = SettingsManager::Instance ()->GetValue<bool> ("vct_bordering", false);
		bool voxelBordering = lastVoxelBordering;
		ImGui::Checkbox ("Voxel Volume Bordering", &voxelBordering);

		bool lastVoxelAmbient = SettingsManager::Instance ()->GetValue<bool> ("vct_ambient", false);
		bool voxelAmbient = lastVoxelAmbient;
		ImGui::Checkbox ("Voxel Volume Air Ambient", &voxelAmbient);

		float lastIndirectLightIntensity = SettingsManager::Instance ()->GetValue<float> ("vct_indirect_intensity", 0.0f);
		float indirectLightIntensity = lastIndirectLightIntensity;
		ImGui::InputFloat ("Indirect Light Intensity", &indirectLightIntensity, 0.1f);

		if (lastVoxelVolumeSize != voxelVolumeSize) {
			SettingsManager::Instance ()->SetValue ("vct_voxels_size", std::to_string (voxelVolumeSize));

			_lastContinuousVoxelization = revoxelization;
			_continuousVoxelizationReset = true;

			SettingsManager::Instance ()->SetValue ("vct_continuous_voxelization", std::to_string (true));
		}

		if (lastRevoxelization != revoxelization) {
			SettingsManager::Instance ()->SetValue ("vct_continuous_voxelization", std::to_string (revoxelization));
		}

		if (lastVoxelBordering != voxelBordering) {
			SettingsManager::Instance ()->SetValue ("vct_bordering", std::to_string (voxelBordering));

			_lastContinuousVoxelization = revoxelization;
			_continuousVoxelizationReset = true;

			SettingsManager::Instance ()->SetValue ("vct_continuous_voxelization", std::to_string (true));
		}

		if (lastVoxelAmbient != voxelAmbient) {
			SettingsManager::Instance ()->SetValue ("vct_ambient", std::to_string (voxelAmbient));

			_lastContinuousVoxelization = revoxelization;
			_continuousVoxelizationReset = true;

			SettingsManager::Instance ()->SetValue ("vct_continuous_voxelization", std::to_string (true));
		}

		if (lastIndirectLightIntensity != indirectLightIntensity) {
			SettingsManager::Instance ()->SetValue ("vct_indirect_intensity", std::to_string (indirectLightIntensity));
		}
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Reflective Shadow Map")) {
		glm::vec2 lastRSMResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("rsm_resolution", glm::vec2 (0));
		int rsmResolution [2] = { (int) lastRSMResolution.x, (int) lastRSMResolution.y };
		ImGui::InputInt2 ("Resolution", rsmResolution);

		int lastRSMSamplesSize = SettingsManager::Instance ()->GetValue<int> ("rsm_samples", 0);
		int rsmSamplesSize = lastRSMSamplesSize;
		ImGui::SliderInt ("Samples Size", &rsmSamplesSize, 1, 200);

		float lastRSMRadius = SettingsManager::Instance ()->GetValue<float> ("rsm_radius", 0.0f);
		float rsmRadius = lastRSMRadius;
		ImGui::InputFloat ("Sample Radius", &rsmRadius);

		float lastRSMIntensity = SettingsManager::Instance ()->GetValue<float> ("rsm_intensity", 0.0f);
		float rsmIntensity = lastRSMIntensity;
		ImGui::InputFloat ("Indirect Light Intensity", &rsmIntensity);

		if (lastRSMResolution.x != rsmResolution [0] || lastRSMResolution.y != rsmResolution [1]) {
			SettingsManager::Instance ()->SetValue ("rsm_resolution",
				std::to_string (rsmResolution [0]) + "," + std::to_string (rsmResolution [1]));
		}

		if (lastRSMSamplesSize != rsmSamplesSize) {
			SettingsManager::Instance ()->SetValue ("rsm_samples", std::to_string (rsmSamplesSize));
		}

		if (lastRSMRadius != rsmRadius) {
			SettingsManager::Instance ()->SetValue ("rsm_radius", std::to_string (rsmRadius));
		}

		if (lastRSMIntensity != rsmIntensity) {
			SettingsManager::Instance ()->SetValue ("rsm_intensity", std::to_string (rsmIntensity));
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

			Texture* texture = TextureManager::Instance ()->GetTexture (lutTexturePath);
			if (texture == nullptr) {
				texture = Resources::LoadTexture (std::string (lutTexturePath));
				TextureManager::Instance ()->AddTexture (texture);
			}

			auto texSize = texture->GetSize ();
			ImGui::Image((void*)(intptr_t) texture->GetGPUIndex (), ImVec2(texSize.width, texSize.height));

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
