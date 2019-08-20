#ifndef MODELVIEW_H
#define MODELVIEW_H

#include "Core/Interfaces/Object.h"

#include <vector>
#include <string>

#include "Core/Resources/Resource.h"
#include "Renderer/RenderViews/MaterialView.h"

struct ObjectBuffer
{
	unsigned int VAO_INDEX;
	unsigned int VBO_INDEX;
	unsigned int VBO_INSTANCE_INDEX;
	unsigned int IBO_INDEX;
	Resource<MaterialView> materialView;
	std::size_t INDEX_COUNT;
	std::size_t INSTANCES_COUNT;
};

class ModelView : public Object
{
protected:
	std::vector<ObjectBuffer> _objectBuffers;

public:
	~ModelView ();

	virtual void Draw ();

	void AddObjectBuffer (const ObjectBuffer& objectBuffer);

	std::vector<ObjectBuffer>::iterator begin ();
	std::vector<ObjectBuffer>::iterator end ();
};

#endif