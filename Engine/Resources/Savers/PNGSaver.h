#ifndef PNGSAVER_H
#define PNGSAVER_H

#include "Resources/ResourceSaver.h"

class PNGSaver : public ResourceSaver
{
public:
	bool Save (const Object* object, const std::string& filename);
};

#endif