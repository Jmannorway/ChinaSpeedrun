#pragma once

#include "Mathf.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <fstream>
#include <cereal/cereal.hpp>

#include "Debug.h"
#include "Scene.h"

namespace cereal
{
	class JSONInputArchive;
	class JSONOutputArchive;
}

typedef std::vector<char> RawData;

namespace cs
{
	class Material;
	class Shader;
	class Texture;
	struct AudioData;
	class Mesh;
	class Scene;
	class Script;

	class ResourceManager
	{
	public:
		friend class VulkanEngineRenderer;

		static Material* GetFirstMaterial();
		static Mesh* GetFirstMesh();

		static void CreateDefaultResources();
		static Material* GetDefaultMaterial();
		static Shader* GetDefaultShader();
		static Texture* GetDefaultTexture();
		static Mesh* GetDefaultMesh();

		template<class T>
		static T* IsDuplicateResource(std::string filename);
		template<class T>
		static T* Load(const std::string filename = "");
		template<class T>
		static bool Save(const std::string filename, T* resource);
		template<class T>
		static void ForcePush(T* resource);
		template <class T>
		static void SaveComponentsInScene(cereal::JSONOutputArchive& archive, const Scene* scene);
		template <class T>
		static void LoadComponentsInScene(cereal::JSONInputArchive& archive, Scene* scene);

		static std::vector<Vector3> LoadLAS(const std::string& filename);
		static Mesh* LoadModel(const std::string filename);
		static Mesh* LoadModelFromMapData(
			std::string filename, unsigned maxPoints = 25000, unsigned triangleNumX = 48, unsigned triangleNumY = 48);
		static AudioData* LoadAudio(const std::string filename);
		static Texture* LoadTexture(const std::string filename);
		static Shader* LoadShader(std::vector<std::string> filenames);
		static Material* LoadMaterial(const std::string filename);
		static Scene* LoadScene(std::string filename);
		static RawData LoadRaw(const std::string filename);
		static Script* LoadScript(const std::string filename);

		static bool SaveScript(const std::string& filename, Script* script);

		// TODO: Inspect this function
		static bool SaveScene(std::string filename, Scene* scene);

		static void ForcePushMesh(Mesh* mesh);
		static std::string GetNameFromPath(std::string path);

		// TODO: change IsDuplicateResource to use .find() instead of []

		template<>
		static Script* IsDuplicateResource(std::string filename)
		{
			return scripts[filename];
		}
		template<>
		static Script* Load(const std::string filename)
		{
			return LoadScript(filename);
		}

		template<>
		static bool Save(const std::string filename, Script* resource)
		{
			return SaveScript(filename, resource);
		}

		template<>
		static Mesh* IsDuplicateResource(std::string filename)
		{
			return meshes[filename];
		}
		template<>
		static Mesh* Load(const std::string filename)
		{
			return LoadModel(filename);
		}

		template<>
		static void ForcePush(Mesh* mesh)
		{
			ForcePushMesh(mesh);
		}

		template<>
		static Texture* IsDuplicateResource(std::string filename)
		{
			return textures[filename];
		}
		template<>
		static Texture* Load(const std::string filename)
		{
			return LoadTexture(filename);
		}

		template<>
		static Material* IsDuplicateResource(std::string filename)
		{
			auto _iter{ materials.find(filename) };
			return _iter != materials.end() ? _iter->second : nullptr;
		}
		template<>
		static Material* Load(const std::string filename)
		{
			return LoadMaterial(filename);
		}

		template<>
		static Shader* IsDuplicateResource(std::string filename)
		{
			return shaders[filename];
		}
		template<>
		static Shader* Load(const std::string filename)
		{
			Shader* _shaderOut{ IsDuplicateResource<Shader>(filename) };

			if (_shaderOut == nullptr)
			{
				const std::string _shaderTypes[]{ "vert", "frag", "comp", "geom" };
				std::vector<std::string> _shaderfiles;

				for (size_t i{ 0 }; i < 3; i++)
				{
					std::string _fullfilePath{ filename + '.' + _shaderTypes[i] };
					std::ifstream _file{ _fullfilePath };

					if (_file.good())
						_shaderfiles.push_back(_fullfilePath);
				}

				_shaderOut = LoadShader(_shaderfiles);
			}

			return _shaderOut;
		}

		template<>
		static Scene* IsDuplicateResource(std::string filename)
		{
			return scenes[filename];
		}
		template<>
		static Scene* Load(const std::string filename)
		{
			Scene* _scene{ IsDuplicateResource<Scene>(filename) };

			if (_scene == nullptr)
				_scene = LoadScene(filename);

			return _scene;
		}

		template<>
		static bool Save(const std::string filename, Scene* resource)
		{
			return SaveScene(filename, resource);
		}

		// It is EXTREMELY dangerous to call this function from a normal script in-game. NEVER do this.
		static void ClearAllResourcePools();

	private:
		static std::unordered_map<std::string, cs::Mesh*> meshes;
		static std::unordered_map<std::string, cs::AudioData*> audio;
		static std::unordered_map<std::string, cs::Texture*> textures;
		static std::unordered_map<std::string, cs::Shader*> shaders;
		static std::unordered_map<std::string, cs::Material*> materials;
		static std::unordered_map<std::string, cs::Scene*> scenes;
		static std::unordered_map<std::string, cs::Script*> scripts;

		static Material* defaultMaterial;
		static Shader* defaultShader;
		static Texture* defaultTexture;
		static Mesh* defaultMesh;
	};

	template <class T>
	void ResourceManager::SaveComponentsInScene(cereal::JSONOutputArchive& archive, const Scene* scene)
	{
		for (auto e : scene->registry.view<T>())
		{
			const auto c{ scene->registry.get<T>(e) };
			archive
			(
				cereal::make_nvp(c.gameObject->name + "." + c.GetTypeName(), c)
			);
		}
	}

	template <class T>
	void ResourceManager::LoadComponentsInScene(cereal::JSONInputArchive& archive, Scene* scene)
	{
		for (auto e : scene->registry.view<T>())
		{
			auto& c{ scene->registry.get<T>(e) };
			archive
			(
				cereal::make_nvp(c.gameObject->name + "." + c.GetTypeName(), c)
			);
		}
	}
}
