#include "EditorRenderModule.h"

#include "RenderPasses/GUI/GUIRenderPass.h"

void EditorRenderModule::Init ()
{
	_renderPasses.push_back (new GUIRenderPass ());
}
