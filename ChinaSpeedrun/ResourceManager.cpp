#include "ResourceManager.h"

#include "Debug.h"

#include "ChinaEngine.h"
#include "VulkanEngineRenderer.h"

#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "AudioData.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <fstream>
#include <AudioFile.h>

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include "AudioComponent.h"
#include "CameraComponent.h"
#include "PhysicsComponent.h"
#include "SceneManager.h"
#include "Serialization.h"

#ifdef NDEBUG
#include <shaderc/shaderc.hpp>
#endif

std::unordered_map<std::string, cs::Material*> cs::ResourceManager::materials;
std::unordered_map<std::string, cs::Texture*> cs::ResourceManager::textures;
std::unordered_map<std::string, cs::Mesh*> cs::ResourceManager::meshes;
std::unordered_map<std::string, cs::Shader*> cs::ResourceManager::shaders;
std::unordered_map<std::string, cs::AudioData*> cs::ResourceManager::audio;
std::unordered_map<std::string, cs::Scene*> cs::ResourceManager::scenes;

// One thing that the resource manager will do automatically is allocation to the vulkan buffers
// In other words, we have direct contact with the VulkanRenderer, and we can tell it to allocate and free resources at will

void cs::ResourceManager::LoadAllComponentsInScene(cereal::JSONInputArchive& archive, Scene* scene)
{

}

cs::Mesh* cs::ResourceManager::LoadModel(const std::string filename)
{
	Mesh* _outMesh{ IsDuplicateResource<Mesh>(filename) };

	if (_outMesh == nullptr)
	{
		_outMesh = new Mesh;

		tinyobj::attrib_t _attributes;
		std::vector<tinyobj::shape_t> _shapes;
		std::vector<tinyobj::material_t> _materials; // currently we don't do anything with the materials, but in the future we will automatically make the materials
		std::string _warning, _error;

		if (!tinyobj::LoadObj(&_attributes, &_shapes, &_materials, &_warning, &_error, filename.c_str()))
		{
			Debug::LogWarning(_warning + _error);
			delete _outMesh;
			return nullptr;
		}

		std::vector<Vertex> _vertices;
		std::vector<uint32_t> _indices;
		// by using an unordered_map we can remove duplicates
		std::unordered_map<Vertex, uint32_t> uniqueVertices{};

		for (const auto& shape : _shapes)
		{
			// in the future when we have multiple shapes, make them into separate objects
			for (const auto& index : shape.mesh.indices)
			{
				Vertex _vertex{};

				_vertex.position =
				{
					_attributes.vertices[3 * index.vertex_index + 0],
					_attributes.vertices[3 * index.vertex_index + 1],
					_attributes.vertices[3 * index.vertex_index + 2]
				};

				_vertex.texCoord =
				{
					_attributes.texcoords[2 * index.texcoord_index + 0],
					1.0f - _attributes.texcoords[2 * index.texcoord_index + 1]
				};

				_vertex.color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(_vertex) == 0)
				{
					uniqueVertices[_vertex] = static_cast<uint32_t>(_vertices.size());
					_vertices.push_back(_vertex);
				}

				_indices.push_back(uniqueVertices[_vertex]);
			}
		}

		_outMesh->SetMesh(_vertices, _indices);
		_outMesh->resourcePath = filename;
		meshes[filename] = _outMesh;

		//_outMesh->Initialize();
	}

	return _outMesh;
}

