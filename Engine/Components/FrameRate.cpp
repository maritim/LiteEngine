#include "FrameRate.h"
#include "Systems/Time/Time.h"

#include <iostream>

#include "Resources/Resources.h"
#include "SceneNodes/TextGUI.h"
#include "Managers/SceneManager.h"

FrameRate::FrameRate () :
	_timeElapsed (0),
	_computeRange (0.3f)
{

}

void FrameRate::Start ()
{
	Font* font = Resources::LoadBitmapFont ("Assets/Fonts/Fonts/sans.fnt");

	_textGUI = new TextGUI ("", font, Vector3 (0, 0.9));
	_textGUI->GetTransform ()->SetScale (Vector3 (0.7, 0.7, 0));
	SceneManager::Instance ()->Current ()->AttachObject (_textGUI);
}

void FrameRate::Update() 
{
	_timeElapsed += Time::GetDeltaTime ();

	static float lastFrameRate = -1;

	float dt = Time::GetDeltaTime();
	float roughFrameRate = 1.0 / (dt == 0 ? 1 : dt);

	if(lastFrameRate == -1) {
		lastFrameRate = roughFrameRate;
	} else {
		lastFrameRate = 0.9 * lastFrameRate + 0.1 * roughFrameRate;
	}

	if (_timeElapsed < _computeRange) {
		return ;
	}

	_textGUI->SetText (std::to_string (lastFrameRate));
	_timeElapsed = 0;
}

FrameRate::~FrameRate()
{

}