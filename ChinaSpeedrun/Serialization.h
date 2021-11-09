#pragma once

#include <cereal/cereal.hpp>

#include "Mathf.h"
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

namespace cs
{
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


