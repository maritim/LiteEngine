#include "EditorProfiler.h"

#include <ImGui/imgui.h>

#include "Debug/Profiler/Profiler.h"

#include "Systems/Settings/SettingsManager.h"

EditorProfiler::EditorProfiler () :
    _timeSpan (2.0f),
    _profilerType (ProfilerType::CPUProfiler),
    _totalCPUTime (0.0f),
    _totalGPUTime (0.0f)
{

}

void EditorProfiler::Show ()
{
    bool isProfilerVisible = SettingsManager::Instance ()->GetValue<bool> ("menu_show_profiler", false);

    if (isProfilerVisible == true) {
        UpdateProfiler ();

        ShowProfiler ();
    }
}

void EditorProfiler::UpdateProfiler ()
{
    auto profilerCPUService = Profiler::Instance ()->GetCPUProfilerService ();
    auto profilerGPUService = Profiler::Instance ()->GetGPUProfilerService ();

    UpdateProfilerStatistics (profilerCPUService->GetLastFrameEvents (),
        profilerCPUService->GetLastFrameTime (), _totalCPUTime,
        _profilerCPUEvents, _profilerCPUFrameTimes,
        _profilerCPUFrameTimeStat, _profilerCPUStats);
    UpdateProfilerStatistics (profilerGPUService->GetLastFrameEvents (),
        profilerGPUService->GetLastFrameTime (), _totalGPUTime,
        _profilerGPUEvents, _profilerGPUFrameTimes,
        _profilerGPUFrameTimeStat, _profilerGPUStats);
}

void EditorProfiler::ShowProfiler ()
{
    PROFILER_LOGGER("EditorProfiler")

    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Profiler", NULL))
    {
        ImGui::End();
        return;
    }

    ShowProfilerSettings ();

    ImGui::Separator ();

    ImGui::BeginChild("OuterRegion", ImVec2(0, 100), false);

    if (_profilerType == (int) ProfilerType::CPUProfiler) {
        ShowProfilerEvents (_profilerCPUFrameTimeStat, _profilerCPUStats);
    }

    if (_profilerType == (int) ProfilerType::GPUProfiler) {
        ShowProfilerEvents (_profilerGPUFrameTimeStat, _profilerGPUStats);
    }

    ImGui::EndChild();

    ImGui::Separator ();

    ImGui::BeginChild("OuterRegion2", ImVec2(0, 0), false);

    if (_profilerType == (int) ProfilerType::CPUProfiler) {
        std::size_t a = 0;
        ShowEvent (a, _profilerCPUStats);
    }

    if (_profilerType == (int) ProfilerType::GPUProfiler) {
        std::size_t a = 0;
        ShowEvent (a, _profilerGPUStats);
    }

    ImGui::EndChild();

    ImGui::End();
}

void EditorProfiler::ShowProfilerSettings ()
{
    bool isActive = Profiler::Instance ()->GetCPUProfilerService ()->IsActive ();

    if (isActive == false) {
        if (ImGui::Button ("Start")) {
            isActive = true;

            _profilerCPUStats.clear ();
            _profilerGPUStats.clear ();
        }
    } else {
        if (ImGui::Button ("Pause")) {
            isActive = false;
        }
    }

    Profiler::Instance ()->GetCPUProfilerService ()->SetActive (isActive);
    Profiler::Instance ()->GetGPUProfilerService ()->SetActive (isActive);

    ImGui::SameLine ();

    const char* items[] = { "CPU", "GPU" };
    ImGui::PushItemWidth (200);
    ImGui::Combo ("Profiler", &_profilerType, items, 2);

    ImGui::SameLine ();

    float lastTimeSpan = _timeSpan;
    ImGui::InputFloat ("Time Span", &_timeSpan, 1.0f);
    if (_timeSpan == 0) {
        _timeSpan = lastTimeSpan;
    }
}

