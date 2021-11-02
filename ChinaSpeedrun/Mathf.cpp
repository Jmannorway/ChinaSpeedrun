#include "Mathf.h"

#include "PolygonCollider.h"

#include <random>
#include <ctime>

#include <glm/gtx/matrix_decompose.hpp>

void cs::Mathf::InitRand()
{
    srand(time(nullptr));
}

int cs::Mathf::Rand()
{
    return rand();
}

float cs::Mathf::RandRange(const float min, const float max)
{
    return min + (rand() / (float)INT_MAX) * (max - min);
}

void cs::Mathf::Clamp(float& value, float min, float max)
{
    value =
        max * (value > max) +
        min * (value < min) +
        value * (value >= min && value <= max);
}

void cs::Mathf::LoopClamp(float& value, float min, float max)
{
    value =
        (value - max) * (value > max) +
        (value - min + max) * (value < min) +
        value * (value >= min && value <= max);
}

float cs::Mathf::Project(const Vector3 projVec, const Plane plane)
{
    return std::abs(projVec.x * plane.normal.x + projVec.y * plane.normal.y + projVec.z * plane.normal.z + plane.length);
}

Vector3 cs::Mathf::Project(const Vector3 projVec, const Vector3 alignVec)
{
    return DotProduct(alignVec, projVec) / Magnitude(projVec) * projVec;
}

float cs::Mathf::Magnitude(const Vector3 vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

Vector3 cs::Mathf::CrossProduct(const Vector3 vec1, const Vector3 vec2)
{
    return glm::cross(vec1, vec2);
}

float cs::Mathf::DotProduct(const Vector3 vec1, const Vector3 vec2)
{
    return glm::dot(vec1, vec2);
}

void cs::Mathf::DecomposeMatrix(const Matrix4x4& transform, Vector3& position, Vector3& rotation, Vector3& scale)
{
    Matrix4x4 _localMatrix{ transform };

    scale.x = glm::length(_localMatrix[0]);
    scale.y = glm::length(_localMatrix[1]);
    scale.z = glm::length(_localMatrix[2]);

    glm::normalize<3, float, glm::packed_highp>(_localMatrix[0]);
    glm::normalize<3, float, glm::packed_highp>(_localMatrix[1]);
    glm::normalize<3, float, glm::packed_highp>(_localMatrix[2]);

    rotation.x = atan2f(_localMatrix[1][2], _localMatrix[2][2]);
    rotation.y = atan2f(-_localMatrix[0][2], sqrtf(_localMatrix[1][2] * _localMatrix[1][2] + _localMatrix[2][2] * _localMatrix[2][2]));
    rotation.z = atan2f(_localMatrix[0][1], _localMatrix[0][0]);

    position = Vector3(_localMatrix[3]);
}
