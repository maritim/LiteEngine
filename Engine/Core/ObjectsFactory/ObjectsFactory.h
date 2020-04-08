#ifndef OBJECTSFACTORY_H
#define OBJECTSFACTORY_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

template<class T>
class ENGINE_API ObjectsFactory : public Object
{
private:
	static ObjectsFactory* _instance;

	typedef T* (*CreateCompFn) ();

	std::map<std::string, CreateCompFn> _workers; 

public:
	static ObjectsFactory* Instance ();

	T* Create (const std::string& name);

	void RegCreateFn (const std::string& name, CreateCompFn fptr);
private:
	ObjectsFactory ();
	~ObjectsFactory ();
	ObjectsFactory (const ObjectsFactory&);
	ObjectsFactory& operator=(const ObjectsFactory&);
};

template<class T>
ObjectsFactory<T>::ObjectsFactory ()
{

}

template<class T>
ObjectsFactory<T>::~ObjectsFactory ()
{

}

template<class T>
ObjectsFactory<T>* ObjectsFactory<T>::_instance;

template<class T>
ObjectsFactory<T>* ObjectsFactory<T>::Instance ()
{
	if (_instance == nullptr) {
		_instance = new ObjectsFactory<T> ();
	}
	return _instance;
}

template<class T>
T* ObjectsFactory<T>::Create (const std::string& name)
{
	typename std::map<std::string, CreateCompFn>::iterator it = _workers.find (name);

	if (it == _workers.end ()) {
		return nullptr;
	}

	return (it->second) ();
} 

template<class T>
void ObjectsFactory<T>::RegCreateFn (const std::string& name, CreateCompFn fptr)
{
	_workers.insert (std::pair<std::string, CreateCompFn> (name, fptr));
}

template <class ObjectType, class ManufacturedType>
class RegisterObject
{
public:
	static ObjectType* CreateInstance()
	{
		return new ManufacturedType;
	}

	RegisterObject(const std::string& id)
	{
		ObjectsFactory<ObjectType>::Instance()->RegCreateFn(id, CreateInstance);
	}
};

#endif