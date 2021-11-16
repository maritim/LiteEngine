#ifndef ANIMATIONMODELLOADER_H
#define ANIMATIONMODELLOADER_H

#include <tinyxml2.h>

#include "Resources/ResourceLoader.h"

#include "Renderer/Render/Mesh/AnimationModel.h"
#include "Renderer/Render/Mesh/AnimationsController.h"
#include "Renderer/Render/Mesh/AnimationContainer.h"

class AnimationModelLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	AnimationModel* GetAnimationModel (tinyxml2::XMLElement* xmlElem, const std::string& filename);

	void ProcessAnimation (tinyxml2::XMLElement* xmlElem, AnimationsController* animController, const std::string& filename);
};

#endif