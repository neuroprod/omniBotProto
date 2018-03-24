#include "Leaf.h"


using namespace std;
using namespace ci;

Leaf::Leaf(){}

LeafRef  Leaf::create()
{
	return make_shared<Leaf>();
}

void Leaf::updateMatrix()
{
	//if (length2(speed)<0.01)return;

	glm::mat4 t = glm::translate(vec3(position.x-worldOffX, position.y-worldOffY, position.z));
	glm::mat4 s = glm::scale(vec3(scale, scale, scale));
	glm::mat4 rx = glm::rotate(rotation.x, vec3(1, 0, 0));
	glm::mat4 ry = glm::rotate(rotation.y, vec3(0, 1, 0));
	glm::mat4 rz = glm::rotate(rotation.z, vec3(0, 0, 1));
	matrix = t*rx*ry*rz *s;

}