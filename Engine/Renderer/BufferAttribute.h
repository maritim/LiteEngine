#ifndef BUFFERATTRIBUTE_H
#define BUFFERATTRIBUTE_H

#include "GL/glew.h"

struct BufferAttribute
{
	std::size_t index;
	std::size_t size;
	GLenum type;
	std::size_t stride;
	std::size_t pointer;
};

#endif