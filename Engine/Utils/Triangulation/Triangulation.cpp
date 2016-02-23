#include "Triangulation.h"

#include "Mesh/ObjectModel.h"

void Triangulation::ConvexTriangulation (Model* model) 
{
	for (std::size_t i=0;i<model->ObjectsCount ();i++) {
		ObjectModel* objModel = model->GetObject (i);

		std::vector<PolygonGroup*> polyGroups;

		for (std::size_t j=0;j<objModel->GetPolygonCount ();j++) {
			PolygonGroup* polyGroup = objModel->GetPolygonGroup (j);

			PolygonGroup* newPolyGroup = ConvexTriangulation (polyGroup);

			polyGroups.push_back (newPolyGroup);
		}

		objModel->Clear ();

		for (std::size_t j=0;j<polyGroups.size ();j++) {
			objModel->AddPolygonGroup (polyGroups [j]);
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

	for (std::size_t i=0;i<polyGroup->GetPolygonCount ();i++) {
		Polygon* poly = polyGroup->GetPolygon (i);

		std::vector<Polygon*> newPoly = ConvexTriangulation (poly);

		for (std::size_t j=0;j<newPoly.size ();j++) {
			resultPolyGroup->AddPolygon (newPoly [j]);
		}
	}

	return resultPolyGroup;
}