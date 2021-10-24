#ifndef ANIMATIONMODELLOADER_H
#define ANIMATIONMODELLOADER_H

#include "Resources/ResourceLoader.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Renderer/Render/Mesh/AnimationModel.h"
#include "Renderer/Render/Mesh/AnimationsController.h"
#include "Renderer/Render/Mesh/AnimationContainer.h"

class AnimationModelLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	AnimationModel* GetAnimationModel (TiXmlElement* xmlElem, const std::string& filename);

	void ProcessAnimation (TiXmlElement* xmlElem, AnimationsController* animController, const std::string& filename);
};

#endif