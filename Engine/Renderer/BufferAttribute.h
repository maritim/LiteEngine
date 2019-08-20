#ifndef BUFFERATTRIBUTE_H
#define BUFFERATTRIBUTE_H

struct BufferAttribute
{
	enum AttrType { 
		ATTR_F,
	};

	std::size_t index;
	std::size_t size;
	AttrType type;
	std::size_t stride;
	std::size_t pointer;
};

#endif