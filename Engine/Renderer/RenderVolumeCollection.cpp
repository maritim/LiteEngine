#include "RenderVolumeCollection.h"

RenderVolumeCollection::RenderVolumeCollection () :
	_currentLevel (0),
	_lastRenderVolumeName ()
{

}

RenderVolumeCollection* RenderVolumeCollection::Insert (const std::string& name, RenderVolumeI* volume)
{
	_renderVolumes [name] = volume;
	_lastRenderVolumeName = name;

	return this;
}

RenderVolumeCollection* RenderVolumeCollection::InsertScoped (const std::string& name, RenderVolumeI* volume)
{
	_scopedVolumes.push (std::pair<std::string, std::size_t> (name, _currentLevel));
	_renderVolumes [name] = volume;

	return this;
}

RenderVolumeCollection* RenderVolumeCollection::StartScope ()
{
	_currentLevel ++;

	return this;
}

RenderVolumeCollection* RenderVolumeCollection::ReleaseScope ()
{
	while (_scopedVolumes.empty () == false &&
		_scopedVolumes.top ().second == _currentLevel) {

		std::string volumeName = _scopedVolumes.top ().first;

		auto it = _renderVolumes.find (volumeName);

		if (it != _renderVolumes.end ()) {
			_renderVolumes.erase (it);
		}

		_scopedVolumes.pop ();
	}

	-- _currentLevel;

	return this;
}

RenderVolumeI* RenderVolumeCollection::GetRenderVolume (const std::string& name) const
{
	auto it = _renderVolumes.find (name);

	if (it == _renderVolumes.end ()) {
		return nullptr;
	}

	return it->second;
}

RenderVolumeI* RenderVolumeCollection::GetPreviousVolume () const
{
	return GetRenderVolume (_lastRenderVolumeName);
}

RenderVolumeCollectionIterator RenderVolumeCollection::begin ()
{
	RenderVolumeCollectionIterator rvcIt (_renderVolumes.begin ());

	return rvcIt;
}

RenderVolumeCollectionIterator RenderVolumeCollection::end ()
{
	RenderVolumeCollectionIterator rvcIt (_renderVolumes.end ());

	return rvcIt;
}

/*
 * Render volume collection iterator
*/

RenderVolumeCollectionIterator& RenderVolumeCollectionIterator::operator++ ()
{
	_it++;

	return *this;
}

bool RenderVolumeCollectionIterator::operator != (const RenderVolumeCollectionIterator& other)
{
	return _it != other._it;
}

RenderVolumeI* RenderVolumeCollectionIterator::operator* ()
{
	return _it->second;
}

RenderVolumeCollectionIterator::RenderVolumeCollectionIterator (std::map<std::string, RenderVolumeI*>::iterator it) :
	_it (it)
{

}