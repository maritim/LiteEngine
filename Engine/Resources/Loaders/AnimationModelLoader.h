#ifndef ANIMATIONMODELLOADER_H
#define ANIMATIONMODELLOADER_H

#include "Resources/ResourceLoader.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Mesh/AnimationModel.h"
#include "Mesh/AnimationsController.h"
#include "Mesh/AnimationContainer.h"

class AnimationModelLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	AnimationModel* GetAnimationModel (TiXmlElement* xmlElem, const std::string& filename);

	void ProcessAnimation (TiXmlElement* xmlElem, AnimationsController* animController, const std::string& filename);
};

#endif