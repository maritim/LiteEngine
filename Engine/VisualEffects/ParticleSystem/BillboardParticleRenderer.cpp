#include "BillboardParticleRenderer.h"

#include <glm/glm.hpp>
#include <vector>

#include "SceneNodes/Model3DRenderer.h"

#include "Mesh/Model.h"
#include "Texture/TextureAtlas.h"
#include "Texture/Texture.h"
#include "Material/Material.h"

#include "Core/Math/Matrix.h"

#include "Managers/TextureManager.h"
#include "Managers/MaterialManager.h"

void BillboardParticleRenderer::SetLifetime (unsigned int* lifetime)
{
	_lifetime = lifetime;
}

void BillboardParticleRenderer::SetTimeAlive (unsigned int* timeAlive)
{
	_timeAlive = timeAlive;
}

Buffer<float> BillboardParticleRenderer::GetParticleData ()
{
	Buffer<float> buffer;

	Material* mat = MaterialManager::Instance ()->GetMaterial (_matName);

	buffer.Add (this->GetModelMatrix ());
	ManageAtlasTexcoord (mat, &buffer);
	ManageCustomAttributes (&buffer);

	return buffer;	
}

std::vector<BufferAttribute> BillboardParticleRenderer::GetBufferAttributes ()
{
	std::vector<BufferAttribute> attributes;

	for (int i=0;i<6;i++) {
		BufferAttribute attribute;

		attribute.index = 3 + i;
		attribute.size = 4;
		attribute.type = GL_FLOAT;
		attribute.stride = 24 * sizeof (float);
		attribute.pointer = i * 4 * sizeof (float);

		attributes.push_back (attribute);
	}

	return attributes;
}

std::vector<PipelineAttribute> BillboardParticleRenderer::GetUniformAttributes ()
{
	Material* mat = MaterialManager::Instance ()->GetMaterial (_matName);

	// Take area scale from the atlas
	Attribute atlasMap = mat->GetAttribute (Attribute::AttrType::ATTR_TEXTURE2D_ATLAS);
	Texture* tex = TextureManager::Instance ()->GetTexture (atlasMap.valueName);

	TextureAtlas* texAtlas = dynamic_cast <TextureAtlas*> (tex);

	// Create attribute
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute uniformAtlasArea;

	uniformAtlasArea.type = PipelineAttribute::AttrType::ATTR_1F;

	uniformAtlasArea.name = "atlasAreaScale";

	uniformAtlasArea.value = texAtlas->GetSize (0);

	attributes.push_back (uniformAtlasArea);

	return attributes;
}

void BillboardParticleRenderer::Attach (Model* model)
{
	for_each_type (ObjectModel*, objModel, *model) {
		for (PolygonGroup* polyGroup : *objModel) {
			_matName = polyGroup->GetMaterialName ();
		}
	}
}

void BillboardParticleRenderer::ManageAtlasTexcoord (Material* mat, Buffer<float>* buffer)
{
	Attribute atlasMap = mat->GetAttribute (Attribute::AttrType::ATTR_TEXTURE2D_ATLAS);
	Texture* tex = TextureManager::Instance ()->GetTexture (atlasMap.valueName);

	TextureAtlas* texAtlas = dynamic_cast <TextureAtlas*> (tex);

	float lifeFactor = 1.0f * (*_timeAlive) / (*_lifetime);

	std::size_t areasCount = texAtlas->GetAreasCount ();
	std::size_t areaIndex = (std::size_t) (lifeFactor * (float) areasCount);
	std::size_t nextAreaIndex = areaIndex + (areaIndex + 1 < areasCount);
	float texBlending = lifeFactor * areasCount - areaIndex;

	glm::vec3 currTexOffset = texAtlas->GetOffset (areaIndex);
	glm::vec3 nextTexOffset = texAtlas->GetOffset (nextAreaIndex);

	buffer->Add (currTexOffset.x); 
	buffer->Add (currTexOffset.y);
	buffer->Add (nextTexOffset.x);
	buffer->Add (nextTexOffset.y);
	buffer->Add (texBlending);
}

void BillboardParticleRenderer::ManageCustomAttributes (Buffer<float>* buffer)
{
	glm::vec3 scale = _transform->GetScale ();

	buffer->Add (scale.x);
	buffer->Add (scale.y);
	buffer->Add (scale.z);
}