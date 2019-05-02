#ifndef RENDERSETTINGSMANAGER_H
#define RENDERSETTINGSMANAGER_H

#include "Core/Singleton/Singleton.h"

#include <map>
#include <string>

#include "Renderer/RenderSettings.h"

class RenderSettingsManager : public Singleton<RenderSettingsManager>
{
	friend Singleton<RenderSettingsManager>;

private:
	std::map<std::string, RenderSettings*> _settings;
	RenderSettings* _active;

public:
	void AddRenderSettings (RenderSettings* settings);
	RenderSettings* GetRenderSettings (const std::string& filename);

	void SetActive (RenderSettings* settings);
	RenderSettings* GetActive () const;
private:
	RenderSettingsManager ();
	~RenderSettingsManager ();
	RenderSettingsManager (const RenderSettingsManager&);
	RenderSettingsManager& operator=(const RenderSettingsManager&);
};

#endif