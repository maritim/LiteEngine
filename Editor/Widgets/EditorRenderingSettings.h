#ifndef EDITORRENDERINGSETTINGS_H
#define EDITORRENDERINGSETTINGS_H

#include "EditorWidget.h"
#include "EditorManager.h"

#include "Renderer/RenderSettings.h"

#include "Core/Resources/Resource.h"
#include "Texture/Texture.h"
#include "Renderer/RenderViews/TextureView.h"

class EditorRenderingSettings : public EditorWidget
{
protected:
	RenderSettings* _settings;

	std::string _lastLUTTexturePath;
	Resource<Texture> _lutTexture;
	Resource<TextureView> _lutTextureView;

	bool _continuousVoxelizationReset;
	bool _lastContinuousVoxelization;
	bool _lastAmbientOcclusionEnabled;
	bool _loadLUTTexture;

public:
	EditorRenderingSettings ();

	void Show ();
protected:
	void ShowRenderingSettingsWindow ();
};

REGISTER_EDITOR_WIDGET(EditorRenderingSettings)

#endif