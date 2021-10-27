#include "GUI.h"

#include <vector>
#include <ImGui/imgui.h>

#include "Systems/Window/Window.h"
#include "Systems/Input/Input.h"
#include "Systems/Cursor/Cursor.h"
#include "Systems/Time/Time.h"
#include "Systems/GUI/Gizmo/Gizmo.h"

#include "Wrappers/OpenGL/GL.h"

#define SCANCODE_REMOVER 0xFF
#define REMOVE_SCANCODE(X) (X & SCANCODE_REMOVER)

void GUI::Init ()
{
	// IMGUI_CHECKVERSION();
	ImGui::CreateContext ();

	ImGui::StyleColorsDark ();

	ImGuiIO& io = ImGui::GetIO ();

	io.KeyMap[ImGuiKey_Tab] = REMOVE_SCANCODE(InputKey::TAB);
	io.KeyMap[ImGuiKey_LeftArrow] = REMOVE_SCANCODE(InputKey::LEFT);
	io.KeyMap[ImGuiKey_RightArrow] = REMOVE_SCANCODE(InputKey::RIGHT);
	io.KeyMap[ImGuiKey_UpArrow] = REMOVE_SCANCODE(InputKey::UP);
	io.KeyMap[ImGuiKey_DownArrow] = REMOVE_SCANCODE(InputKey::DOWN);
	io.KeyMap[ImGuiKey_PageUp] = REMOVE_SCANCODE(InputKey::PAGEUP);
	io.KeyMap[ImGuiKey_PageDown] = REMOVE_SCANCODE(InputKey::PAGEDOWN);
	io.KeyMap[ImGuiKey_Home] = REMOVE_SCANCODE(InputKey::HOME);
	io.KeyMap[ImGuiKey_End] = REMOVE_SCANCODE(InputKey::END);
	io.KeyMap[ImGuiKey_Insert] = REMOVE_SCANCODE(InputKey::INSERT);
	io.KeyMap[ImGuiKey_Delete] = REMOVE_SCANCODE(InputKey::DELETE);
	io.KeyMap[ImGuiKey_Backspace] = REMOVE_SCANCODE(InputKey::BACKSPACE);
	io.KeyMap[ImGuiKey_Space] = REMOVE_SCANCODE(InputKey::SPACE);
	io.KeyMap[ImGuiKey_Enter] = REMOVE_SCANCODE(InputKey::RETURN);
	io.KeyMap[ImGuiKey_Escape] = REMOVE_SCANCODE(InputKey::ESCAPE);
	io.KeyMap[ImGuiKey_A] = InputKey::A;
	io.KeyMap[ImGuiKey_C] = InputKey::C;
	io.KeyMap[ImGuiKey_V] = InputKey::V;
	io.KeyMap[ImGuiKey_X] = InputKey::X;
	io.KeyMap[ImGuiKey_Y] = InputKey::Y;
	io.KeyMap[ImGuiKey_Z] = InputKey::Z;

	/*
	 * Load font
	*/

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	unsigned int fontTextureID;
	GL::GenTextures (1, &fontTextureID);
	GL::BindTexture (GL_TEXTURE_2D, fontTextureID);

	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL::TexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL::PixelStorei (GL_UNPACK_ROW_LENGTH, 0);

	GL::TexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	io.Fonts->TexID = (ImTextureID)(intptr_t)fontTextureID;

	/*
	 * Enable mouse cursors
	*/

	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;

	/*
	 * Enable docking
	*/

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void GUI::Update ()
{
	ImGuiIO& io = ImGui::GetIO();

	/*
	 * Set mouse state
	*/

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_LEFT)) {
		io.MouseDown [0] = true;
	}

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_RIGHT)) {
		io.MouseDown [1] = true;
	}

	if (Input::GetMouseButtonDown (MOUSE_BUTTON_MIDDLE)) {
		io.MouseDown [2] = true;
	}

	if (Input::GetMouseButtonUp (MOUSE_BUTTON_LEFT)) {
		io.MouseDown [0] = false;
	}

	if (Input::GetMouseButtonUp (MOUSE_BUTTON_RIGHT)) {
		io.MouseDown [1] = false;
	}

	if (Input::GetMouseButtonUp (MOUSE_BUTTON_MIDDLE)) {
		io.MouseDown [2] = false;
	}

	/*
	 * Set mouse wheel state
	*/

	glm::ivec2 mouseWheel = Input::GetMouseWheel ();

	if (mouseWheel.x > 0) io.MouseWheelH += 1;
	if (mouseWheel.x < 0) io.MouseWheelH -= 1;
	if (mouseWheel.y > 0) io.MouseWheel += 1;
	if (mouseWheel.y < 0) io.MouseWheel -= 1;

	/*
	 * Set mouse position
	*/

	glm::ivec2 mousePosition = Input::GetMousePosition ();

	io.MousePos = ImVec2 ((float) mousePosition.x, (float) mousePosition.y);

	/*
	 * Set control keys
	*/

	std::vector<InputKey> controlKeys {
		InputKey::TAB, InputKey::LEFT, InputKey::RIGHT, InputKey::UP, InputKey::DOWN,
		InputKey::PAGEUP, InputKey::PAGEDOWN, InputKey::HOME, InputKey::END, InputKey::INSERT, 
		InputKey::DELETE, InputKey::BACKSPACE, InputKey::SPACE, InputKey::RETURN, InputKey::ESCAPE,
		InputKey::A, InputKey::C, InputKey::V, InputKey::X, InputKey::Y, InputKey::Z
	};

	for (auto controlKey : controlKeys) {
		if (Input::GetKeyUp (controlKey)) {
			io.KeysDown [(std::size_t) REMOVE_SCANCODE(controlKey)] = false;
		}
		if (Input::GetKeyDown (controlKey)) {
			io.KeysDown [(std::size_t) REMOVE_SCANCODE(controlKey)] = true;
		}
	}

	io.KeyShift = Input::GetKey (InputKey::LSHIFT) || Input::GetKey (InputKey::RSHIFT);
	io.KeyCtrl = Input::GetKey (InputKey::LCTRL) || Input::GetKey (InputKey::RCTRL);
	io.KeyAlt = Input::GetKey (InputKey::LALT) || Input::GetKey (InputKey::RALT);
	io.KeySuper = Input::GetKey (InputKey::LGUI) || Input::GetKey (InputKey::RGUI);

	io.AddInputCharactersUTF8 (Input::GetIMETextComposition ().c_str ());

	/*
	 * Set delta time
	*/

	io.DeltaTime = Time::GetDeltaTime ();

	/*
	 * Set cursor
	*/

	std::vector<CursorType> cursors {
		CursorType::CURSOR_TYPE_ARROW, CursorType::CURSOR_TYPE_IBEAM,
		CursorType::CURSOR_TYPE_SIZEALL, CursorType::CURSOR_TYPE_SIZENS,
		CursorType::CURSOR_TYPE_SIZEWE, CursorType::CURSOR_TYPE_SIZENESW,
		CursorType::CURSOR_TYPE_SIZENWSE
	};

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor ();

	Cursor::SetCursor (cursors [imgui_cursor]);

	/*
	 * Set window size
	*/

	std::size_t width = Window::GetWidth ();
	std::size_t height = Window::GetHeight ();

	io.DisplaySize = ImVec2 ((float)width, (float)height);

	/*
	 * Start ImGui frame
	*/

	ImGui::NewFrame ();

	/*
	 * Start Gizmos frame
	*/

	Gizmo::NewFrame ();
}

void GUI::Clear ()
{
	ImGuiIO& io = ImGui::GetIO();

	unsigned int fontTextureID = (intptr_t) io.Fonts->TexID;
	GL::DeleteTextures (1, &fontTextureID);

	io.Fonts->TexID = 0;

	ImGui::DestroyContext ();
}