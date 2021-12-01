#include "JCollision.h"

cs::JCollision::JCollision(JPhysicsComponent& a, TransformComponent& at, JPhysicsComponent& b,
	TransformComponent& bt, JCollisionPoints points) :
	a(a), b(b), at(at), bt(bt), points(points)
{
}
