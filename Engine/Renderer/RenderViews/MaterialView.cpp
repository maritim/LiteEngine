#include "MaterialView.h"

MaterialView::MaterialView () :
	name(""),
	ambientColor(0.2, 0.2, 0.2),
	diffuseColor(0.8, 0.8, 0.8),
	specularColor (1.0, 1.0, 1.0),
	shininess(32),
	transparency (0.0),
	refractiveIndex (1.0),
	illum(0),
	blending (770, 771),
	ambientTexture(nullptr),
	diffuseTexture (nullptr),
	specularTexture (nullptr),
	specularHighlight(nullptr),
	alphaTexture (nullptr),
	bumpTexture (nullptr),
	cubeTexture (nullptr),
	shaderView (nullptr)
{

}
