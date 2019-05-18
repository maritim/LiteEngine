#ifndef COMPONENTSFACTORY_H
#define COMPONENTSFACTORY_H

/*
 * Thanks to: http://www.drdobbs.com/conversations-abstract-factory-template/184403786
 * for generic Factory Design Pattern ideas
*/

#include "Core/Singleton/Singleton.h"

#include <string>
#include <map>

#include "Component.h"

#define REGISTER_COMPONENT(COMPONENT) static RegisterComponent<COMPONENT> dummy (#COMPONENT);

class ENGINE_API ComponentsFactory : public Singleton<ComponentsFactory>
{
	friend Singleton<ComponentsFactory>;

	DECLARE_SINGLETON(ComponentsFactory)

private:
	typedef Component* (*CreateCompFn) ();

	std::map<std::string, CreateCompFn> _workers; 

public:
	Component* Create (const std::string& name);

	void RegCreateFn (const std::string& name, CreateCompFn fptr);
private:
	ComponentsFactory ();
	~ComponentsFactory ();
	ComponentsFactory (const ComponentsFactory&);
	ComponentsFactory& operator=(const ComponentsFactory&);
};

template <class ManufacturedType>
class RegisterComponent
{
public:
	static Component* CreateInstance()
	{
		return new ManufacturedType;
	}

	RegisterComponent(const std::string& id)
	{
		ComponentsFactory::Instance()->RegCreateFn(id, CreateInstance);
	}
};

#endif