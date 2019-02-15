#include "RenderVolumeCollection.h"

RenderVolumeCollection::RenderVolumeCollection () :
	_currentLevel (0),
	_lastRenderVolumeName ()
{

}

RenderVolumeCollection* RenderVolumeCollection::Insert (const std::string& name, RenderVolumeI* volume, bool external)
{
	_scopedVolumes.push (std::pair<std::string, std::size_t> (name, _currentLevel));

	auto it = _nestedRenderVolumes.find (name);

	if (it == _nestedRenderVolumes.end ()) {
		_nestedRenderVolumes [name] = std::stack<std::pair<RenderVolumeI*, std::size_t>> ();
	}

	it = _nestedRenderVolumes.find (name);

	if (it->second.empty () == false && it->second.top ().second == _currentLevel) {
		it->second.pop ();
	}

	it->second.push (std::pair<RenderVolumeI*, std::size_t> (volume, _currentLevel));

	if (external == true) {
		_lastRenderVolumeName = name;
	}

	return this;
}

RenderVolumeCollection* RenderVolumeCollection::StartScope ()
{
	_currentLevel ++;

	return this;
}

RenderVolumeCollection* RenderVolumeCollection::ReleaseScope ()
{
	RenderVolumeI* lastRenderVolume = GetRenderVolume (_lastRenderVolumeName);

	while (_scopedVolumes.empty () == false &&
		_scopedVolumes.top ().second == _currentLevel) {

		std::string volumeName = _scopedVolumes.top ().first;

		auto it = _nestedRenderVolumes.find (volumeName);

		if (it != _nestedRenderVolumes.end ()) {
			if (it->second.top ().second == _currentLevel) {
				it->second.pop ();
			}

			if (it->second.empty () == true) {
				_nestedRenderVolumes.erase (it);
			}
		}

		_scopedVolumes.pop ();
	}

	-- _currentLevel;

	Insert (_lastRenderVolumeName, lastRenderVolume);

	return this;
}

RenderVolumeI* RenderVolumeCollection::GetRenderVolume (const std::string& name) const
{
	auto it = _nestedRenderVolumes.find (name);

	if (it == _nestedRenderVolumes.end ()) {
		return nullptr;
	}

	return it->second.top ().first;
}

RenderVolumeI* RenderVolumeCollection::GetPreviousVolume () const
{
	return GetRenderVolume (_lastRenderVolumeName);
}

RenderVolumeCollectionIterator RenderVolumeCollection::begin ()
{
	RenderVolumeCollectionIterator rvcIt (_nestedRenderVolumes.begin ());

	return rvcIt;
}

RenderVolumeCollectionIterator RenderVolumeCollection::end ()
{
	RenderVolumeCollectionIterator rvcIt (_nestedRenderVolumes.end ());

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
	return _it->second.top ().first;
}

RenderVolumeCollectionIterator::RenderVolumeCollectionIterator (std::map<std::string, std::stack<std::pair<RenderVolumeI*, std::size_t>>>::iterator it) :
	_it (it)
{

}