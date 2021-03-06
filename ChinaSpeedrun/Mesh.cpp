#include "Mesh.h"

#include <iterator>

#include "Material.h"
#include "ChinaEngine.h"
#include "VulkanEngineRenderer.h"
#include "ResourceManager.h"

cs::Mesh::Mesh() :
	vertices{}, indices{}
{
	Initialize();
}

cs::Mesh::Mesh(std::vector<Vertex> vertexArray, std::vector<uint32_t> indexArray) :
	vertices{vertexArray}, indices{indexArray}
{
	Initialize();
}

void cs::Mesh::Initialize()
{
	ChinaEngine::renderer.SolveMesh(this, Solve::ADD);
	//ChinaEngine::renderer.AllocateMesh(vertices, indices, vertexBufferOffset, indexBufferOffset, vertexSize, indexSize, vertexBufferRef, indexBufferRef);
}

cs::Mesh* cs::Mesh::CreateDefaultPlane(const Vector2 extent)
{
	const std::vector<Vertex> _vertices
	{
		{{-extent.x, -extent.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{extent.x, -extent.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{extent.x, extent.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{-extent.x, extent.y, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	const std::vector<uint32_t> _indices
	{
		0, 1, 2, 2, 3, 0
	};

	Mesh* _outMesh{ new Mesh(_vertices, _indices) };
	_outMesh->resourcePath = "default_plane";
	ResourceManager::ForcePush<Mesh>(_outMesh);
	return _outMesh;
}

cs::Mesh* cs::Mesh::CreateDefaultCube(const Vector3 extent)
{
	const std::vector<Vertex> _vertices
	{
		{{-extent.x, -extent.y, extent.z}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{extent.x, -extent.y, extent.z}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{extent.x, extent.y, extent.z}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{-extent.x, extent.y, extent.z}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
		{{-extent.x, -extent.y, -extent.z}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
		{{extent.x, -extent.y, -extent.z}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
		{{extent.x, extent.y, -extent.z}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{-extent.x, extent.y, -extent.z}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
	};

	const std::vector<uint32_t> _indices
	{
		0, 1, 2, 2, 3, 0,
		4, 0, 7, 7, 0, 3,
		2, 7, 3, 2, 6, 7,
		2, 1, 6, 6, 1, 5,
		1, 0, 4, 4, 5, 1,
		5, 4, 6, 6, 4, 7
	};

	Mesh* _outMesh{ new Mesh(_vertices, _indices) };
	_outMesh->resourcePath = "default_cube";
	ResourceManager::ForcePush<Mesh>(_outMesh);
	return _outMesh;
}

cs::Mesh* cs::Mesh::CreateDefaultIcoSphere(const uint8_t subdivisions, const float radius)
{
	std::vector<cs::Vertex> _vertices{};
	std::vector<uint32_t> _indices{};

	return new Mesh(_vertices, _indices);
}

void cs::Mesh::SetMesh(std::vector<cs::Vertex> vertexArray, std::vector<uint32_t> indexArray)
{
	vertices = vertexArray;
	indices = indexArray;
}

std::vector<cs::Vertex> const& cs::Mesh::GetVertices() const
{
	return vertices;
}

std::vector<uint32_t> const& cs::Mesh::GetIndices() const
{
	return indices;
}
