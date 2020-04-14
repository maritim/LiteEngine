#include "FrameRate.h"
#include "Systems/Time/Time.h"

#include <iostream>

#include "Resources/Resources.h"

FrameRate::FrameRate () :
	_timeElapsed (0),
	_computeRange (0.3f)
{

}

void FrameRate::Start ()
{
	_textGUIComponent = _parent->GetComponent<TextGUIComponent> ();

	Resource<Font> font = Resources::LoadBitmapFont ("Assets/Fonts/Fonts/sans.fnt");

	_textGUIComponent->SetFont (font);

	_parent->GetTransform ()->SetPosition (glm::vec3 (0.0f, 0.9f, 0.0f));
	_parent->GetTransform ()->SetScale (glm::vec3 (0.7f, 0.7f, 0.0f));
}

void FrameRate::Update() 
{
	_timeElapsed += Time::GetDeltaTime ();

	static float lastFrameRate = -1;

	float dt = Time::GetDeltaTime();
	float roughFrameRate = 1.0f / (dt == 0 ? 1 : dt);

	if(lastFrameRate == -1) {
		lastFrameRate = roughFrameRate;
	} else {
		lastFrameRate = 0.9f * lastFrameRate + 0.1f * roughFrameRate;
	}

	if (_timeElapsed < _computeRange) {
		return ;
	}

	_textGUIComponent->SetText (std::to_string (lastFrameRate));
	_timeElapsed = 0;
}

FrameRate::~FrameRate()
{

}