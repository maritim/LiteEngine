#include "BulletDebugDraw.h"

#include "Systems/GUI/Gizmo/Gizmo.h"

void BulletDebugDraw::drawLine (const btVector3& first,const btVector3& last, const btVector3& color)
{
	Gizmo::DrawLine (
		glm::vec3 (first.x (), first.y (), first.z ()),
		glm::vec3 (last.x (), last.y (), last.z ()),
		Color (color.x () * 255, color.y () * 255, color.z () * 255)
	);
}

int BulletDebugDraw::getDebugMode () const
{
	return btIDebugDraw::DBG_DrawWireframe;
}