void EditorProfiler::ShowProfilerEvents (float profilerFrameTimeStat, const std::vector<ProfilerEntry>& profilerStats)
{
    if (profilerStats.size () == 0) {
        return;
    }

    std::size_t maxDepth = 0;

    for (auto profilerEvent : profilerStats) {
        maxDepth = std::max (maxDepth, profilerEvent.NestDepth);
    }

    std::size_t windowWidth = ImGui::GetWindowWidth ();

    ImGui::BeginGroup();

    ImVec4 buttonColor((ImVec4)ImColor::HSV(5/7.0f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
    float buttonWidth = windowWidth;
    if (buttonWidth >= 6.0f) {
        ImGui::SetCursorPosX (0);
        ImGui::Button(std::to_string (profilerFrameTimeStat).c_str (), ImVec2(buttonWidth, 20));
        if (ImGui::IsItemHovered ()) {
            ImGui::SetTooltip ("Frame: %.3f", profilerFrameTimeStat);
        }
    }
    ImGui::PopStyleColor(3);

    ImGui::EndGroup();

    for (std::size_t depthIndex = 0; depthIndex <= maxDepth; depthIndex ++) {
        std::size_t eventIndex = 0;

        ImGui::BeginGroup();

        for (auto profilerEvent : profilerStats) {
            if (profilerEvent.NestDepth != depthIndex) {
                continue;
            }

            ImVec4 buttonColor((ImVec4)ImColor::HSV(eventIndex/7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColor);
            float buttonWidth = (profilerEvent.Duration / profilerFrameTimeStat) * windowWidth - 6;
            float buttonPos = (profilerEvent.StartTime / profilerFrameTimeStat) * windowWidth;
            if (buttonWidth >= 6.0f) {
                ImGui::SetCursorPosY (25 * (depthIndex + 1));
                ImGui::SetCursorPosX (buttonPos);
                ImGui::Button(std::to_string (profilerEvent.Duration).c_str (), ImVec2(buttonWidth, 20));
                if (ImGui::IsItemHovered ()) {
                    ImGui::SetTooltip ("%s: %.3f", profilerEvent.Name.c_str (), profilerEvent.Duration);
                }
            }
            ImGui::PopStyleColor(3);
            ImGui::SameLine();
            ++ eventIndex;
        }

        ImGui::EndGroup();
    }
}

void EditorProfiler::ShowEvent (std::size_t& profilerIndex, const std::vector<ProfilerEntry>& profilerStats)
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    if (profilerIndex >= profilerStats.size ()) {
        return;
    }

    std::size_t profilerDepth = profilerStats [profilerIndex].NestDepth;

    while (profilerIndex < profilerStats.size () &&
        profilerStats [profilerIndex].NestDepth == profilerDepth) {

        ImGuiTreeNodeFlags current_node_flags = node_flags;
        if (profilerIndex < profilerStats.size () && profilerStats [profilerIndex + 1].NestDepth != profilerDepth + 1) {
            current_node_flags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (ImGui::TreeNodeEx(&profilerStats + profilerIndex, current_node_flags, "%s -> %.3f (ms)",
            profilerStats [profilerIndex].Name.c_str (), profilerStats [profilerIndex].Duration)) {

            ++ profilerIndex;
            if (profilerIndex < profilerStats.size () && profilerStats [profilerIndex].NestDepth == profilerDepth + 1) {
                ShowEvent (profilerIndex, profilerStats);
            }

            ImGui::TreePop ();
        } else {
            ++ profilerIndex;
            while (profilerIndex < profilerStats.size () &&
                profilerStats [profilerIndex].NestDepth > profilerDepth) {
                ++ profilerIndex;
            }
        }
    }
}

void EditorProfiler::UpdateProfilerStatistics (const std::vector<ProfilerEntry*>& profilerEntries, 
    float lastFrameTime, float& totalTime, std::list<ProfilerEntry>& profilerEvents,
    std::list<float>& profilerFrameTimes, float& profilerFrameTimeStat,
    std::vector<ProfilerEntry>& profilerStats)
{
    if (Profiler::Instance ()->GetCPUProfilerService ()->IsActive () == false) {
        return;
    }

    if (profilerEntries.size () == 0) {
        return;
    }

    if (profilerEntries.size () != profilerStats.size ()) {
        profilerEvents.clear ();
        profilerStats.clear ();
        profilerFrameTimes.clear ();

        totalTime = 0;
    }

    /*
     * Update time span profiler events
    */

    std::size_t framesCount = profilerEvents.size () / profilerEntries.size ();

    totalTime += lastFrameTime / 1000.0f;

    std::size_t eventIndex = 0;
    for (auto profilerEvent : profilerEntries) {
        profilerEvents.push_back (*profilerEvent);

        if (profilerStats.size () <= eventIndex) {
            profilerStats.push_back (*profilerEvent);
            eventIndex ++;
            continue;
        }

        profilerStats [eventIndex].Duration =
            (profilerStats [eventIndex].Duration * framesCount +
            profilerEvent->Duration) / (framesCount + 1);
        profilerStats [eventIndex].StartTime =
            (profilerStats [eventIndex].StartTime * framesCount +
            profilerEvent->StartTime) / (framesCount + 1);

        eventIndex ++;
    }

    profilerFrameTimes.push_back (lastFrameTime);

    profilerFrameTimeStat = (profilerFrameTimeStat * framesCount +
        lastFrameTime) / (framesCount + 1);

    ++ framesCount;

    while (totalTime > _timeSpan && profilerEvents.empty () == false) {
        totalTime -= profilerFrameTimes.front () / 1000.0f;

        profilerFrameTimeStat = (profilerFrameTimeStat * framesCount -
            profilerFrameTimes.front ()) / (framesCount - 1);

        profilerFrameTimes.pop_front ();

        for (std::size_t eventIndex = 0; eventIndex < profilerEntries.size (); eventIndex ++) {

            profilerStats [eventIndex].Duration =
                (profilerStats [eventIndex].Duration * framesCount -
                profilerEvents.front ().Duration) / (framesCount - 1);
            profilerStats [eventIndex].StartTime =
                (profilerStats [eventIndex].StartTime * framesCount -
                profilerEvents.front ().StartTime) / (framesCount - 1);

            profilerEvents.pop_front ();
        }

        -- framesCount;
    }
}
