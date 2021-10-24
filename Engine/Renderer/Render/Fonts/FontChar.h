#ifndef FONTCHAR_H
#define FONTCHAR_H

#include "Core/Interfaces/Object.h"

#include <glm/glm.hpp>

class FontChar : public Object
{
protected:
	// id	The character id.
	unsigned char _id;
	// x	The left position of the character image in the texture.
	// y	The top position of the character image in the texture.
	glm::uvec2 _position;
	// width	The width of the character image in the texture.
	// height	The height of the character image in the texture.
	glm::uvec2 _size;
	// xoffset	How much the current position should be offset when copying the image from the texture to the screen.
	// yoffset	How much the current position should be offset when copying the image from the texture to the screen.
	glm::ivec2 _offset; 
	// xadvance	How much the current position should be advanced after drawing the character.
	short int _xAdvance;

public:
	FontChar ();
	virtual ~FontChar ();

	unsigned char GetId () const;
	glm::uvec2 GetPosition () const;
	glm::uvec2 GetSize () const;
	glm::ivec2 GetOffset () const;
	short int GetXAdvance () const;

	void SetId (unsigned char id);
	void SetPosition (const glm::uvec2& position);
	void SetSize (const glm::uvec2& size);
	void SetOffset (const glm::ivec2& offset);
	void SetXAdvance (short int xAdvance);
};

#endif