cs::Mesh* cs::ResourceManager::LoadModelFromMapData(const std::string filename)
{
	std::vector<Vector3> _points;

	// Load points from file
	{
		std::ifstream _file(filename);

		if (!_file.is_open() || _file.bad())
		{
			Debug::LogWarning("LoadModelFromMapData failed to open file");
			return nullptr;
		}

		constexpr unsigned _USER_POINT_NUMBER = 2000;

		// Skip header (in a funny way)
		{
			unsigned _header;
			_file >> _header;
		}

		// Push back all read vectors into a VECTOR (array (fuck std))
		Vector3 _vec;

		for (unsigned i = 0; !_file.eof() && i < _USER_POINT_NUMBER; i++)
		{
			_file >> _vec.x;
			_file >> _vec.y;
			_file >> _vec.z;
			_points.push_back(_vec);
		}
	}

	// Get minimum and maximum point values
	Vector3 _minPoint = _points[0], _maxPoint = _points[0];

	for (auto p : _points)
	{
		_minPoint.x = glm::min(_minPoint.x, p.x);
		_minPoint.y = glm::min(_minPoint.y, p.y);
		_minPoint.z = glm::min(_minPoint.z, p.z);

		_maxPoint.x = glm::max(_maxPoint.x, p.x);
		_maxPoint.y = glm::max(_maxPoint.y, p.y);
		_maxPoint.z = glm::max(_maxPoint.z, p.z);
	}

	Debug::LogInfo(_minPoint.x, ", ", _minPoint.y, ", ", _minPoint.z);
	Debug::LogInfo(_maxPoint.x, ", ", _maxPoint.y, ", ", _maxPoint.z);

	// Offset the collection of points to be centered
	{
		Vector3 _offset = -_minPoint;

		for (auto& p : _points)
		{
			p += _offset;
		}

		_minPoint += _offset;
		_maxPoint += _offset;
	}

	{
		Vector3 _firstPoint = _points[0];
		Vector3 _lastPoint = _points[_points.size() - 1];
		Debug::LogInfo(_firstPoint.x, ", ", _firstPoint.y, ", ", _firstPoint.z);
		Debug::LogInfo(_lastPoint.x, ", ", _lastPoint.y, ", ", _lastPoint.z);
	}

	// Create an array of vertices and fit them to their closest read-in counterpart
	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;

	{
		const unsigned _TRIANGLE_NUMBER_X = 30;
		const unsigned _TRIANGLE_NUMBER_Y = 30;
		const Vector2 _distance =
			(Vector2(_maxPoint.x, _maxPoint.y) - Vector2(_minPoint.x, _minPoint.y)) /
			Vector2(_TRIANGLE_NUMBER_X, _TRIANGLE_NUMBER_Y);
		Vector3 vec;
		int _closestPoint;
		float _pointDistance, _closestPointDistance;

		_vertices.resize(_TRIANGLE_NUMBER_X * _TRIANGLE_NUMBER_Y);

		auto _verticesVectorGetIndex = [](unsigned x, unsigned y) -> unsigned {return x * _TRIANGLE_NUMBER_Y + y; };

		for (unsigned x = 0; x < _TRIANGLE_NUMBER_X; x++)
		{
			for (unsigned y = 0; y < _TRIANGLE_NUMBER_Y; y++)
			{
				vec = { x * _distance.x, y * _distance.y, 0.f };

				_closestPointDistance = distance(vec, Vector3(_points[0].x, _points[0].y, 0.f));
				_closestPoint = 0;

				for (int i = 1; i < _points.size(); i++)
				{
					_pointDistance = distance(vec, Vector3(_points[i].x, _points[i].y, 0.f));

					if (_pointDistance < _closestPointDistance)
					{
						_closestPoint = i;
						_closestPointDistance = _pointDistance;
					}
				}

				_vertices[_verticesVectorGetIndex(x, y)] = Vertex({
					_points[_closestPoint],
					{ 0.f, 1.f, 1.f },
					{Mathf::RandRange(0.f, 1.f), Mathf::RandRange(0.f, 1.f)} });
			}
		}

		_indices.reserve(_TRIANGLE_NUMBER_X* _TRIANGLE_NUMBER_Y * 6);

		for (unsigned x = 0; x < _TRIANGLE_NUMBER_X - 1; x++)
		{
			for (unsigned y = 0; y < _TRIANGLE_NUMBER_Y - 1; y++)
			{
				_indices.push_back(_verticesVectorGetIndex(x, y));
				_indices.push_back(_verticesVectorGetIndex(x + 1, y));
				_indices.push_back(_verticesVectorGetIndex(x, y + 1));
				_indices.push_back(_verticesVectorGetIndex(x + 1, y));
				_indices.push_back(_verticesVectorGetIndex(x + 1, y + 1));
				_indices.push_back(_verticesVectorGetIndex(x, y + 1));
			}
		}
	}

	Debug::LogInfo(_vertices.size(), ", ", _indices.size());
	Mesh* _mesh = new Mesh(_vertices, _indices);
	meshes.insert({ filename, _mesh });
	return _mesh;
}

cs::AudioData* cs::ResourceManager::LoadAudio(const std::string filename)
{
	AudioFile<float> _file;

	std::vector<uint8_t> _buffer;

	if (_file.loadBuffer(filename, _buffer))
	{
		const auto _audioData{ new AudioData(
			_buffer,
			AudioMeta(static_cast<float>(_buffer.size()) /
				static_cast<float>(_file.getBitDepth()) /
				static_cast<float>(_file.getNumChannelsAsRead()) /
				static_cast<float>(_file.getSampleRate()) * 8.f,
			_file.getSampleRate(),
			_file.getBitDepth(),
			_file.getNumChannelsAsRead()))};
		audio.insert({ filename, _audioData });
		return _audioData;
	}

	return nullptr;
}

