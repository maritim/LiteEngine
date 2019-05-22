#include "ParticleSystemLoader.h"

#include <string>

#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "VisualEffects/ParticleSystem/Emiter.h"
#include "VisualEffects/ParticleSystem/Particle.h"

#include "Mesh/Model.h"
#include "Managers/MaterialManager.h"

#include "VisualEffects/ParticleSystem/PointEmiter.h"
#include "VisualEffects/ParticleSystem/QuadEmiter.h"
#include "VisualEffects/ParticleSystem/CircleEmiter.h"
#include "VisualEffects/ParticleSystem/SphereEmiter.h"
#include "VisualEffects/ParticleSystem/CubeEmiter.h"
#include "VisualEffects/ParticleSystem/MeshEmiter.h"

#include "VisualEffects/ParticleSystem/BillboardParticle.h"
#include "VisualEffects/ParticleSystem/MeshParticle.h"

#include "Utils/Curves/AnimationCurve.h"
#include "Utils/Curves/EaseCurve.h"
#include "Utils/Primitives/Primitive.h"
#include "Utils/Files/FileSystem.h"
#include "Utils/Extensions/StringExtend.h"

#include "Resources.h"

Object* ParticleSystemLoader::Load (const std::string& filename)
{
	TiXmlDocument doc;
	if(!doc.LoadFile(filename.c_str ())) {
		return NULL;
	}

	TiXmlElement* root = doc.FirstChildElement ("ParticleSystem");

	if (root == NULL) {
		return NULL;
	}

	ParticleSystem* partSys = new ParticleSystem ();

	TiXmlElement* content = root->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "CountRange") {
			ProcessPartCount (content, partSys);
		}
		else if (name == "EmissionRate") {
			ProcessEmissionRate (content, partSys);
		}
		else if (name == "Emiter") {
			ProcessEmiter (content, partSys, filename);
		}
		else if (name == "Renderer") {
			ProcessRenderer (content, partSys);
		}

		content = content->NextSiblingElement ();
	}

	doc.Clear ();

	return partSys;
}

void ParticleSystemLoader::ProcessPartCount (TiXmlElement* xmlElem, ParticleSystem* partSys)
{
	std::size_t minPart = std::stoi (xmlElem->Attribute ("min"));
	std::size_t maxPart = std::stoi (xmlElem->Attribute ("max"));

	partSys->SetMinimPartCount (minPart);
	partSys->SetMaximPartCount (maxPart);
}

void ParticleSystemLoader::ProcessEmissionRate (TiXmlElement* xmlElem, ParticleSystem* partSys)
{
	std::size_t rate = std::stoi (xmlElem->Attribute ("rate"));

	partSys->SetEmissionRate (rate);
}

void ParticleSystemLoader::ProcessEmiter (TiXmlElement* xmlElem, ParticleSystem* partSys, 
	const std::string& filename)
{
	Emiter* emiter = CreateEmiter (xmlElem, filename);

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Particle") {
			ProcessParticle (content, emiter, filename);
		}
		else if (name == "Transform") {
			ProcessTransform (content, emiter);
		}
		else if (name == "EmissionShape") {
			ProcessEmisShape (content, emiter);
		}
		else if (name == "ScaleCurve") {
			ProcessScaleCurve (content, emiter);
		}
		else if (name == "SpeedCurve") {
			ProcessTweenCurve (content, emiter);
		}
		else if (name == "LifetimeRange") {
			ProcessLifetimeRange (content, emiter);
		}
		else if (name == "SpeedRange") {
			ProcessSpeedRange (content, emiter);
		}
		else if (name == "ScaleRange") {
			ProcessScaleRange (content, emiter);
		}

		content = content->NextSiblingElement ();
	}

	partSys->SetEmiter (emiter);
}

void ParticleSystemLoader::ProcessRenderer (TiXmlElement* xmlElem, ParticleSystem* partSys)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "DepthMask") {
			ProcessDepthMask (content, partSys);
		}

		content = content->NextSiblingElement ();
	}
}

Emiter* ParticleSystemLoader::CreateEmiter (TiXmlElement* xmlElem, const std::string& filename)
{
	Emiter* emiter = NULL;

	std::string type = xmlElem->Attribute ("type");

	if (type == "POINT") {
		emiter = new PointEmiter ();
	}
	else if (type == "QUAD") {
		float width = std::stof (xmlElem->Attribute ("width"));
		float depth = std::stof (xmlElem->Attribute ("depth"));

		emiter = new QuadEmiter (width, depth);
	}
	else if (type == "CIRCLE") {
		float radius = std::stof (xmlElem->Attribute ("radius"));

		emiter = new CircleEmiter (radius);
	}
	else if (type == "SPHERE") {
		float radius = std::stof (xmlElem->Attribute ("radius"));

		emiter = new SphereEmiter (radius);
	}
	else if (type == "CUBE") {
		float width = std::stof (xmlElem->Attribute ("width"));
		float depth = std::stof (xmlElem->Attribute ("depth"));
		float height = std::stof (xmlElem->Attribute ("height"));

		emiter = new CubeEmiter (width, depth, height);
	}
	else if (type == "MESH") {
		std::string path = xmlElem->Attribute ("path");
		path = FileSystem::GetDirectory (filename) + path;

		Resource<Model> mesh = Resources::LoadModel (path);

		emiter = new MeshEmiter (mesh);
	}

	return emiter;
}

