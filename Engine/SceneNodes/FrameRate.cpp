#include "FrameRate.h"
#include "Systems/Time/Time.h"

#include <iostream>

#include "Resources/Resources.h"
#include "SceneNodes/TextGUI.h"

FrameRate::FrameRate (Scene* currentScene)
{
	Font* font = Resources::LoadBitmapFont ("Assets/Fonts/Fonts/sans.fnt");

	_textGUI = new TextGUI ("", font, Vector3 (0, 0.9));
	_textGUI->GetTransform ()->SetScale (Vector3 (0.7, 0.7, 0));
	currentScene->AttachObject (_textGUI);
}

void FrameRate::Update() 
{
	static float lastFrameRate = -1;

	float dt = Time::GetDeltaTime();
	float roughFrameRate = 1.0 / (dt == 0 ? 1 : dt);

	if(lastFrameRate == -1) {
		lastFrameRate = roughFrameRate;
	} else {
		lastFrameRate = 0.9 * lastFrameRate + 0.1 * roughFrameRate;
	}

	_textGUI->SetText (std::to_string (lastFrameRate));
}

void FrameRate::Draw()
{

}

FrameRate::~FrameRate()
{

}