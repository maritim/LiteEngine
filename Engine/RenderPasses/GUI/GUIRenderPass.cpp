#include "GUIRenderPass.h"

#include <cstdint>

#include "Systems/GUI/ImGui/imgui.h"

#include "Resources/Resources.h"
#include "Renderer/RenderSystem.h"

#include "Renderer/Pipeline.h"

void GUIRenderPass::Init (const RenderSettings& settings)
{
	/*
	 * Shader for gui objects
	*/

	Resource<Shader> shader = Resources::LoadShader ({
		"Assets/Shaders/GUI/guiVertex.glsl",
		"Assets/Shaders/GUI/guiFragment.glsl"
	});

	_shaderView = RenderSystem::LoadShader (shader);

	/*
	 * Load font texture
	*/

	GL::GenVertexArrays (1, &_VAO_ID);
	GL::BindVertexArray (_VAO_ID);

	GL::GenBuffers (1, &_VBO_ID);
	GL::GenBuffers (1, &_IBO_ID);

	GL::BindBuffer (GL_ARRAY_BUFFER, _VBO_ID);

	GL::EnableVertexAttribArray (0);
	GL::EnableVertexAttribArray (1);
	GL::EnableVertexAttribArray (2);
	GL::VertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, pos));
	GL::VertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof (ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, uv));
	GL::VertexAttribPointer (2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof (ImDrawVert), (GLvoid*)IM_OFFSETOF(ImDrawVert, col));
}

RenderVolumeCollection* GUIRenderPass::Execute (const RenderScene* renderScene, const Camera* camera,
	const RenderSettings& settings, RenderVolumeCollection* rvc)
{
	/*
	 * Start editor rendering pass
	*/

	StartEditorPass ();

	/*
	 * Editor rendering pass
	*/

	EditorPass (settings);

	/*
	 * End editor rendering pass
	*/

	EndEditorPass ();

	return rvc;
}

void GUIRenderPass::StartEditorPass ()
{
	/*
	 * Lock shader
	*/

	LockShader ();

	/*
	 * Bind framebuffer
	*/

	GL::BindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);
}

void GUIRenderPass::EditorPass (const RenderSettings& settings)
{
	/*
	 * Render ImGui
	*/

	ImGui::Render ();

	/*
	 * Get ImGui render data
	*/

	auto draw_data = ImGui::GetDrawData ();

	/*
	 * Set OpenGL state
	*/

	GL::Enable (GL_BLEND);
	GL::BlendEquation (GL_FUNC_ADD);
	GL::BlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GL::Disable (GL_CULL_FACE);
	GL::Disable (GL_DEPTH_TEST);
	GL::Enable (GL_SCISSOR_TEST);

	/*
	 * Set viewport size
	*/

	GL::Viewport (settings.viewport.x, settings.viewport.y,
		settings.viewport.width, settings.viewport.height);

	//GL::Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
	 * Activate font texture
	*/

	ImGuiIO& io = ImGui::GetIO();

	GL::ActiveTexture (GL_TEXTURE0);
	GL::BindTexture (GL_TEXTURE_2D, ((intptr_t) io.Fonts->TexID));

	/*
	 * Send custom attributes
	*/

	Pipeline::SendCustomAttributes (nullptr, GetCustomAttributes ());

	/*
	 * Render
	*/

	ImVec2 pos = draw_data->DisplayPos;

	for (int n = 0; n < draw_data->CmdListsCount; n++) {

		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		GL::BindVertexArray (_VAO_ID);

		GL::BindBuffer (GL_ARRAY_BUFFER, _VBO_ID);
		GL::BufferData (GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, _IBO_ID);
		GL::BufferData (GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {

			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

			if (pcmd->UserCallback) {
				pcmd->UserCallback (cmd_list, pcmd);
			}

			if (!pcmd->UserCallback) {
				ImVec4 clip_rect = ImVec4 (pcmd->ClipRect.x - pos.x, pcmd->ClipRect.y - pos.y, pcmd->ClipRect.z - pos.x, pcmd->ClipRect.w - pos.y);

				if (clip_rect.x < settings.framebuffer.width && clip_rect.y < settings.framebuffer.height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {

                    // Apply scissor/clipping rectangle
					GL::Scissor((int)clip_rect.x, (int)(settings.framebuffer.height - clip_rect.w), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));

					/*
					 * Draw
					*/

					GL::BindTexture (GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
					GL::DrawElements (GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
				}
	        }
	        idx_buffer_offset += pcmd->ElemCount;
	    }
	}

	GL::Disable (GL_SCISSOR_TEST);
}

void GUIRenderPass::EndEditorPass ()
{
	/*
	 * Unlock current locked shader for further rendering
	*/

	Pipeline::UnlockShader ();
}

std::vector<PipelineAttribute> GUIRenderPass::GetCustomAttributes ()
{
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute projectionMatrix;
	PipelineAttribute font;

	projectionMatrix.type = PipelineAttribute::AttrType::ATTR_MATRIX_4X4F;
	font.type = PipelineAttribute::AttrType::ATTR_1I;

	projectionMatrix.name = "projectionMatrix";
	font.name = "font";

	auto draw_data = ImGui::GetDrawData ();
	float L = draw_data->DisplayPos.x;
	float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
	float T = draw_data->DisplayPos.y;
	float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

	glm::mat4 projectionMat = glm::ortho (L, R, B, T);

	projectionMatrix.matrix = projectionMat;
	font.value.x = 0;

	attributes.push_back (projectionMatrix);
	attributes.push_back (font);

	return attributes;
}

void GUIRenderPass::LockShader ()
{
	/*
	 * Lock the shader for not animated normal mapped objects
	*/

	Pipeline::LockShader (_shaderView);
}

void GUIRenderPass::Clear ()
{
	/*
	 * Delete buffers
	*/

	GL::DeleteBuffers (1, &_VBO_ID);
	GL::DeleteBuffers (1, &_IBO_ID);

	/*
	 * Delete VAO
	*/

	GL::DeleteVertexArrays (1, &_VAO_ID);
}
