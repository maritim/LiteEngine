#ifndef RESOURCE_H
#define RESOURCE_H

#include "Core/Interfaces/Object.h"

#include <map>

template <class T>
class Resource : public Object
{
protected:
	T* _source;
	std::size_t* _counter;
private:
	static std::map<T*, std::pair<std::string, std::size_t*>> _sources;
	static std::map<std::string, T*> _paths;

public:
	Resource (T* = nullptr, const std::string& path = "");
	~Resource ();
	Resource (const Resource& other);

	T& operator* ();
	const T& operator* () const;
	T* operator-> ();
	const T* operator-> () const;
	Resource<T>& operator= (const Resource<T>& other);
	Resource<T>& operator= (std::nullptr_t);
	bool operator== (const Resource<T>& other) const;
	bool operator!= (const Resource<T>& other) const;
	bool operator== (std::nullptr_t) const;
	bool operator!= (std::nullptr_t) const;

	static Resource<T> GetResource (const std::string& path);
};

template <class T>
std::map<T*, std::pair<std::string, std::size_t*>> Resource<T>::_sources;

template <class T>
std::map<std::string, T*> Resource<T>::_paths;

template <class T>
Resource<T>::Resource (T* source, const std::string& path) :
	_source (source),
	_counter (nullptr)
{
	if (_source == nullptr) {
		return;
	}

	auto itSource = _sources.find (source);

	if (itSource == _sources.end ()) {
		_counter = new std::size_t (0);
		_sources [_source] = std::pair<std::string, std::size_t*> (path, _counter);

		//TODO: Fix this
		_paths [path] = _source;
	}

	if (itSource != _sources.end ()) {
		_counter = itSource->second.second;
	}

	(*_counter) ++;
}

template <class T>
Resource<T>::Resource (const Resource& other) :
	_source (other._source),
	_counter (other._counter)
{
	if (_source == nullptr) {
		return;
	}

	(*_counter) ++;
}

template <class T>
Resource<T>::~Resource ()
{
	if (_source == nullptr) {
		return;
	}

	(*_counter) --;

	if (*_counter == 0) {
		auto itSource = _sources.find (_source);

		if (itSource == _sources.end ()) {
			//TODO: Fix this
		}

		_paths.erase (itSource->second.first);
		_sources.erase (_source);

		delete _source;
		delete _counter;
	}
}

template <class T>
T& Resource<T>::operator* ()
{
	return *_source;
}

template <class T>
const T& Resource<T>::operator* () const
{
	return *_source;
}

template <class T>
T* Resource<T>::operator-> ()
{
	return _source;
}

template <class T>
const T* Resource<T>::operator-> () const
{
	return _source;
}

template <class T>
Resource<T>& Resource<T>::operator= (const Resource<T>& other)
{
	if (_source == other._source) {
		return *this;
	}

	if (_source != nullptr) {
		(*_counter) --;

		if (*_counter == 0) {
			auto itSource = _sources.find (_source);

			if (itSource == _sources.end ()) {
				//TODO: Fix this
			}

			_paths.erase (itSource->second.first);
			_sources.erase (_source);

			delete _source;
			delete _counter;
		}
	}

	_source = other._source;
	_counter = other._counter;

	if (_source != nullptr) {
		(*_counter) ++;
	}

	return *this;
}

template <class T>
Resource<T>& Resource<T>::operator= (std::nullptr_t other)
{
	return (*this) = Resource<T> (nullptr);
}

template <class T>
bool Resource<T>::operator== (const Resource<T>& other) const
{
	return _source == other._source;
}

template <class T>
bool Resource<T>::operator!= (const Resource<T>& other) const
{
	return ! (*this == other);
}

template <class T>
bool Resource<T>::operator== (std::nullptr_t check) const
{
	return _source == nullptr;
}

template <class T>
bool Resource<T>::operator!= (std::nullptr_t check) const
{
	return ! (*this == check);
}

template <class T>
Resource<T> Resource<T>::GetResource (const std::string& path)
{
	auto itPaths = _paths.find (path);

	if (itPaths == _paths.end ()) {
		return nullptr;
	}

	return Resource (itPaths->second);
}

#endif