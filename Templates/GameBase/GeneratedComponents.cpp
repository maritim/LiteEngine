#include "Systems/Components/EditorComponent.h"
#include "Resources/Loaders/ComponentLoaders/ComponentAttributeLoader.h"
#include "Resources/Savers/ComponentSavers/ComponentAttributeSaver.h"
#include "Systems/Components/ComponentAttributeWidget.h"

#include "Components\CameraController.h"
class GAME_API HTCameraController : public CameraController, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTCameraController)

#include "Components\CharacterController.h"
class GAME_API HTCharacterController : public CharacterController, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTCharacterController)

#include "Components\FPSCameraController.h"
class GAME_API HTFPSCameraController : public FPSCameraController, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTFPSCameraController)

#include "Components\FlyCameraController.h"
class GAME_API HTFlyCameraController : public FlyCameraController, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTFlyCameraController)

#include "Components\FrameRate.h"
class GAME_API HTFrameRate : public FrameRate, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTFrameRate)

#include "Components\StatsView.h"
class GAME_API HTStatsView : public StatsView, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTStatsView)

#include "Components\WeaponComponent.h"
class GAME_API HTWeaponComponent : public WeaponComponent, public EditorComponent
{
public:
	void Load (TiXmlElement* xmlElem)
	{
		for (TiXmlAttribute* attribute = xmlElem->FirstAttribute (); attribute != nullptr; attribute = attribute->Next ()) {
		}
		for (TiXmlElement* content = xmlElem->FirstChildElement (); content != nullptr; content = content->NextSiblingElement ()) {
		}
	}

public:
	void Save (TiXmlElement* xmlElem) const
	{
	}

public:
	void Show ()
	{
		ImGui::Separator ();
		if (ImGui::CollapsingHeader (GetName ().c_str (), ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PushID (this);
			if (ImGui::Button ("Detach Component", ImVec2 (ImGui::GetWindowWidth (), 18))) {
				_parent->DetachComponent (this);
			}
			ImGui::PopID ();
		}
	}

};
REGISTER_COMPONENT(HTWeaponComponent)

