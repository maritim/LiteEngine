#include "BillboardParticle.h"

#include "Particle.h"

#include "Texture/TextureAtlas.h"

#include "Renderer/RenderSystem.h"

BillboardParticle::BillboardParticle () :
	Particle (),
	_textureAtlas (nullptr),
	_textureAtlasView (nullptr),
	_shaderName ("BILLBOARD")
{
	// ShaderManager::Instance ()->AddShader (_shaderName,
	// 	"Assets/Shaders/Particles/billboardVertex.glsl",
	// 	"Assets/Shaders/Particles/billboardFragment.glsl");
}

void BillboardParticle::SetTextureAtlas (const Resource<Texture>& textureAtlas)
{
	_textureAtlas = textureAtlas;

	_textureAtlasView = RenderSystem::LoadTexture (textureAtlas);
}

std::vector<BufferAttribute> BillboardParticle::GetBufferAttributes () const
{
	std::vector<BufferAttribute> attributes;

	for (int i=0;i<6;i++) {
		BufferAttribute attribute;

		attribute.index = 3 + i;
		attribute.size = 4;
		attribute.type = BufferAttribute::AttrType::ATTR_F;
		attribute.stride = 24 * sizeof (float);
		attribute.pointer = i * 4 * sizeof (float);

		attributes.push_back (attribute);
	}

	return attributes;
}

unsigned char* BillboardParticle::GetBuffer ()
{
	std::size_t instanceIndex = 0;

	glm::vec3 position = _transform->GetPosition ();
	glm::quat rotationq = _transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 rotation = glm::mat4_cast(rotationq);

	glm::mat4 modelMatrix = translate * rotation;

	for (short int i=0;i<4;i++) {
		for (short int j=0;j<4;j++) {
			_instanceBuffer [instanceIndex ++] = modelMatrix [i][j];
		}
	}

	const TextureAtlas* texAtlas = dynamic_cast <const TextureAtlas*> (&*_textureAtlas);

	float lifeFactor = 1.0f * _timeAlive / _lifetime;

	std::size_t areasCount = texAtlas->GetAreasCount ();
	std::size_t areaIndex = (std::size_t) (lifeFactor * (float) areasCount);
	std::size_t nextAreaIndex = areaIndex + (areaIndex + 1 < areasCount);
	float texBlending = lifeFactor * areasCount - areaIndex;

	glm::vec2 currTexOffset = texAtlas->GetOffset (areaIndex);
	glm::vec2 nextTexOffset = texAtlas->GetOffset (nextAreaIndex);

	_instanceBuffer [instanceIndex ++] = currTexOffset.x;
	_instanceBuffer [instanceIndex ++] = currTexOffset.y;
	_instanceBuffer [instanceIndex ++] = nextTexOffset.x;
	_instanceBuffer [instanceIndex ++] = nextTexOffset.y;
	_instanceBuffer [instanceIndex ++] = texBlending;

	glm::vec3 scale = _transform->GetScale ();

	_instanceBuffer [instanceIndex ++] = scale.x;
	_instanceBuffer [instanceIndex ++] = scale.y;
	_instanceBuffer [instanceIndex ++] = scale.z;

	return (unsigned char*) _instanceBuffer;
}

std::size_t BillboardParticle::GetSize () const
{
	return sizeof (float) * 24;
}

std::string BillboardParticle::GetShaderName () const
{
	return _shaderName;
}

std::vector<PipelineAttribute> BillboardParticle::GetAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	// Take area scale from the atlas
	const TextureAtlas* texAtlas = dynamic_cast <const TextureAtlas*> (&*_textureAtlas);

	PipelineAttribute textureAtlas;
	PipelineAttribute uniformAtlasArea;

	textureAtlas.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;
	uniformAtlasArea.type = PipelineAttribute::AttrType::ATTR_1F;

	textureAtlas.name = "textureAtlas";
	uniformAtlasArea.name = "atlasAreaScale";

	textureAtlas.value.x = _textureAtlasView->GetGPUIndex ();
	uniformAtlasArea.value = glm::vec3 (texAtlas->GetSize (0), 0.0f);

	attributes.push_back (textureAtlas);
	attributes.push_back (uniformAtlasArea);

	return attributes;
}

Particle* BillboardParticle::Clone ()
{
	BillboardParticle* clone = new BillboardParticle ();

	clone->SetMesh (_mesh);

	return clone;
}