cs::Texture* cs::ResourceManager::LoadTexture(const std::string filename)
{
	Texture* _outTexture{ IsDuplicateResource<Texture>(filename) };

	if (_outTexture == nullptr)
	{
		_outTexture = new Texture;
		_outTexture->pixels = stbi_load(filename.c_str(), &_outTexture->width, &_outTexture->height, &_outTexture->usedColorChannels, STBI_rgb_alpha);

		if (_outTexture->pixels == nullptr)
		{
			Debug::LogWarning("Cannot open file : [" + filename + ']');
			return nullptr;
		}
		
		_outTexture->mipLevels = static_cast<uint32_t>(std::floor(std::log2(_outTexture->width > _outTexture->height ? _outTexture->width : _outTexture->height))) + 1;
		_outTexture->resourcePath = filename;
		textures[filename] = _outTexture;

		//_outTexture->Initialize();
	}

	return _outTexture;
}

cs::Shader* cs::ResourceManager::LoadShader(std::vector<std::string> filenames)
{
	std::unordered_map<std::string, RawData> _outShaderSPVs;

	for (std::string file : filenames)
	{
		std::string _shaderType{ file.substr(file.find_last_of('.') + 1) };

#ifdef NDEBUG
		shaderc_shader_kind _shaderKind{ shaderc_glsl_default_anyhit_shader };

		if (_shaderType == "vert")
			_shaderKind = shaderc_glsl_default_vertex_shader;
		else if (_shaderType == "frag")
			_shaderKind = shaderc_glsl_default_fragment_shader;
		else if (_shaderType == "comp")
			_shaderKind = shaderc_glsl_default_compute_shader;
		else if (_shaderType == "geom")
			_shaderKind = shaderc_glsl_default_geometry_shader;
		else
		{
			Debug::LogWarning("The shader type \"" + _shaderType + "\" is either not supported or does not exist.");
			break;
		}

		shaderc::Compiler _compiler;
		std::string _outText;

		shaderc::SpvCompilationResult _result{ _compiler.CompileGlslToSpv(_outText, shaderc_glsl_default_vertex_shader, file.c_str()) };

		if (_result.GetCompilationStatus() != shaderc_compilation_status_success)
			Debug::LogWarning(_result.GetErrorMessage());
		else
			_outShaderSPVs[_shaderType] = { _outText.begin(), _outText.end() };
#else
		std::string _newFilepath{ file };

		if (_shaderType != "vert" && _shaderType != "frag" && _shaderType != "geom" && _shaderType != "comp")
		{
			Debug::LogWarning("The shader type \"" + _shaderType + "\" is either not supported or does not exist.");
			break;
		}

		_newFilepath.insert(_newFilepath.find_last_of('.'), '_' + _shaderType);
		_newFilepath.replace(_newFilepath.find_last_of('.') + 1, _newFilepath.length(), "spv");

		_outShaderSPVs[_shaderType] = LoadRaw(_newFilepath);		
#endif // NDEBUG
	}

	std::string _shaderResource{ filenames[0].substr(0, filenames[0].find_last_of('.')) };

	shaders[_shaderResource] = new Shader(_outShaderSPVs);
	shaders[_shaderResource]->resourcePath = _shaderResource;

	return shaders[_shaderResource];
}

cs::Material* cs::ResourceManager::LoadMaterial(const std::string filename)
{
	auto _outMaterial{ IsDuplicateResource<Material>(filename) };
	// We don't have our own material formatter or reader... sooooo, empty...
	if (_outMaterial == nullptr)
	{
		_outMaterial = new Material;

		_outMaterial->resourcePath = filename;

		materials[filename] = _outMaterial;
	}

	return _outMaterial;
}

RawData cs::ResourceManager::LoadRaw(const std::string filename)
{
	std::ifstream _file{ filename, std::ios::ate | std::ios::binary };

	if (!_file.is_open())
	{
		Debug::LogWarning("Could not open: " + filename);
		return std::vector<char>();
	}

	size_t _fileSize{ (size_t)_file.tellg() };
	std::vector<char> _buffer(_fileSize);

	_file.seekg(0);
	_file.read(_buffer.data(), _fileSize);

	_file.close();

	return _buffer;
}

