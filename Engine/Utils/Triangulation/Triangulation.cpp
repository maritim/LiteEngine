#include "Triangulation.h"

#include "Mesh/ObjectModel.h"

void Triangulation::ConvexTriangulation (Model* model) 
{
	for_each_type (ObjectModel*, objModel, *model) {
		std::vector<PolygonGroup*> polyGroups;

		for (PolygonGroup* polyGroup : *objModel) {
			PolygonGroup* newPolyGroup = ConvexTriangulation (polyGroup);

			polyGroups.push_back (newPolyGroup);
		}

		objModel->Clear ();

		for (PolygonGroup* polyGroup : polyGroups) {
			objModel->AddPolygonGroup (polyGroup);
		}
	}
}

std::vector<Polygon*> Triangulation::ConvexTriangulation (Polygon* polygon)
{
	std::vector<Polygon*> result;

	for (std::size_t i=2;i<polygon->VertexCount ();i++) {
		Polygon* newPoly = new Polygon ();

		newPoly->AddVertex (polygon->GetVertex (0));
		newPoly->AddVertex (polygon->GetVertex (i-1));
		newPoly->AddVertex (polygon->GetVertex (i));

		if (polygon->HaveNormals ()) {
			newPoly->AddNormal (polygon->GetNormal (0));
			newPoly->AddNormal (polygon->GetNormal (i-1));
			newPoly->AddNormal (polygon->GetNormal (i));
		}

		if (polygon->HaveUV ()) {
			newPoly->AddTexcoord (polygon->GetTexcoord (0));
			newPoly->AddTexcoord (polygon->GetTexcoord (i-1));
			newPoly->AddTexcoord (polygon->GetTexcoord (i));
		}

		result.push_back (newPoly);
	}

	return result;
}

PolygonGroup* Triangulation::ConvexTriangulation (PolygonGroup* polyGroup)
{
	PolygonGroup* resultPolyGroup = new PolygonGroup (polyGroup->GetName ());
	resultPolyGroup->SetMaterialName (polyGroup->GetMaterialName ());

	for (Polygon* poly : *polyGroup) {
		std::vector<Polygon*> newPolygons = ConvexTriangulation (poly);

		for (Polygon* newPoly : newPolygons) {
			resultPolyGroup->AddPolygon (newPoly);
		}
	}

	return resultPolyGroup;
}