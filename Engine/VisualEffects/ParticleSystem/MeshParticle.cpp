#include "MeshParticle.h"

#include "Particle.h"

MeshParticle::MeshParticle () :
	Particle ()
{

}

std::vector<BufferAttribute> MeshParticle::GetBufferAttributes () const
{
	std::vector<BufferAttribute> attributes;

	for (int i=0;i<4;i++) {
		BufferAttribute attribute;

		attribute.index = 3 + i;
		attribute.size = 4;
		attribute.type = BufferAttribute::AttrType::ATTR_F;
		attribute.stride = 16 * sizeof (float);
		attribute.pointer = i * 4 * sizeof (float);

		attributes.push_back (attribute);
	}

	return attributes;
}

unsigned char* MeshParticle::GetBuffer ()
{
	glm::vec3 position = _transform->GetPosition ();
	glm::vec3 scalev = _transform->GetScale ();
	glm::quat rotationq = _transform->GetRotation ();

	glm::mat4 translate = glm::translate (glm::mat4 (1.f), glm::vec3 (position.x, position.y, position.z));
	glm::mat4 scale = glm::scale (glm::mat4 (1.f), glm::vec3 (scalev.x, scalev.y, scalev.z));
	glm::mat4 rotation = glm::mat4_cast(rotationq);

	glm::mat4 modelMatrix = translate * scale * rotation;

	for (short int i=0;i<4;i++) {
		for (short int j=0;j<4;j++) {
			_instanceBuffer [i * 4 + j * 4] = modelMatrix [i][j];
		}
	}

	return (unsigned char*) _instanceBuffer;
}

std::size_t MeshParticle::GetSize () const
{
	return sizeof (float) * 16;
}

std::vector<PipelineAttribute> MeshParticle::GetAttributes () const
{
	std::vector<PipelineAttribute> attributes;

	return attributes;
}

Particle* MeshParticle::Clone ()
{
	MeshParticle* clone = new MeshParticle ();

	return clone;
}