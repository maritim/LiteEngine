#ifndef FONT_H
#define FONT_H

#include "Core/Interfaces/Object.h"

#include <string>

#include "Core/Resources/Resource.h"
#include "Texture/Texture.h"

#include "FontChar.h"

class Font : public Object
{
protected:
	std::string _name;

public:
	Font ();
	virtual ~Font ();

	const std::string& GetName () const;
	void SetName (const std::string& name);

	virtual FontChar* GetChar (unsigned char id) const = 0;
	virtual Resource<Texture> GetTexture (std::size_t id) const = 0;
	virtual std::size_t GetLineHeight () = 0;
};

#endif