#include "Gizmo.h"

std::vector<GizmoLine> Gizmo::_lines;

void Gizmo::NewFrame ()
{
	_lines.clear ();
}

void Gizmo::DrawLine (const glm::vec3& first, const glm::vec3& last, const Color& color, bool depthTest)
{
	GizmoLine line;

	line.first = first;
	line.last = last;
	line.color = color.ToVector3 ();
	line.depthTest = depthTest;

	_lines.push_back (line);
}

const std::vector<GizmoLine>& Gizmo::GetLines ()
{
	return _lines;
}
