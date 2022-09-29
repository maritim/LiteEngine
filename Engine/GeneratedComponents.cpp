#include "Systems/Components/EditorComponent.h"
#include "Resources/Loaders/ComponentLoaders/ComponentAttributeLoader.h"
#include "Resources/Savers/ComponentSavers/ComponentAttributeSaver.h"
#include "Systems/Components/ComponentAttributeWidget.h"

#include "Components\AmbientLightComponent.h"
class ENGINE_API HTAmbientLightComponent : public AmbientLightComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "color") {
				_color = ComponentAttributeLoader::GetValue<Color> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "intensity") {
				_intensity = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "color") {
				_color = ComponentAttributeLoader::Load<Color> (content);
			}
			if (std::string (content->Value ()) == "intensity") {
				_intensity = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Color> (xmlElem, _color, "color");
		ComponentAttributeSaver::Save<float> (xmlElem, _intensity, "intensity");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_color);
			if (ComponentAttributeWidget::Show<Color> (_color, "color")) {
				SetColor (_color);
			}
			ImGui::PopID ();
			ImGui::PushID (&_intensity);
			if (ComponentAttributeWidget::Show<float> (_intensity, "intensity")) {
				SetIntensity (_intensity);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTAmbientLightComponent)

#include "Components\AudioSourceComponent.h"
class ENGINE_API HTAudioSourceComponent : public AudioSourceComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "audioClip") {
				_audioClip = ComponentAttributeLoader::GetValue<Resource<AudioClip>> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "volume") {
				_volume = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "loop") {
				_loop = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "audioClip") {
				_audioClip = ComponentAttributeLoader::Load<Resource<AudioClip>> (content);
			}
			if (std::string (content->Value ()) == "volume") {
				_volume = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "loop") {
				_loop = ComponentAttributeLoader::Load<bool> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Resource<AudioClip>> (xmlElem, _audioClip, "audioClip");
		ComponentAttributeSaver::Save<float> (xmlElem, _volume, "volume");
		ComponentAttributeSaver::Save<bool> (xmlElem, _loop, "loop");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_audioClip);
			if (ComponentAttributeWidget::Show<Resource<AudioClip>> (_audioClip, "audioClip")) {
				SetAudioClip (_audioClip);
			}
			ImGui::PopID ();
			ImGui::PushID (&_volume);
			if (ComponentAttributeWidget::Show<float> (_volume, "volume")) {
				SetVolume (_volume);
			}
			ImGui::PopID ();
			ImGui::PushID (&_loop);
			if (ComponentAttributeWidget::Show<bool> (_loop, "loop")) {
				SetLoop (_loop);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTAudioSourceComponent)

#include "Components\Collision\BoxColliderComponent.h"
class ENGINE_API HTBoxColliderComponent : public BoxColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "extents") {
				_extents = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "extents") {
				_extents = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _extents, "extents");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (&_extents);
			if (ComponentAttributeWidget::Show<glm::vec3> (_extents, "extents")) {
				SetExtents (_extents);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTBoxColliderComponent)

#include "Components\Cameras\CameraComponent.h"
class ENGINE_API HTCameraComponent : public CameraComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "aspect") {
				_aspect = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zNear") {
				_zNear = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zFar") {
				_zFar = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "aspect") {
				_aspect = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zNear") {
				_zNear = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zFar") {
				_zFar = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::Load<bool> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<float> (xmlElem, _aspect, "aspect");
		ComponentAttributeSaver::Save<float> (xmlElem, _zNear, "zNear");
		ComponentAttributeSaver::Save<float> (xmlElem, _zFar, "zFar");
		ComponentAttributeSaver::Save<bool> (xmlElem, _constrainAspect, "constrainAspect");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_aspect);
			if (ComponentAttributeWidget::Show<float> (_aspect, "aspect")) {
				SetAspect (_aspect);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zNear);
			if (ComponentAttributeWidget::Show<float> (_zNear, "zNear")) {
				SetZNear (_zNear);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zFar);
			if (ComponentAttributeWidget::Show<float> (_zFar, "zFar")) {
				SetZFar (_zFar);
			}
			ImGui::PopID ();
			ImGui::PushID (&_constrainAspect);
			if (ComponentAttributeWidget::Show<bool> (_constrainAspect, "constrainAspect")) {
				SetConstrainAspect (_constrainAspect);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTCameraComponent)

