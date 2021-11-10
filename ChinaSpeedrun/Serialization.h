#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

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

namespace cs
{
	template<class Archive>
	void serialize(Archive& ar, GameObject& obj)
	{
		ar(
			cereal::make_nvp("name", obj.name),
			cereal::make_nvp("active", obj.active)
		);
	}

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
			cereal::make_nvp("far", c.farPlane)
		);
	}

	template<class Archive>
	void save(Archive& ar, const MeshRendererComponent& c)
	{
		ar(
			cereal::make_nvp("mesh", c.mesh->GetResourcePath()),
			cereal::make_nvp("material", c.material->GetResourcePath())
		);
	}

	template<class Archive>
	void load(Archive& ar, MeshRendererComponent& c)
	{
		std::string materialPath, meshPath;

		ar(
			cereal::make_nvp("mesh", meshPath),
			cereal::make_nvp("material", materialPath)
		);

		c.SetMesh(ResourceManager::Load<Mesh>(meshPath));
		c.material = ResourceManager::Load<Material>(materialPath);
	}

	template<class Archive>
	void serialize(Archive& ar, PhysicsComponent& c)
	{
		ar(
			cereal::make_nvp("gravity scale", c.definition.gravityScale),
			cereal::make_nvp("linear velocity", c.definition.linearVelocity),
			cereal::make_nvp("angular velocity", c.definition.angularVelocity)
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
}


