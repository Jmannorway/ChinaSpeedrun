#include "JCollision.h"

cs::JCollision::JCollision(JPhysicsComponent& pc1, TransformComponent& tc1, JPhysicsComponent& pc2,
	TransformComponent& tc2, JCollisionPoints points) :
	pc1(pc1), pc2(pc2), tc1(tc1), tc2(tc2), points(points)
{
}
