#ifndef PARTICLESYSTEMLOADER_H
#define PARTICLESYSTEMLOADER_H

#include "ResourceLoader.h"

#include <string>

#include "Core/Parsers/XML/TinyXml/tinyxml.h"

#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "VisualEffects/ParticleSystem/Emiter.h"
#include "VisualEffects/ParticleSystem/Particle.h"

class ParticleSystemLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	void ProcessPartCount (TiXmlElement* xmlElem, ParticleSystem* sys);
	void ProcessEmissionRate (TiXmlElement* xmlElem, ParticleSystem* sys);
	void ProcessEmiter (TiXmlElement* xmlElem, ParticleSystem* sys, const std::string& filename);
	void ProcessRenderer (TiXmlElement* xmlElem, ParticleSystem* sys);

	Emiter* CreateEmiter (TiXmlElement* xmlElem, const std::string& filename);
	void ProcessParticle (TiXmlElement* xmlElem, Emiter* emiter, const std::string& filename);
	void ProcessParticleMesh (TiXmlElement* xmlElem, Particle* particle, const std::string& filename);
	void ProcessMeshMaterial (TiXmlElement* xmlElem, Resource<Model>& mesh, const std::string& filename);
	void ProcessParticleRigidbody (TiXmlElement* xmlElem, Particle* particle);
	void ProcessParticleGravity (TiXmlElement* xmlElem, Rigidbody* rigidbody);
	void ProcessTransform (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessEmisShape (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessScaleCurve (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessTweenCurve (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessLifetimeRange (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessScaleRange (TiXmlElement* xmlElem, Emiter* emiter);
	void ProcessSpeedRange (TiXmlElement* xmlElem, Emiter* emiter);

	void ProcessDepthMask (TiXmlElement* xmlElem, ParticleSystem* sys);

	glm::vec3 GetVector (TiXmlElement* xmlElem);
	glm::quat GetQuaternion (TiXmlElement* xmlElem);
};

#endif