#include "ComponentsFactory.h"

ComponentsFactory::ComponentsFactory ()
{

}

ComponentsFactory::~ComponentsFactory ()
{

}

SPECIALIZE_SINGLETON(ComponentsFactory)

Component* ComponentsFactory::Create (const std::string& name)
{
	std::map<std::string, CreateCompFn>::iterator it = _workers.find (name);

	if (it == _workers.end ()) {
		return nullptr;
	}

	return (it->second) ();
} 

void ComponentsFactory::RegCreateFn (const std::string& name, CreateCompFn fptr)
{
	_workers.insert (std::pair<std::string, CreateCompFn> (name, fptr));
}