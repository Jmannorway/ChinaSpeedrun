#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <b2/b2_fixture.h>

#include "Mathf.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"

namespace glm
{
	template<class Archive>
	void serialize(Archive& ar, Vector1& v)
	{
		ar(
			cereal::make_nvp("x", v.x)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, Vector2& v)
	{
		ar(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, Vector3& v)
	{
		ar(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, Vector4& v)
	{
		ar(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}
}
																								 
template<class Archive>																			 
void serialize(Archive& ar, b2Vec2& v)															 
{																								 
	ar(																							 
		cereal::make_nvp("x", v.x),																 
		cereal::make_nvp("y", v.y)																 
	);																							 
}																								 
																								 
template<class Archive>																			 
void serialize(Archive& ar, b2BodyDef& bdef)													 
{																								 
	ar(																							 
		cereal::make_nvp("position",		bdef.position			),									 
		cereal::make_nvp("angle",			bdef.angle				),									 
		cereal::make_nvp("linear velocity", bdef.linearVelocity		),									 
		cereal::make_nvp("angular velocity",bdef.angularVelocity	),									 
		cereal::make_nvp("linear damping",	bdef.linearDamping		),									 
		cereal::make_nvp("angular damping", bdef.angularDamping		),									 
		cereal::make_nvp("allow sleep",		bdef.allowSleep			),									 
		cereal::make_nvp("awake",			bdef.awake				),									 
		cereal::make_nvp("fixed rotation",	bdef.fixedRotation		),
		cereal::make_nvp("bullet",			bdef.bullet				),
		cereal::make_nvp("type",			bdef.type				),
		cereal::make_nvp("enabled",			bdef.enabled			),
		cereal::make_nvp("gravity scale",	bdef.gravityScale		)
	);
}

template<class Archive>
void serialize(Archive& ar, b2FixtureDef& fdef)
{
	ar(
		cereal::make_nvp("friction", fdef.friction),
		cereal::make_nvp("restitution", fdef.restitution),
		cereal::make_nvp("restitution threshold", fdef.restitutionThreshold),
		cereal::make_nvp("density", fdef.density),
		cereal::make_nvp("is sensor", fdef.isSensor)
	);
}

namespace cs
{
	/*
	 * Resource (it isn't RAII compliant to serialize these as paths)
	 * Not to mention. The engine isn't working with smart pointers at all
	 */

	/*
	 * Game object
	 */
	template<class Archive>
	void serialize(Archive& ar, GameObject& obj)
	{
		ar(
			cereal::make_nvp("name", obj.name),
			cereal::make_nvp("active", obj.active)
		);
	}

	/*
	 * Components
	 */
	template<class Archive>
	void serialize(Archive& ar, AudioComponent& c)
	{
		ar(
			cereal::make_nvp("sound name", c.soundName)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, CameraComponent& c)
	{
		ar(
			cereal::make_nvp("fov", c.fov),
			cereal::make_nvp("near", c.nearPlane),
			cereal::make_nvp("far", c.farPlane),
			cereal::make_nvp("projection", c.projection),
			cereal::make_nvp("left", c.leftPlane),
			cereal::make_nvp("right", c.rightPlane),
			cereal::make_nvp("up", c.topPlane),
			cereal::make_nvp("down", c.bottomPlane)
		);
	}

	template<class Archive>
	void save(Archive& ar, const MeshRendererComponent& c)
	{
		// TODO: Check for nullptr
		std::string
		_meshPath = c.mesh ? c.mesh->GetResourcePath() : std::string(),
		_materialPath = c.material ? c.material->GetResourcePath() : std::string(),
		_shaderPath = c.material && c.material->shader ? c.material->shader->GetResourcePath() : std::string();

		ar(
			cereal::make_nvp("mesh", _meshPath),
			cereal::make_nvp("material", _materialPath),
			cereal::make_nvp("shader", _shaderPath)
		);
	}

	template<class Archive>
	void load(Archive& ar, MeshRendererComponent& c)
	{
		std::string materialPath, meshPath, shaderPath;

		ar(
			cereal::make_nvp("mesh", meshPath),
			cereal::make_nvp("material", materialPath),
			cereal::make_nvp("shader", shaderPath)
		);

		c.SetMesh(ResourceManager::Load<Mesh>(meshPath));
		c.material = ResourceManager::Load<Material>(materialPath);
		c.material->shader = ResourceManager::Load<Shader>(shaderPath);
	}

	template<class Archive>
	void serialize(Archive& ar, PhysicsComponent& c)
	{
		ar(
			cereal::make_nvp("body definition", c.definition)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, TransformComponent& c)
	{
		ar(
			cereal::make_nvp("position", c.position),
			cereal::make_nvp("rotation", c.rotation),
			cereal::make_nvp("scale", c.scale)
		);
	}

	template<class Archive>
	void serialize(Archive& ar, JPhysicsComponent& c)
	{
		ar(
			cereal::make_nvp("mass", c.mass),
			cereal::make_nvp("velocity", c.velocity),
			cereal::make_nvp("force", c.force)
		);
	}

	template<class Archive>
	void save(Archive& ar, const ScriptComponent& c)
	{
		ar(
			cereal::make_nvp("script", c.GetScript()->GetResourcePath())
		);
	}

	template<class Archive>
	void load(Archive& ar, ScriptComponent& c)
	{
		std::string _scriptPath;

		ar(
			cereal::make_nvp("script", _scriptPath)
		);

		c.SetScript(ResourceManager::LoadScript(_scriptPath));
	}
}
