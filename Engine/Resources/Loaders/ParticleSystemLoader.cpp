#include "ParticleSystemLoader.h"

#include <string>

#include "VisualEffects/ParticleSystem/ParticleSystem.h"
#include "VisualEffects/ParticleSystem/Emiter.h"
#include "VisualEffects/ParticleSystem/Particle.h"

#include "Renderer/Render/Mesh/Model.h"

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

#include "Core/Console/Console.h"

#include "Resources/Resources.h"

using namespace tinyxml2;

Object* ParticleSystemLoader::Load (const std::string& filename)
{
	XMLDocument doc;
	if(doc.LoadFile(filename.c_str ()) != XML_SUCCESS) {
		Console::LogError (filename + " has error(s) in its syntax. Cannot proceed further.");
		return nullptr;
	}

	XMLElement* root = doc.FirstChildElement ("ParticleSystem");

	if (root == NULL) {
		return NULL;
	}

	ParticleSystem* partSys = new ParticleSystem ();

	XMLElement* content = root->FirstChildElement ();

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

void ParticleSystemLoader::ProcessPartCount (XMLElement* xmlElem, ParticleSystem* partSys)
{
	std::size_t minPart = std::stoi (xmlElem->Attribute ("min"));
	std::size_t maxPart = std::stoi (xmlElem->Attribute ("max"));

	partSys->SetMinimPartCount (minPart);
	partSys->SetMaximPartCount (maxPart);
}

void ParticleSystemLoader::ProcessEmissionRate (XMLElement* xmlElem, ParticleSystem* partSys)
{
	std::size_t rate = std::stoi (xmlElem->Attribute ("rate"));

	partSys->SetEmissionRate (rate);
}

void ParticleSystemLoader::ProcessEmiter (XMLElement* xmlElem, ParticleSystem* partSys, 
	const std::string& filename)
{
	Emiter* emiter = CreateEmiter (xmlElem, filename);

	XMLElement* content = xmlElem->FirstChildElement ();

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

void ParticleSystemLoader::ProcessRenderer (XMLElement* xmlElem, ParticleSystem* partSys)
{
	XMLElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "DepthMask") {
			ProcessDepthMask (content, partSys);
		}

		content = content->NextSiblingElement ();
	}
}

Emiter* ParticleSystemLoader::CreateEmiter (XMLElement* xmlElem, const std::string& filename)
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

void ParticleSystemLoader::ProcessParticle (XMLElement* xmlElem, Emiter* emiter, 
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

	XMLElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Mesh") {
			ProcessParticleMesh (content, particlePrototype, filename);
		}
		else if (name == "TextureAtlas") {
			ProcessTextureAtlas (content, particlePrototype, filename);
		}

		content = content->NextSiblingElement ();
	}

	emiter->SetParticlePrototype (particlePrototype);
}

void ParticleSystemLoader::ProcessParticleMesh (XMLElement* xmlElem, Particle* prototype, 
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

	XMLElement* content = xmlElem->FirstChildElement ();

	while (content) {
		std::string name = content->Value ();

		if (name == "Material") {
			ProcessMeshMaterial (content, mesh, filename);
		}

		content = content->NextSiblingElement ();
	}

	prototype->SetMesh (mesh);
}

void ParticleSystemLoader::ProcessMeshMaterial (XMLElement* xmlElem, Resource<Model>& mesh,
	const std::string& filename)
{
	std::string matLbName = xmlElem->FirstChildElement ("MaterialLibrary")->GetText ();
	std::string matName = xmlElem->FirstChildElement ("MaterialName")->GetText ();

	matLbName = FileSystem::GetDirectory (filename) + matLbName;
	Resource<MaterialLibrary> matLb = Resources::LoadMaterialLibrary (matLbName);

	matName = matLbName + "::" + matName;
	Resource<Material> mat = matLb->GetMaterial (matName);

	for_each_type (ObjectModel*, objModel, *mesh) {
		for (PolygonGroup* polyGroup : *objModel) {
			polyGroup->SetMaterial (mat);
		}
	}
}

void ParticleSystemLoader::ProcessTextureAtlas (XMLElement* xmlElem, Particle* particle, const std::string& filename)
{
	auto billboardParticle = dynamic_cast<BillboardParticle*> (particle);

	if (billboardParticle == nullptr) {
		return;
	}

	std::string textureAtlasPath = xmlElem->Attribute ("path");
	textureAtlasPath = FileSystem::GetDirectory (filename) + textureAtlasPath;

	Resource<Texture> textureAtlas = Resources::LoadTextureAtlas (textureAtlasPath);

	billboardParticle->SetTextureAtlas (textureAtlas);
}

void ParticleSystemLoader::ProcessTransform (XMLElement* xmlElem, Emiter* emiter)
{
	XMLElement* content = xmlElem->FirstChildElement ();

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

glm::vec3 ParticleSystemLoader::GetVector (XMLElement* xmlElem)
{
	glm::vec3 vec (0.0f);

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

glm::quat ParticleSystemLoader::GetQuaternion (XMLElement* xmlElem)
{
	glm::vec3 vec = GetVector (xmlElem);

	return glm::quat (vec);
}

void ParticleSystemLoader::ProcessEmisShape (XMLElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	if (type == "CONE") {
		float height = std::stof (xmlElem->Attribute ("height"));
		float radius = std::stof (xmlElem->Attribute ("radius"));

		emiter->SetEmissionShape (height, radius);
	}
}

void ParticleSystemLoader::ProcessScaleCurve (XMLElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	// TODO: implement more animation curves
	if (type == "EASE") {
		int ease = std::stoi (xmlElem->Attribute ("easeindex"));

		emiter->SetScaleCurve ( new AnimationCurve ((EaseCurve::EaseType) ease));
	}
}

void ParticleSystemLoader::ProcessTweenCurve (XMLElement* xmlElem, Emiter* emiter)
{
	std::string type = xmlElem->Attribute ("type");

	// TODO: implement more animation curves
	if (type == "EASE") {
		int ease = std::stoi (xmlElem->Attribute ("easeindex"));

		emiter->SetTweenCurve (new AnimationCurve ((EaseCurve::EaseType) ease));
	}
}

void ParticleSystemLoader::ProcessLifetimeRange (XMLElement* xmlElem, Emiter* emiter)
{
	unsigned int minim = std::stoi (xmlElem->Attribute ("min"));
	unsigned int maxim = std::stoi (xmlElem->Attribute ("max"));

	emiter->SetPartLifetimeRange (minim, maxim);
}

void ParticleSystemLoader::ProcessScaleRange (XMLElement* xmlElem, Emiter* emiter)
{
	float minim = std::stof (xmlElem->Attribute ("min"));
	float maxim = std::stof (xmlElem->Attribute ("max"));

	emiter->SetPartScaleRange (minim, maxim);
}

void ParticleSystemLoader::ProcessSpeedRange (XMLElement* xmlElem, Emiter* emiter)
{
	float minim = std::stof (xmlElem->Attribute ("min"));
	float maxim = std::stof (xmlElem->Attribute ("max"));

	emiter->SetPartSpeedRange (minim, maxim);
}

void ParticleSystemLoader::ProcessDepthMask (XMLElement* xmlElem, ParticleSystem* partSys)
{
	bool check = Extensions::StringExtend::ToBool (xmlElem->Attribute ("check"));

	partSys->SetDepthMaskCheck (check);
}