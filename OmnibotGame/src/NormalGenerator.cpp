#include "NormalGenerator.h"


using namespace std;
using namespace ci;
vector<vec3> NormalGenerator::getNormals(vector<ci::vec3> positions, std::vector<unsigned short>indices)
{
	vector<vec3> normals;
	for (auto p : positions)
	{
		vec3 n;
		normals.push_back(n);
	}

	for (int i = 0; i < indices.size(); i+=3)
	{
		int i1 = indices[i];
		int i2 = indices[i+1];
		int i3 = indices[i+2];

		vec3 A1 = positions[i2] - positions[i1];
		vec3 A2 = positions[i3] - positions[i1];

		vec3 n = glm::cross(A1, A2);
		n =glm::normalize(n);

		normals[i1] += n;
		normals[i2] += n;
		normals[i3] += n;
	}

	for (int i = 0; i < normals.size();i++)
	{
		normals[i] =glm::normalize(normals[i]);
		
	}

	return normals;


}