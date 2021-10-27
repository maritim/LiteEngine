#include "EditorStats.h"

#include <sstream>
#include <iomanip>
#include <glm/vec2.hpp>
#include <ImGui/imgui.h>

#include "EditorScene.h"

#include "Systems/Time/Time.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "RenderPasses/RenderStatisticsObject.h"

EditorStats::EditorStats () :
	_timeElapsed (0.0f),
	_computeRange (0.3f),
	_firstTime (true)
{

}

void EditorStats::Show ()
{
	ShowStatistics ();
}

void EditorStats::ShowStatistics ()
{
	_timeElapsed += Time::GetDeltaTime ();

	float dt = Time::GetDeltaTime();
	float roughFrameRate = 1.0f / (dt == 0 ? 1 : dt);

	if (_firstTime == true) {
		_lastFrameRate = roughFrameRate;
		_firstTime = false;
	} else {
		_lastFrameRate = 0.9f * _lastFrameRate + 0.1f * roughFrameRate;
	}

	if (_timeElapsed > _computeRange) {
		_frameRates.push_back (_lastFrameRate);

		_timeElapsed = 0;
	}

	if (_frameRates.size () > 100) {
		_frameRates.erase (_frameRates.begin ());
	}

	glm::ivec2 sceneWindowPos = EditorScene::Instance ()->GetWindowPosition ();
	glm::ivec2 sceneWindowSize = EditorScene::Instance ()->GetWindowSize ();

	ImGui::SetNextWindowPos(ImVec2 (sceneWindowPos.x + sceneWindowSize.x - 300, sceneWindowPos.y + 45), ImGuiCond_Always);
	ImGui::SetNextWindowSize (ImVec2 (200, 100), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowBgAlpha(0.3f);

	if (ImGui::Begin("Statistics", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav)) {

		ImGui::Text ("Statistics");
		ImGui::Separator ();

		ImGui::Spacing ();

		ImGui::Text("Graphics: %.2f FPS (%.2f ms)", _lastFrameRate, (1000.0f / _lastFrameRate));

		ImGui::Spacing ();

		auto renderStatisticsObject = StatisticsManager::Instance ()->GetStatisticsObject <RenderStatisticsObject> ();

		std::size_t drawnVerticesCount = renderStatisticsObject->DrawnVerticesCount;
		std::size_t drawnPolygonsCount = renderStatisticsObject->DrawnPolygonsCount;
		std::size_t drawnObjectsCount = renderStatisticsObject->DrawnObjectsCount;

		std::string verticesCount = std::to_string (drawnVerticesCount % 1000);
		while (drawnVerticesCount /= 1000) {
			verticesCount = std::to_string (drawnVerticesCount % 1000) + "," + verticesCount;
		}

		std::string polygonsCount = std::to_string (drawnPolygonsCount % 1000);
		while (drawnPolygonsCount /= 1000) {
			polygonsCount = std::to_string (drawnPolygonsCount % 1000) + "," + polygonsCount;
		}

		ImGui::Text ("Vertices: %s Triangles: %s", verticesCount.c_str (), polygonsCount.c_str ());
		ImGui::Text ("Objects: %lu", drawnObjectsCount);

		ImGui::Spacing ();

		ImGui::Text ("Window Resolution: %dx%d", sceneWindowSize.x, sceneWindowSize.y);

		ImGui::Spacing ();

		std::stringstream ss;
		ss << std::fixed << std::setprecision (2) << _lastFrameRate;
		std::string frameRate = ss.str ();

		ImGui::PlotLines (std::string (frameRate + " FPS").c_str (), _frameRates.data (), _frameRates.size ());
	}

	ImGui::End();
}