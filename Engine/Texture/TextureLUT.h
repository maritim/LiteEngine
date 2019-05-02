#ifndef TEXTURELUT_H
#define TEXTURELUT_H

#include "Texture.h"

class TextureLUT : public Texture
{
protected:
	std::size_t _depth;

public:
	TextureLUT (const std::string& name);

	void SetDepth (std::size_t depth);

	std::size_t GetDepth () const;
};

#endif