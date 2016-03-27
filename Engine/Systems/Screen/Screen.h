#ifndef SCREEN_H
#define SCREEN_H

#include "Core/Singleton/Singleton.h"

#include <cstddef>

#include "Core/Math/glm/glm.hpp"

class Screen : public Singleton<Screen>
{
	friend class Singleton<Screen>;

private:
	std::size_t _screenWidth;
	std::size_t _screenHeight;

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