void cs::ResourceManager::ReloadScene(Scene* scene, std::string filename)
{
	std::ifstream _inStream(filename);

	if (_inStream.bad() || !_inStream.is_open())
		return;

	cereal::JSONInputArchive _inArchive(_inStream);

	LoadComponentsInScene<AudioComponent>(_inArchive, scene);
	LoadComponentsInScene<CameraComponent>(_inArchive, scene);
	LoadComponentsInScene<MeshRendererComponent>(_inArchive, scene);
	LoadComponentsInScene<TransformComponent>(_inArchive, scene);
	LoadComponentsInScene<PhysicsComponent>(_inArchive, scene);
}

cs::Scene* cs::ResourceManager::LoadScene(const std::string filename)
{
	std::ifstream _inStream(filename);

	if (_inStream.bad() || !_inStream.is_open())
		return nullptr;

	cereal::JSONInputArchive _inArchive(_inStream);

	std::vector<std::vector<unsigned>> _cc;

	_inArchive(cereal::make_nvp("construction count", _cc));

	// TODO: save and load scene name, and check if scene is already loaded (name == name?)
	auto _scene{ SceneManager::CreateScene("Loaded Scene") };
	
	for (int i = 0; i < _cc.size(); i++)
	{
		auto _obj{ _scene->AddGameObject() };
		_inArchive(*_obj);

		for (int ii = 0; ii < Component::__COMPONENT_ENUM_TYPE_MAX; ii++)
			for (int iii = 0; iii < _cc[i][ii]; iii++)
				_obj->AddComponentType(static_cast<Component::Type>(ii));
	}

	LoadComponentsInScene<AudioComponent>(_inArchive, _scene);
	LoadComponentsInScene<CameraComponent>(_inArchive, _scene);
	LoadComponentsInScene<MeshRendererComponent>(_inArchive, _scene);
	LoadComponentsInScene<TransformComponent>(_inArchive, _scene);
	LoadComponentsInScene<PhysicsComponent>(_inArchive, _scene);

	return _scene;
}

void cs::ResourceManager::SaveScene(const std::string filename, Scene* scene)
{
	std::ofstream _outStream(filename);

	if (_outStream.bad() || !_outStream.is_open())
		return;

	cereal::JSONOutputArchive _outArchive(_outStream);

	// Count objects and components assigned to each
	std::vector<std::vector<unsigned>> _cc;
	_cc.resize(scene->gameObjects.size());
	for (int i = 0; i < scene->gameObjects.size(); i++)
	{
		_cc[i].resize(Component::__COMPONENT_ENUM_TYPE_MAX);

		for (int ii = 0; ii < Component::__COMPONENT_ENUM_TYPE_MAX; ii++)
			_cc[i][ii] = 0;

		for (auto c : scene->gameObjects[i]->GetAllComponents())
		{
			Component::Type _type(c->GetType());

			if (_type > 0 && _type < Component::__COMPONENT_ENUM_TYPE_MAX)
				_cc[i][c->GetType()]++;
			else
				std::cerr << c->gameObject->name << " component: " << c->GetType() << " is not a valid component" << std::endl;
		}
	}

	_outArchive(cereal::make_nvp("construction count", _cc));

	// Write game object variables
	for (int i = 0; i < scene->gameObjects.size(); i++)
	{
		_outArchive(cereal::make_nvp(scene->gameObjects[i]->name, *scene->gameObjects[i]));
	}

	// Write literally all components
	SaveComponentsInScene<AudioComponent>(_outArchive, scene);
	SaveComponentsInScene<CameraComponent>(_outArchive, scene);
	SaveComponentsInScene<MeshRendererComponent>(_outArchive, scene);
	SaveComponentsInScene<TransformComponent>(_outArchive, scene);
	SaveComponentsInScene<PhysicsComponent>(_outArchive, scene);
}

void cs::ResourceManager::ForcePushMesh(Mesh* mesh)
{
	meshes[mesh->resourcePath] = mesh;
}

void cs::ResourceManager::ClearAllResourcePools()
{
	for (const std::pair<std::string, Texture*> texture : textures)
		ChinaEngine::renderer.SolveTexture(texture.second, Solve::REMOVE);
	textures.clear();

	for (const std::pair<std::string, Mesh*> mesh : meshes)
		ChinaEngine::renderer.SolveMesh(mesh.second, Solve::REMOVE);
	meshes.clear();

	for (const std::pair<std::string, Shader*> shader : shaders)
		ChinaEngine::renderer.SolveShader(shader.second, Solve::REMOVE);
	shaders.clear();

	for (const std::pair<std::string, Material*> material : materials)
		ChinaEngine::renderer.SolveMaterial(material.second, Solve::REMOVE);
	materials.clear();

	/*for (const std::pair<std::string, AudioData*> audio : audioTracks)
		delete audio.second;
	audioTracks.clear();*/
}
