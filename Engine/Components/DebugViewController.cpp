#include "DebugViewController.h"

#include <sstream>
#include <iomanip>
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
	ShowRenderingSettingsWindow ();
}

void DebugViewController::ShowRenderingSettingsWindow ()
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
			bool lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
			bool ambientOcclusionEnabled = lastAmbientOcclusionEnabled;
			ImGui::Checkbox ("Enabled", &ambientOcclusionEnabled);

			glm::vec2 lastResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssao_resolution", glm::vec2 (0, 0));
			int resolution [2] { (int) lastResolution.x, (int) lastResolution.y };
			ImGui::InputInt2 ("Resolution", resolution);

			int lastSamplesSize = SettingsManager::Instance ()->GetValue<int> ("ssao_samples", 0);
			int samplesSize = lastSamplesSize;
			ImGui::SliderInt ("Samples Size", &samplesSize, 1, 64);

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

			if (lastResolution.x != resolution [0] || lastResolution.y != resolution [1]) {
				SettingsManager::Instance ()->SetValue ("ssao_resolution",
					std::to_string (resolution [0]) + "," + std::to_string (resolution [1]));
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

		if (ImGui::TreeNode ("Screen Space Reflection")) {
			bool lastScreenSpaceReflectionEnabled = SettingsManager::Instance ()->GetValue<bool> ("screen_space_reflection", false);
			bool screenSpaceReflectionEnabled = lastScreenSpaceReflectionEnabled;
			ImGui::Checkbox ("Enabled", &screenSpaceReflectionEnabled);

			glm::vec2 lastResolution = SettingsManager::Instance ()->GetValue<glm::vec2> ("ssr_resolution", glm::vec2 (0, 0));
			int resolution [2] { (int) lastResolution.x, (int) lastResolution.y };
			ImGui::InputInt2 ("Resolution", resolution);

			float lastRoughness = SettingsManager::Instance ()->GetValue<float> ("ssr_roughness", 0.0f);
			float roughness = lastRoughness;
			ImGui::SliderFloat ("Roughness", &roughness, 0.0f, 1.0f);

			int lastSampleIterations = SettingsManager::Instance ()->GetValue<int> ("ssr_iterations", 0);
			int sampleIterations = lastSampleIterations;
			ImGui::InputInt ("Sample Iterations", &sampleIterations, 1);

			float lastSSRSampleSkip = SettingsManager::Instance ()->GetValue<float> ("ssr_sample_skip", 0.0f);
			float ssrSampleSkip = lastSSRSampleSkip;
			ImGui::InputFloat ("Spatial Sample Skip", &ssrSampleSkip, 0.1f);

			float lastSSRSpatialBias = SettingsManager::Instance ()->GetValue<float> ("ssr_spatial_bias", 0.0f);
			float ssrSpatialBias = lastSSRSpatialBias;
			ImGui::InputFloat ("Spatial Bias", &ssrSpatialBias, 0.1f);

			float lastSSRIntensity = SettingsManager::Instance ()->GetValue<float> ("ssr_intensity", 0.0f);
			float ssrIntensity = lastSSRIntensity;
			ImGui::InputFloat ("Intensity", &ssrIntensity, 0.1f);

			if (lastResolution.x != resolution [0] || lastResolution.y != resolution [1]) {
				SettingsManager::Instance ()->SetValue ("ssr_resolution",
					std::to_string (resolution [0]) + "," + std::to_string (resolution [1]));
			}

			if (lastSampleIterations != sampleIterations) {
				SettingsManager::Instance ()->SetValue ("ssr_iterations", std::to_string (sampleIterations));
			}

			if (lastRoughness != roughness) {
				SettingsManager::Instance ()->SetValue ("ssr_roughness", std::to_string (roughness));
			}

			if (lastSSRSampleSkip != ssrSampleSkip) {
				SettingsManager::Instance ()->SetValue ("ssr_sample_skip", std::to_string (ssrSampleSkip));
			}

			if (lastSSRSpatialBias != ssrSpatialBias) {
				SettingsManager::Instance ()->SetValue ("ssr_spatial_bias", std::to_string (ssrSpatialBias));
			}

			if (lastSSRIntensity != ssrIntensity) {
				SettingsManager::Instance ()->SetValue ("ssr_intensity", std::to_string (ssrIntensity));
			}

			if (lastScreenSpaceReflectionEnabled != screenSpaceReflectionEnabled && renderModule == 0) {
				SettingsManager::Instance ()->SetValue ("screen_space_reflection", std::to_string (screenSpaceReflectionEnabled));
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode ("Bloom")) {
			bool lastBloomEnabled = SettingsManager::Instance ()->GetValue<bool> ("bloom", false);
			bool bloomEnabled = lastBloomEnabled;
			ImGui::Checkbox ("Enabled", &bloomEnabled);

			float lastBloomThreshold = SettingsManager::Instance ()->GetValue<float> ("bloom_threshold", 0.0f);
			float bloomThreshold = lastBloomThreshold;
			ImGui::SliderFloat ("Threshold", &bloomThreshold, 0.0f, 10.0f);

			float lastBloomIntensity = SettingsManager::Instance ()->GetValue<float> ("bloom_intensity", 0.0f);
			float bloomIntensity = lastBloomIntensity;
			ImGui::InputFloat ("Intensity", &bloomIntensity, 0.1f);

			if (lastBloomThreshold != bloomThreshold) {
				SettingsManager::Instance ()->SetValue ("bloom_threshold", std::to_string (bloomThreshold));
			}

			if (lastBloomIntensity != bloomIntensity) {
				SettingsManager::Instance ()->SetValue ("bloom_intensity", std::to_string (bloomIntensity));
			}

			if (lastBloomEnabled != bloomEnabled) {
				SettingsManager::Instance ()->SetValue ("bloom", std::to_string (bloomEnabled));
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
