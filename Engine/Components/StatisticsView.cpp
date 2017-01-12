#include "StatisticsView.h"

#include "Resources/Resources.h"
#include "Managers/SceneManager.h"

#include "Debug/Statistics/StatisticsManager.h"
#include "Debug/Statistics/StatisticsObject.h"
#include "Debug/Statistics/DrawnObjectsCountStat.h"

void StatisticsView::Start ()
{
	Font* font = Resources::LoadBitmapFont ("Assets/Fonts/Fonts/arial.fnt");

	_textGUI = new TextGUI ("", font, glm::vec2 (0.0f, 0.0f));
	_textGUI->GetTransform ()->SetScale (glm::vec3 (0.7f, 0.7f, 0.0f));
	SceneManager::Instance ()->Current ()->AttachObject (_textGUI);
}

void StatisticsView::Update ()
{
	StatisticsObject* stat = StatisticsManager::Instance ()->GetStatisticsObject ("DrawnObjectsCount");
	std::size_t drawnObjectsCount = dynamic_cast<DrawnObjectsCountStat*> (stat)->GetDrawnObjectsCount ();

	_textGUI->SetText ("Total objects drawn: " + std::to_string (drawnObjectsCount));	
}
