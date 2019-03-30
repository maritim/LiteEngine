#ifndef ANIMATIONMODELLOADER_H
#define ANIMATIONMODELLOADER_H

#include "ResourceLoader.h"

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "Mesh/AnimationModel.h"
#include "Mesh/AnimationsController.h"
#include "Mesh/AnimationContainer.h"

#include "Mesh/BoneNode.h"

class AnimationModelLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& fileName);

protected:
	AnimationModel* GetAnimationModel (TiXmlElement* xmlElem);

	void ProcessAnimation (TiXmlElement* xmlElem, AnimationsController* animController);
};

#endif