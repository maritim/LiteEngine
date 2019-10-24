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
	std::size_t INSTANCES_COUNT;

	std::size_t VerticesCount;
	std::size_t PolygonsCount;
};

struct GroupBuffer
{
	Resource<MaterialView> materialView;
	std::size_t INDEX_COUNT;
	std::size_t offset;
};

class ModelView : public Object
{
protected:
	ObjectBuffer _objectBuffer;
	std::vector<GroupBuffer> _groupBuffers;

public:
	~ModelView ();

	virtual void Draw ();

	void SetObjectBuffer (const ObjectBuffer& objectBuffer);

	void AddGroupBuffer (const GroupBuffer& groupBuffer);

	ObjectBuffer& GetObjectBuffer ();

	std::size_t GetVerticesCount () const;
	std::size_t GetPolygonsCount () const;

	std::vector<GroupBuffer>::iterator begin ();
	std::vector<GroupBuffer>::iterator end ();
};

#endif