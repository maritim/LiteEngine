#ifndef GIZMO_H
#define GIZMO_H

#include <vector>

#include "GizmoLine.h"

#include "Utils/Color/Color.h"

class Gizmo
{
private:
	static std::vector<GizmoLine> _lines;

public:
	static void NewFrame ();

	static void DrawLine (const glm::vec3& first, const glm::vec3& last, const Color& color, bool depthTest = false);

	static const std::vector<GizmoLine>& GetLines ();
};

#endif