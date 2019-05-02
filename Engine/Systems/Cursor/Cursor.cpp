#include "Cursor.h"

#include <cstddef>

SDL_Cursor* Cursor::_cursors [CursorType::CURSOR_TYPE_COUNT];

void Cursor::Init ()
{
	for (std::size_t index = 0; index < CursorType::CURSOR_TYPE_COUNT; index ++) {
		_cursors [index] = SDL_CreateSystemCursor ((SDL_SystemCursor) index);
	}
}

void Cursor::Hide ()
{
	SDL_ShowCursor (SDL_DISABLE);
}

void Cursor::Show ()
{
	SDL_ShowCursor (SDL_ENABLE);
}

bool Cursor::IsShown ()
{
	auto isShown = SDL_ShowCursor (SDL_QUERY);

	return isShown == SDL_ENABLE;
}

void Cursor::SetCursor (CursorType type)
{
	SDL_SetCursor (_cursors [(int) type]);
}

void Cursor::Clear ()
{
	for (std::size_t index = 0; index < CursorType::CURSOR_TYPE_COUNT; index ++) {
		SDL_FreeCursor (_cursors [index]);
	}
}
