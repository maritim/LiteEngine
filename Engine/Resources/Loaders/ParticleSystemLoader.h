#ifndef PARTICLESYSTEMLOADER_H
#define PARTICLESYSTEMLOADER_H

#include <string>
#include <tinyxml2.h>

#include "Resources/ResourceLoader.h"

#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "VisualEffects/ParticleSystem/Emiter.h"
#include "VisualEffects/ParticleSystem/Particle.h"

class ParticleSystemLoader : public ResourceLoader
{
public:
	Object* Load (const std::string& filename);
private:
	void ProcessPartCount (tinyxml2::XMLElement* xmlElem, ParticleSystem* sys);
	void ProcessEmissionRate (tinyxml2::XMLElement* xmlElem, ParticleSystem* sys);
	void ProcessEmiter (tinyxml2::XMLElement* xmlElem, ParticleSystem* sys, const std::string& filename);
	void ProcessRenderer (tinyxml2::XMLElement* xmlElem, ParticleSystem* sys);

	Emiter* CreateEmiter (tinyxml2::XMLElement* xmlElem, const std::string& filename);
	void ProcessParticle (tinyxml2::XMLElement* xmlElem, Emiter* emiter, const std::string& filename);
	void ProcessParticleMesh (tinyxml2::XMLElement* xmlElem, Particle* particle, const std::string& filename);
	void ProcessMeshMaterial (tinyxml2::XMLElement* xmlElem, Resource<Model>& mesh, const std::string& filename);
	void ProcessTextureAtlas (tinyxml2::XMLElement* xmlElem, Particle* particle, const std::string& filename);
	void ProcessParticleRigidbody (tinyxml2::XMLElement* xmlElem, Particle* particle);
	// void ProcessParticleGravity (tinyxml2::XMLElement* xmlElem, Rigidbody* rigidbody);
	void ProcessTransform (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessEmisShape (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessScaleCurve (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessTweenCurve (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessLifetimeRange (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessScaleRange (tinyxml2::XMLElement* xmlElem, Emiter* emiter);
	void ProcessSpeedRange (tinyxml2::XMLElement* xmlElem, Emiter* emiter);

	void ProcessDepthMask (tinyxml2::XMLElement* xmlElem, ParticleSystem* sys);

	glm::vec3 GetVector (tinyxml2::XMLElement* xmlElem);
	glm::quat GetQuaternion (tinyxml2::XMLElement* xmlElem);
};

#endif