#include "Components\Collision\CapsuleColliderComponent.h"
class ENGINE_API HTCapsuleColliderComponent : public CapsuleColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "radius") {
				_radius = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "radius") {
				_radius = ComponentAttributeLoader::Load<float> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "height") {
				_height = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "height") {
				_height = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
		ComponentAttributeSaver::Save<float> (xmlElem, _radius, "radius");
		ComponentAttributeSaver::Save<float> (xmlElem, _height, "height");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (&_radius);
			if (ComponentAttributeWidget::Show<float> (_radius, "radius")) {
				SetRadius (_radius);
			}
			ImGui::PopID ();
			ImGui::PushID (&_height);
			if (ComponentAttributeWidget::Show<float> (_height, "height")) {
				SetHeight (_height);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTCapsuleColliderComponent)

#include "Components\Collision\ColliderComponent.h"
class ENGINE_API HTColliderComponent : public ColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTColliderComponent)

#include "Components\Collision\CylinderColliderComponent.h"
class ENGINE_API HTCylinderColliderComponent : public CylinderColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "extents") {
				_extents = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "extents") {
				_extents = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _extents, "extents");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (&_extents);
			if (ComponentAttributeWidget::Show<glm::vec3> (_extents, "extents")) {
				SetExtents (_extents);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTCylinderColliderComponent)

#include "Components\Lighting\DirectionalLightComponent.h"
class ENGINE_API HTDirectionalLightComponent : public DirectionalLightComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "color") {
				_color = ComponentAttributeLoader::GetValue<Color> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "intensity") {
				_intensity = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::GetValue<LightComponent::ShadowInformation> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "color") {
				_color = ComponentAttributeLoader::Load<Color> (content);
			}
			if (std::string (content->Value ()) == "intensity") {
				_intensity = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::Load<bool> (content);
			}
			if (std::string (content->Value ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::Load<LightComponent::ShadowInformation> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Color> (xmlElem, _color, "color");
		ComponentAttributeSaver::Save<float> (xmlElem, _intensity, "intensity");
		ComponentAttributeSaver::Save<bool> (xmlElem, _castShadows, "castShadows");
		ComponentAttributeSaver::Save<LightComponent::ShadowInformation> (xmlElem, _shadowInformation, "shadowInformation");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_color);
			if (ComponentAttributeWidget::Show<Color> (_color, "color")) {
				SetColor (_color);
			}
			ImGui::PopID ();
			ImGui::PushID (&_intensity);
			if (ComponentAttributeWidget::Show<float> (_intensity, "intensity")) {
				SetIntensity (_intensity);
			}
			ImGui::PopID ();
			ImGui::PushID (&_castShadows);
			if (ComponentAttributeWidget::Show<bool> (_castShadows, "castShadows")) {
				SetCastShadows (_castShadows);
			}
			ImGui::PopID ();
			ImGui::PushID (&_shadowInformation);
			if (ComponentAttributeWidget::Show<LightComponent::ShadowInformation> (_shadowInformation, "shadowInformation")) {
				SetShadowInformation (_shadowInformation);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTDirectionalLightComponent)

#include "Components\Lighting\LightComponent.h"
class ENGINE_API HTLightComponent : public LightComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "color") {
				_color = ComponentAttributeLoader::GetValue<Color> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "intensity") {
				_intensity = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::GetValue<LightComponent::ShadowInformation> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "color") {
				_color = ComponentAttributeLoader::Load<Color> (content);
			}
			if (std::string (content->Value ()) == "intensity") {
				_intensity = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::Load<bool> (content);
			}
			if (std::string (content->Value ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::Load<LightComponent::ShadowInformation> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Color> (xmlElem, _color, "color");
		ComponentAttributeSaver::Save<float> (xmlElem, _intensity, "intensity");
		ComponentAttributeSaver::Save<bool> (xmlElem, _castShadows, "castShadows");
		ComponentAttributeSaver::Save<LightComponent::ShadowInformation> (xmlElem, _shadowInformation, "shadowInformation");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_color);
			if (ComponentAttributeWidget::Show<Color> (_color, "color")) {
				SetColor (_color);
			}
			ImGui::PopID ();
			ImGui::PushID (&_intensity);
			if (ComponentAttributeWidget::Show<float> (_intensity, "intensity")) {
				SetIntensity (_intensity);
			}
			ImGui::PopID ();
			ImGui::PushID (&_castShadows);
			if (ComponentAttributeWidget::Show<bool> (_castShadows, "castShadows")) {
				SetCastShadows (_castShadows);
			}
			ImGui::PopID ();
			ImGui::PushID (&_shadowInformation);
			if (ComponentAttributeWidget::Show<LightComponent::ShadowInformation> (_shadowInformation, "shadowInformation")) {
				SetShadowInformation (_shadowInformation);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTLightComponent)

#include "Components\Collision\MeshColliderComponent.h"
class ENGINE_API HTMeshColliderComponent : public MeshColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "model") {
				_model = ComponentAttributeLoader::GetValue<Resource<Model>> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "model") {
				_model = ComponentAttributeLoader::Load<Resource<Model>> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
		ComponentAttributeSaver::Save<Resource<Model>> (xmlElem, _model, "model");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (&_model);
			if (ComponentAttributeWidget::Show<Resource<Model>> (_model, "model")) {
				SetModel (_model);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTMeshColliderComponent)

#include "Components\Cameras\OrthographicCameraComponent.h"
class ENGINE_API HTOrthographicCameraComponent : public OrthographicCameraComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "aspect") {
				_aspect = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zNear") {
				_zNear = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zFar") {
				_zFar = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "aspect") {
				_aspect = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zNear") {
				_zNear = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zFar") {
				_zFar = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::Load<bool> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "width") {
				_width = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "width") {
				_width = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<float> (xmlElem, _aspect, "aspect");
		ComponentAttributeSaver::Save<float> (xmlElem, _zNear, "zNear");
		ComponentAttributeSaver::Save<float> (xmlElem, _zFar, "zFar");
		ComponentAttributeSaver::Save<bool> (xmlElem, _constrainAspect, "constrainAspect");
		ComponentAttributeSaver::Save<float> (xmlElem, _width, "width");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_aspect);
			if (ComponentAttributeWidget::Show<float> (_aspect, "aspect")) {
				SetAspect (_aspect);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zNear);
			if (ComponentAttributeWidget::Show<float> (_zNear, "zNear")) {
				SetZNear (_zNear);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zFar);
			if (ComponentAttributeWidget::Show<float> (_zFar, "zFar")) {
				SetZFar (_zFar);
			}
			ImGui::PopID ();
			ImGui::PushID (&_constrainAspect);
			if (ComponentAttributeWidget::Show<bool> (_constrainAspect, "constrainAspect")) {
				SetConstrainAspect (_constrainAspect);
			}
			ImGui::PopID ();
			ImGui::PushID (&_width);
			if (ComponentAttributeWidget::Show<float> (_width, "width")) {
				SetWidth (_width);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTOrthographicCameraComponent)

