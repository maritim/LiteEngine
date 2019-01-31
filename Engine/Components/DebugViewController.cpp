#include "DebugViewController.h"

#include <glm/vec2.hpp>
#include "Editor/ImGui/imgui.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"

#include "Renderer/RenderManager.h"

void DebugViewController::Start ()
{
	_continuousVoxelizationReset = false;
}

void DebugViewController::Update ()
{
	std::size_t windowHeight = Window::GetHeight ();

	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, (int) (0.9 * windowHeight)), ImGuiCond_FirstUseEver);

	ImGuiWindowFlags windowFlags = 0;

	if (!ImGui::Begin("Rendering Settings", NULL, windowFlags)) {
		ImGui::End();

		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	int lastRenderModule = SettingsManager::Instance ()->GetValue<int> ("render_module", 0);
	int renderModule = lastRenderModule;
	ImGui::Combo("Render Module", &renderModule, "Direct Light\0Voxel Cone Trace\0Reflective Shadow Map\0");

	if (lastRenderModule != renderModule) {
		SettingsManager::Instance ()->SetValue ("render_module", std::to_string (renderModule));

		if (renderModule == 0) {
			RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_DIRECT_LIGHTING);
			SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (_lastAmbientOcclusionEnabled));
		}

		if (renderModule == 1) {
			RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_VOXEL_CONE_TRACE);

			_lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
			SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (false));
		}

		if (renderModule == 2) {
			RenderManager::Instance ()->SetRenderMode (RenderMode::RENDER_MODE_REFLECTIVE_SHADOW_MAP);

			_lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
			SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (false));
		}
	}

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
	}

    ImGui::Spacing();

	if (ImGui::CollapsingHeader ("Post Processing")) {
		if (ImGui::TreeNode ("Screen Space Ambient Occlusion")) {
			bool lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
			bool ambientOcclusionEnabled = lastAmbientOcclusionEnabled;
			ImGui::Checkbox ("Enabled", &ambientOcclusionEnabled);

			int lastSamplesSize = SettingsManager::Instance ()->GetValue<int> ("ssao_samples", 0);
			int samplesSize = lastSamplesSize;
			ImGui::SliderInt ("Samples Size", &samplesSize, 0, 63);

			glm::vec2 lastNoiseMapResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_noise_resolution", glm::vec2 (0, 0));
			int noiseMapResolution [2] { (int) lastNoiseMapResolution.x, (int) lastNoiseMapResolution.y };
			ImGui::InputInt2 ("Noise Resolution", noiseMapResolution);

			float lastSSAORadius = SettingsManager::Instance ()->GetValue<float> ("ssao_radius", 0.0f);
			float ssaoRadius = lastSSAORadius;
			ImGui::InputFloat ("Radius", &ssaoRadius, 0.1f);

			float lasstSSAOBias = SettingsManager::Instance ()->GetValue<float> ("ssao_bias", 0.0f);
			float ssaoBias = lasstSSAOBias;
			ImGui::InputFloat ("Bias", &ssaoBias, 0.1f);

			bool lastSSAOBlurEnabled = SettingsManager::Instance ()->GetValue<bool> ("ssao_blur", false);
			bool ssaoBlurEnabled = lastSSAOBlurEnabled;
			ImGui::Checkbox ("Blur Enabled", &ssaoBlurEnabled);

			if (lastSamplesSize != samplesSize) {
				SettingsManager::Instance ()->SetValue ("ssao_samples", std::to_string (samplesSize));
			}

			if (lastNoiseMapResolution.x != noiseMapResolution [0] || lastNoiseMapResolution.y != noiseMapResolution [1]) {
				SettingsManager::Instance ()->SetValue ("ssao_noise_resolution", 
					std::to_string (noiseMapResolution [0]) + "," + std::to_string (noiseMapResolution [1]));
			}

			if (lastSSAORadius != ssaoRadius) {
				SettingsManager::Instance ()->SetValue ("ssao_radius", std::to_string (ssaoRadius));
			}

			if (lasstSSAOBias != ssaoBias) {
				SettingsManager::Instance ()->SetValue ("ssao_bias", std::to_string (ssaoBias));
			}

			if (lastSSAOBlurEnabled != ssaoBlurEnabled) {
				SettingsManager::Instance ()->SetValue ("ssao_blur", std::to_string (ssaoBlurEnabled));
			}

			if (lastAmbientOcclusionEnabled != ambientOcclusionEnabled && renderModule == 0) {
				SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (ambientOcclusionEnabled));
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("High Dynamic Range")) {
			bool lastHDREnabled = SettingsManager::Instance ()->GetValue<bool> ("high_dynamic_range", false);
			bool hdrEnabled = lastHDREnabled;
			ImGui::Checkbox ("Enabled", &hdrEnabled);

			float lastExposure = SettingsManager::Instance ()->GetValue<float> ("hdr_exposure", 0.0f);
			float exposure = lastExposure;
			ImGui::InputFloat ("Exposure", &exposure, 0.1f);

			if (lastExposure != exposure) {
				SettingsManager::Instance ()->SetValue ("hdr_exposure", std::to_string (exposure));
			}

			if (lastHDREnabled != hdrEnabled) {
				SettingsManager::Instance ()->SetValue ("high_dynamic_range", std::to_string (hdrEnabled));
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Gamma Correction")) {
			bool lastGammaCorrectionEnabled = SettingsManager::Instance ()->GetValue<bool> ("gamma_correction", false);
			bool gammaCorrectionEnabled = lastGammaCorrectionEnabled;
			ImGui::Checkbox ("Enabled", &gammaCorrectionEnabled);

			if (lastGammaCorrectionEnabled != gammaCorrectionEnabled) {
				SettingsManager::Instance ()->SetValue ("gamma_correction", std::to_string (gammaCorrectionEnabled));
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();
}
