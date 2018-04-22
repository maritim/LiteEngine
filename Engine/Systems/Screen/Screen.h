#ifndef SCREEN_H
#define SCREEN_H

/*
 * Did I really need this?
*/

#include "Core/Singleton/Singleton.h"

#include <glm/glm.hpp>
#include <cstddef>

class Screen : public Singleton<Screen>
{
	friend class Singleton<Screen>;

public:
	std::size_t GetWidth ();
	std::size_t GetHeight ();
	glm::uvec2 GetSize ();

	void SetWidth (std::size_t x);
	void SetHeight (std::size_t y);
private:
	Screen ();
	Screen (const Screen& other);
	Screen& operator=(const Screen& other);
	~Screen ();
};

#endif