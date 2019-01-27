#include "DebugViewController.h"

#include <glm/vec2.hpp>
#include "Editor/ImGui/imgui.h"

#include "Systems/Settings/SettingsManager.h"
#include "Systems/Window/Window.h"

void DebugViewController::Start ()
{

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

	if (ImGui::CollapsingHeader ("Screen Space Ambient Occlusion")) {
		bool lastAmbientOcclusionEnabled = SettingsManager::Instance ()->GetValue<bool> ("ambient_occlusion", false);
		bool ambientOcclusionEnabled = lastAmbientOcclusionEnabled;
		ImGui::Checkbox ("Enabled", &ambientOcclusionEnabled);

		int lastSamplesSize = SettingsManager::Instance ()->GetValue<int> ("ssao_samples", 0);
		int samplesSize = lastSamplesSize;
		ImGui::InputInt ("Samples Size", &samplesSize);

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

		if (lastAmbientOcclusionEnabled != ambientOcclusionEnabled) {
			SettingsManager::Instance ()->SetValue ("ambient_occlusion", std::to_string (ambientOcclusionEnabled));
		}
	}

	if (ImGui::CollapsingHeader ("High Dynamic Range")) {
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
	}

	if (ImGui::CollapsingHeader ("Gamma Correction")) {
		bool lastGammaCorrectionEnabled = SettingsManager::Instance ()->GetValue<bool> ("gamma_correction", false);
		bool gammaCorrectionEnabled = lastGammaCorrectionEnabled;
		ImGui::Checkbox ("Enabled", &gammaCorrectionEnabled);

		if (lastGammaCorrectionEnabled != gammaCorrectionEnabled) {
			SettingsManager::Instance ()->SetValue ("gamma_correction", std::to_string (gammaCorrectionEnabled));
		}
	}

	ImGui::End();
}
