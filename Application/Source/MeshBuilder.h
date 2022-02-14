#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float length = 1.f);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, unsigned numSlices, float radius);
	static Mesh* GenerateSemiCircle(const std::string& meshName, Color color, unsigned numSlices, float radius);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, unsigned numStacks = 10, unsigned numSlices = 10, float radius = 1.f);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, unsigned numStacks, unsigned numSlices, float radius);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, unsigned numSlices, unsigned numStacks, float OuterRadius, float InnerRadius);
	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
	static Mesh* GenerateCube(const std::string& meshName, Color color, float length);
	static Mesh* GenerateCone(const std::string& meshName, Color color, unsigned numSlices, float radius, float height);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);

	// resolution

};

#endif