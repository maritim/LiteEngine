#ifndef RENDERMODULEMANAGER_H
#define RENDERMODULEMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Renderer/RenderModule.h"

#define REGISTER_RENDER_MODULE(RENDER_MODULE) static RenderModuleRegister<RENDER_MODULE> dummy (#RENDER_MODULE);

class ENGINE_API RenderModuleManager : public Singleton<RenderModuleManager>
{
	friend Singleton<RenderModuleManager>;

	DECLARE_SINGLETON(RenderModuleManager)

private:
	std::map<std::string, RenderModule*> _renderModules;

public:
	void RegisterRenderModule (const std::string& name, RenderModule* renderModule);
	RenderModule* GetRenderModule (const std::string& name);

	void Clear ();
private:
	RenderModuleManager ();
	~RenderModuleManager ();
	RenderModuleManager (const RenderModuleManager&);
	RenderModuleManager& operator=(const RenderModuleManager&);
};

template <class RenderModuleType>
class RenderModuleRegister : public Object
{
public:
	RenderModuleRegister (const std::string& name)
	{
		RenderModuleManager::Instance ()->RegisterRenderModule (name, new RenderModuleType ());
	}
};

#endif