#include "Components\Cameras\PerspectiveCameraComponent.h"
class ENGINE_API HTPerspectiveCameraComponent : public PerspectiveCameraComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "aspect") {
				_aspect = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zNear") {
				_zNear = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "zFar") {
				_zFar = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "aspect") {
				_aspect = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zNear") {
				_zNear = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "zFar") {
				_zFar = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "constrainAspect") {
				_constrainAspect = ComponentAttributeLoader::Load<bool> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "fieldOfViewAngle") {
				_fieldOfViewAngle = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "fieldOfViewAngle") {
				_fieldOfViewAngle = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<float> (xmlElem, _aspect, "aspect");
		ComponentAttributeSaver::Save<float> (xmlElem, _zNear, "zNear");
		ComponentAttributeSaver::Save<float> (xmlElem, _zFar, "zFar");
		ComponentAttributeSaver::Save<bool> (xmlElem, _constrainAspect, "constrainAspect");
		ComponentAttributeSaver::Save<float> (xmlElem, _fieldOfViewAngle, "fieldOfViewAngle");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_aspect);
			if (ComponentAttributeWidget::Show<float> (_aspect, "aspect")) {
				SetAspect (_aspect);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zNear);
			if (ComponentAttributeWidget::Show<float> (_zNear, "zNear")) {
				SetZNear (_zNear);
			}
			ImGui::PopID ();
			ImGui::PushID (&_zFar);
			if (ComponentAttributeWidget::Show<float> (_zFar, "zFar")) {
				SetZFar (_zFar);
			}
			ImGui::PopID ();
			ImGui::PushID (&_constrainAspect);
			if (ComponentAttributeWidget::Show<bool> (_constrainAspect, "constrainAspect")) {
				SetConstrainAspect (_constrainAspect);
			}
			ImGui::PopID ();
			ImGui::PushID (&_fieldOfViewAngle);
			if (ComponentAttributeWidget::Show<float> (_fieldOfViewAngle, "fieldOfViewAngle")) {
				SetFieldOfViewAngle (_fieldOfViewAngle);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTPerspectiveCameraComponent)

