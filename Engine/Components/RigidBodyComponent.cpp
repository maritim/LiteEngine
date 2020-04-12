#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent () :
	_rigidbody (new Rigidbody ())
{

}

RigidBodyComponent::~RigidBodyComponent ()
{
	delete _rigidbody;
}

void RigidBodyComponent::Awake ()
{
	_rigidbody->SetTransform (_parent->GetTransform ());

	_rigidbody->Init ();

	_rigidbody->SetMass (_mass);
	_rigidbody->SetCollider (_collider);

	_rigidbody->OnAttachedToScene ();
}

void RigidBodyComponent::Update ()
{
	// _rigidbody->Update ();
}

void RigidBodyComponent::SetActive (bool isActive)
{
	if (_rigidbody == nullptr) {
		return;
	}

	_rigidbody->SetActive (isActive);
}

void RigidBodyComponent::OnAttachedToScene ()
{
	if (_rigidbody == nullptr) {
		return;
	}

	_rigidbody->OnAttachedToScene ();
}

void RigidBodyComponent::OnDetachedFromScene ()
{
	if (_rigidbody == nullptr) {
		return;
	}

	_rigidbody->OnDetachedFromScene ();
}

