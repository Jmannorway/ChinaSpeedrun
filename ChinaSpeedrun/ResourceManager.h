#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <fstream>

#include "Scene.h"

namespace cereal
{
	class XMLOutputArchive;
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

	class ResourceManager
	{
	public:
		friend class VulkanEngineRenderer;

		template<class T>
		static T* IsDuplicateResource(std::string filename);
		template<class T>
		static T* Load(const std::string filename);
		template<class T>
		static void Save(const std::string filename, T* resource);
		template<class T>
		static void ForcePush(T* resource);
		/*template<class T>
		static void SerializeComponent(cereal::XMLOutputArchive& archive, Scene* scene);*/

		static Mesh* LoadModel(const std::string filename);
		static AudioData* LoadAudio(const std::string filename);
		static Texture* LoadTexture(const std::string filename);
		static Shader* LoadShader(std::vector<std::string> filenames);
		static Material* LoadMaterial(const std::string filename);
		static Scene* LoadScene(const std::string filename);
		static RawData LoadRaw(const std::string filename);

		static void SaveScene(const std::string filename, const Scene* resource);

		static void ForcePushMesh(Mesh* mesh);

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
			return materials[filename];
		}
		template<>
		static Material* Load(const std::string filename)
		{
			Material* _matOut{ IsDuplicateResource<Material>(filename) };

			if (_matOut == nullptr)
				_matOut = LoadMaterial(filename);

			return _matOut;
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
		static void Save(const std::string filename, Scene* resource)
		{
			SaveScene(filename, resource);
		}

		// It is EXTREMELY dangerous to call this function from a normal script in-game. NEVER do this.
		static void ClearAllResourcePools();

	private:
		static std::unordered_map<std::string, cs::Mesh*> meshes;
		static std::unordered_map<std::string, cs::AudioData*> audio;
		static std::unordered_map<std::string, cs::Texture*> textures;
		static std::unordered_map<std::string, cs::Shader*> shaders;
		static std::unordered_map<std::string, cs::Material*> materials;
		static std::unordered_map<std::string, cs::Scene*> scenes; // Is this necessary?
	};

	/*template <class T>
	void ResourceManager::SerializeComponent(cereal::XMLOutputArchive& archive, Scene* scene)
	{
		for (auto e : scene->registry.view<T>())
		{
			auto T& _component{ scene->registry.get<T>(e) };
			archive(cereal::make_nvp(_component.gameObject->name + "." + std::to_string(_component.type), _component));
		}
	}*/
}
