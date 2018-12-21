#ifndef RENDERVOLUMECOLLECTION_H
#define RENDERVOLUMECOLLECTION_H

#include "Core/Interfaces/Object.h"

#include <map>
#include <stack>
#include <string>

#include "RenderVolumeI.h"

class RenderVolumeCollectionIterator;

class RenderVolumeCollection : public Object
{
protected:
	std::map<std::string, RenderVolumeI*> _renderVolumes;
	std::stack<std::pair<std::string, std::size_t>> _scopedVolumes;
	std::size_t _currentLevel;
	std::string _lastRenderVolumeName;

public:
	RenderVolumeCollection ();

	RenderVolumeCollection* Insert (const std::string& name, RenderVolumeI* volume);
	RenderVolumeCollection* InsertScoped (const std::string& name, RenderVolumeI* volume);

	RenderVolumeCollection* StartScope ();
	RenderVolumeCollection* ReleaseScope ();

	RenderVolumeI* GetRenderVolume (const std::string& name) const;
	RenderVolumeI* GetPreviousVolume () const;

	RenderVolumeCollectionIterator begin ();
	RenderVolumeCollectionIterator end ();
};

class RenderVolumeCollectionIterator
{
	friend RenderVolumeCollection;

protected:
	std::map<std::string, RenderVolumeI*>::iterator _it;

public:
	RenderVolumeCollectionIterator& operator++ ();
	bool operator != (const RenderVolumeCollectionIterator& other);
	RenderVolumeI* operator* ();
protected:
	RenderVolumeCollectionIterator (std::map<std::string, RenderVolumeI*>::iterator it);
};

#endif