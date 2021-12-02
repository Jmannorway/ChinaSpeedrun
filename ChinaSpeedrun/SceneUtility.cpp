#include "SceneUtility.h"

#include "SceneManager.h"
#include "JCollisionShape.h"
#include "GameObject.h"
#include "Mesh.h"

bool cs::SceneUtility::CreateStaticTriangleCollidersFromMesh(const Mesh* const mesh, Vector3 offset)
{
    if (mesh)
    {
        const auto _indices = mesh->GetIndices();
        const auto _vertices = mesh->GetVertices();

        for (unsigned i = 0, _objIndex = 0; i < _indices.size() - 2; i += 3)
        {
            auto _obj = SceneManager::InstanceObject(
                ("Collision triangle " + std::to_string(_objIndex)).c_str(),
                offset);
            auto& _jpc = _obj->AddComponent<JPhysicsComponent>();
            _jpc.shape = new JCollisionTriangle(
                _vertices[_indices[i]].position,
                _vertices[_indices[i + 1]].position,
                _vertices[_indices[i + 2]].position);
            _jpc.gravityScale = 0.f;
            _objIndex++;
        }

        return true;
    }

    return false;
}
