#include "Material.h"

#include "Shader.h"

cs::Material::Material(Shader* newShader) :
	shader{newShader}
{}

void cs::Material::Initialize()
{

}