void ParticleSystemLoader::ProcessParticle (TiXmlElement* xmlElem, Emiter* emiter, 
	const std::string& filename)
{
	std::string type = xmlElem->Attribute ("type");

	Particle* particlePrototype = nullptr;

	if (type == "BILLBOARD") {
		particlePrototype = new BillboardParticle ();
	} 
	else if (type == "MESH") {
		particlePrototype = new MeshParticle ();
	}

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Mesh") {
			ProcessParticleMesh (content, particlePrototype, filename);
		}

		content = content->NextSiblingElement ();
	}

	emiter->SetParticlePrototype (particlePrototype);
}

void ParticleSystemLoader::ProcessParticleMesh (TiXmlElement* xmlElem, Particle* prototype, 
	const std::string& filename)
{
	Resource<Model> mesh = nullptr;

	std::string type = xmlElem->Attribute ("type");

	if (type == "Primitive") {
		Primitive::Type ptype = (Primitive::Type) 
			std::stoi (xmlElem->Attribute ("index"));

		mesh = Primitive::Instance ()->Create (ptype);
	}
	else if (type == "Path") {
		std::string path = xmlElem->Attribute ("path");
		path = FileSystem::GetDirectory (filename) + path;

		mesh = Resources::LoadModel (path);
	}

	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Material") {
			ProcessMeshMaterial (content, mesh, filename);
		}

		content = content->NextSiblingElement ();
	}

	prototype->SetMesh (mesh);
}

void ParticleSystemLoader::ProcessMeshMaterial (TiXmlElement* xmlElem, Resource<Model>& mesh,
	const std::string& filename)
{
	std::string matLbName = xmlElem->FirstChildElement ("MaterialLibrary")->GetText ();
	std::string matName = xmlElem->FirstChildElement ("MaterialName")->GetText ();

	matLbName = FileSystem::GetDirectory (filename) + matLbName;
	MaterialLibrary* matLb = Resources::LoadMaterialLibrary (matLbName);

	matName = matLbName + "::" + matName;
	Material* mat = matLb->GetMaterial (matName);

	MaterialManager::Instance ()->AddMaterial (mat);

	for_each_type (ObjectModel*, objModel, *mesh) {
		for (PolygonGroup* polyGroup : *objModel) {
			polyGroup->SetMaterialName (mat->name);
		}
	}
}

void ParticleSystemLoader::ProcessTransform (TiXmlElement* xmlElem, Emiter* emiter)
{
	TiXmlElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Position") {
			emiter->GetTransform ()->SetPosition (GetVector (content));
		}
		else if (name == "Scale") {
			emiter->GetTransform ()->SetScale (GetVector (content));
		}
		else if (name == "Rotation") {
			emiter->GetTransform ()->SetRotation (GetQuaternion (content));
		}

		content = content->NextSiblingElement ();
	}
}

glm::vec3 ParticleSystemLoader::GetVector (TiXmlElement* xmlElem)
{
	glm::vec3 vec;

	const char* x = xmlElem->Attribute ("x");
	const char* y = xmlElem->Attribute ("y");
	const char* z = xmlElem->Attribute ("z");

	if (x) {
		vec.x = std::stof (x);
	}

	if (y) {
		vec.y = std::stof (y);
	}

	if (z) {
		vec.z = std::stof (z);
	}

	return vec;
}

glm::quat ParticleSystemLoader::GetQuaternion (TiXmlElement* xmlElem)
{
	glm::vec3 vec = GetVector (xmlElem);

	return glm::quat (vec);
}

void ParticleSystemLoader::ProcessEmisShape (TiXmlElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	if (type == "CONE") {
		float height = std::stof (xmlElem->Attribute ("height"));
		float radius = std::stof (xmlElem->Attribute ("radius"));

		emiter->SetEmissionShape (height, radius);
	}
}

void ParticleSystemLoader::ProcessScaleCurve (TiXmlElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	// TODO: implement more animation curves
	if (type == "EASE") {
		int ease = std::stoi (xmlElem->Attribute ("easeindex"));

		emiter->SetScaleCurve ( new AnimationCurve ((EaseCurve::EaseType) ease));
	}
}

void ParticleSystemLoader::ProcessTweenCurve (TiXmlElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	// TODO: implement more animation curves
	if (type == "EASE") {
		int ease = std::stoi (xmlElem->Attribute ("easeindex"));

		emiter->SetTweenCurve (new AnimationCurve ((EaseCurve::EaseType) ease));
	}
}

void ParticleSystemLoader::ProcessLifetimeRange (TiXmlElement* xmlElem, Emiter* emiter)
{
	unsigned int minim = std::stoi (xmlElem->Attribute ("min"));
	unsigned int maxim = std::stoi (xmlElem->Attribute ("max"));

	emiter->SetPartLifetimeRange (minim, maxim);
}

void ParticleSystemLoader::ProcessScaleRange (TiXmlElement* xmlElem, Emiter* emiter)
{
	float minim = std::stof (xmlElem->Attribute ("min"));
	float maxim = std::stof (xmlElem->Attribute ("max"));

	emiter->SetPartScaleRange (minim, maxim);
}

void ParticleSystemLoader::ProcessSpeedRange (TiXmlElement* xmlElem, Emiter* emiter)
{
	float minim = std::stof (xmlElem->Attribute ("min"));
	float maxim = std::stof (xmlElem->Attribute ("max"));

	emiter->SetPartSpeedRange (minim, maxim);
}

void ParticleSystemLoader::ProcessDepthMask (TiXmlElement* xmlElem, ParticleSystem* partSys)
{
	bool check = Extensions::StringExtend::ToBool (xmlElem->Attribute ("check"));

	partSys->SetDepthMaskCheck (check);
}