#include "Components\Lighting\PointLightComponent.h"
class ENGINE_API HTPointLightComponent : public PointLightComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "color") {
				_color = ComponentAttributeLoader::GetValue<Color> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "intensity") {
				_intensity = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::GetValue<LightComponent::ShadowInformation> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "color") {
				_color = ComponentAttributeLoader::Load<Color> (content);
			}
			if (std::string (content->Value ()) == "intensity") {
				_intensity = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::Load<bool> (content);
			}
			if (std::string (content->Value ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::Load<LightComponent::ShadowInformation> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "range") {
				_range = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "range") {
				_range = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Color> (xmlElem, _color, "color");
		ComponentAttributeSaver::Save<float> (xmlElem, _intensity, "intensity");
		ComponentAttributeSaver::Save<bool> (xmlElem, _castShadows, "castShadows");
		ComponentAttributeSaver::Save<LightComponent::ShadowInformation> (xmlElem, _shadowInformation, "shadowInformation");
		ComponentAttributeSaver::Save<float> (xmlElem, _range, "range");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_color);
			if (ComponentAttributeWidget::Show<Color> (_color, "color")) {
				SetColor (_color);
			}
			ImGui::PopID ();
			ImGui::PushID (&_intensity);
			if (ComponentAttributeWidget::Show<float> (_intensity, "intensity")) {
				SetIntensity (_intensity);
			}
			ImGui::PopID ();
			ImGui::PushID (&_castShadows);
			if (ComponentAttributeWidget::Show<bool> (_castShadows, "castShadows")) {
				SetCastShadows (_castShadows);
			}
			ImGui::PopID ();
			ImGui::PushID (&_shadowInformation);
			if (ComponentAttributeWidget::Show<LightComponent::ShadowInformation> (_shadowInformation, "shadowInformation")) {
				SetShadowInformation (_shadowInformation);
			}
			ImGui::PopID ();
			ImGui::PushID (&_range);
			if (ComponentAttributeWidget::Show<float> (_range, "range")) {
				SetRange (_range);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTPointLightComponent)

#include "Components\RenderObjectComponent.h"
class ENGINE_API HTRenderObjectComponent : public RenderObjectComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "renderStage") {
				_renderStage = ComponentAttributeLoader::GetValue<int> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "layer") {
				_layer = ComponentAttributeLoader::GetValue<int> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "model") {
				_model = ComponentAttributeLoader::GetValue<Resource<Model>> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "renderStage") {
				_renderStage = ComponentAttributeLoader::Load<int> (content);
			}
			if (std::string (content->Value ()) == "layer") {
				_layer = ComponentAttributeLoader::Load<int> (content);
			}
			if (std::string (content->Value ()) == "model") {
				_model = ComponentAttributeLoader::Load<Resource<Model>> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<int> (xmlElem, _renderStage, "renderStage");
		ComponentAttributeSaver::Save<int> (xmlElem, _layer, "layer");
		ComponentAttributeSaver::Save<Resource<Model>> (xmlElem, _model, "model");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_renderStage);
			if (ComponentAttributeWidget::Show<int> (_renderStage, "renderStage")) {
				SetRenderStage (_renderStage);
			}
			ImGui::PopID ();
			ImGui::PushID (&_layer);
			if (ComponentAttributeWidget::Show<int> (_layer, "layer")) {
				SetLayer (_layer);
			}
			ImGui::PopID ();
			ImGui::PushID (&_model);
			if (ComponentAttributeWidget::Show<Resource<Model>> (_model, "model")) {
				SetModel (_model);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTRenderObjectComponent)

#include "Components\RigidBodyComponent.h"
class ENGINE_API HTRigidBodyComponent : public RigidBodyComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "mass") {
				_mass = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "mass") {
				_mass = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<float> (xmlElem, _mass, "mass");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_mass);
			if (ComponentAttributeWidget::Show<float> (_mass, "mass")) {
				SetMass (_mass);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTRigidBodyComponent)

#include "Components\Collision\SphereColliderComponent.h"
class ENGINE_API HTSphereColliderComponent : public SphereColliderComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "offset") {
				_offset = ComponentAttributeLoader::GetValue<glm::vec3> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "offset") {
				_offset = ComponentAttributeLoader::Load<glm::vec3> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "radius") {
				_radius = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "radius") {
				_radius = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<glm::vec3> (xmlElem, _offset, "offset");
		ComponentAttributeSaver::Save<float> (xmlElem, _radius, "radius");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_offset);
			if (ComponentAttributeWidget::Show<glm::vec3> (_offset, "offset")) {
				SetOffset (_offset);
			}
			ImGui::PopID ();
			ImGui::PushID (&_radius);
			if (ComponentAttributeWidget::Show<float> (_radius, "radius")) {
				SetRadius (_radius);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTSphereColliderComponent)

#include "Components\Lighting\SpotLightComponent.h"
class ENGINE_API HTSpotLightComponent : public SpotLightComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "color") {
				_color = ComponentAttributeLoader::GetValue<Color> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "intensity") {
				_intensity = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::GetValue<bool> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::GetValue<LightComponent::ShadowInformation> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "color") {
				_color = ComponentAttributeLoader::Load<Color> (content);
			}
			if (std::string (content->Value ()) == "intensity") {
				_intensity = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "castShadows") {
				_castShadows = ComponentAttributeLoader::Load<bool> (content);
			}
			if (std::string (content->Value ()) == "shadowInformation") {
				_shadowInformation = ComponentAttributeLoader::Load<LightComponent::ShadowInformation> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "range") {
				_range = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "range") {
				_range = ComponentAttributeLoader::Load<float> (content);
			}
		}
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "cutoff") {
				_cutoff = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "outerCutoff") {
				_outerCutoff = ComponentAttributeLoader::GetValue<float> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "cutoff") {
				_cutoff = ComponentAttributeLoader::Load<float> (content);
			}
			if (std::string (content->Value ()) == "outerCutoff") {
				_outerCutoff = ComponentAttributeLoader::Load<float> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Color> (xmlElem, _color, "color");
		ComponentAttributeSaver::Save<float> (xmlElem, _intensity, "intensity");
		ComponentAttributeSaver::Save<bool> (xmlElem, _castShadows, "castShadows");
		ComponentAttributeSaver::Save<LightComponent::ShadowInformation> (xmlElem, _shadowInformation, "shadowInformation");
		ComponentAttributeSaver::Save<float> (xmlElem, _range, "range");
		ComponentAttributeSaver::Save<float> (xmlElem, _cutoff, "cutoff");
		ComponentAttributeSaver::Save<float> (xmlElem, _outerCutoff, "outerCutoff");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_color);
			if (ComponentAttributeWidget::Show<Color> (_color, "color")) {
				SetColor (_color);
			}
			ImGui::PopID ();
			ImGui::PushID (&_intensity);
			if (ComponentAttributeWidget::Show<float> (_intensity, "intensity")) {
				SetIntensity (_intensity);
			}
			ImGui::PopID ();
			ImGui::PushID (&_castShadows);
			if (ComponentAttributeWidget::Show<bool> (_castShadows, "castShadows")) {
				SetCastShadows (_castShadows);
			}
			ImGui::PopID ();
			ImGui::PushID (&_shadowInformation);
			if (ComponentAttributeWidget::Show<LightComponent::ShadowInformation> (_shadowInformation, "shadowInformation")) {
				SetShadowInformation (_shadowInformation);
			}
			ImGui::PopID ();
			ImGui::PushID (&_range);
			if (ComponentAttributeWidget::Show<float> (_range, "range")) {
				SetRange (_range);
			}
			ImGui::PopID ();
			ImGui::PushID (&_cutoff);
			if (ComponentAttributeWidget::Show<float> (_cutoff, "cutoff")) {
				SetCutoff (_cutoff);
			}
			ImGui::PopID ();
			ImGui::PushID (&_outerCutoff);
			if (ComponentAttributeWidget::Show<float> (_outerCutoff, "outerCutoff")) {
				SetOuterCutoff (_outerCutoff);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTSpotLightComponent)

#include "Components\TextGUIComponent.h"
class ENGINE_API HTTextGUIComponent : public TextGUIComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
			if (std::string (attribute->Name ()) == "font") {
				_font = ComponentAttributeLoader::GetValue<Resource<Font>> (attribute->Value ());
			}
			if (std::string (attribute->Name ()) == "text") {
				_text = ComponentAttributeLoader::GetValue<std::string> (attribute->Value ());
			}
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
			if (std::string (content->Value ()) == "font") {
				_font = ComponentAttributeLoader::Load<Resource<Font>> (content);
			}
			if (std::string (content->Value ()) == "text") {
				_text = ComponentAttributeLoader::Load<std::string> (content);
			}
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
		ComponentAttributeSaver::Save<Resource<Font>> (xmlElem, _font, "font");
		ComponentAttributeSaver::Save<std::string> (xmlElem, _text, "text");
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (&_font);
			if (ComponentAttributeWidget::Show<Resource<Font>> (_font, "font")) {
				SetFont (_font);
			}
			ImGui::PopID ();
			ImGui::PushID (&_text);
			if (ComponentAttributeWidget::Show<std::string> (_text, "text")) {
				SetText (_text);
			}
			ImGui::PopID ();
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTTextGUIComponent)

