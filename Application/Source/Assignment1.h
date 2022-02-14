#ifndef ASSIGNMENT1_H
#define ASSIGNMENT1_H

#include "Scene.h"
#include "Camera2.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Light.h"

class Assignment1 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CUBE_1,
		GEO_CUBE_2,
		GEO_CUBE_3,
		GEO_CUBE_4,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_SPHERE_1,
		GEO_SPHERE_2,
		GEO_SPHERE_3,
		GEO_LIGHTBALL,
		GEO_CYLINDER,
		GEO_CYLINDER_1,
		GEO_CONE,
		GEO_CONE_1,
		GEO_TORUS,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_NUMLIGHTS,
		U_TOTAL,

	};
public:
	Assignment1();
	~Assignment1();
	float Rotationcount,count,Ymovement,hand_rotation,m_count;
	bool Ybool;
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
private:
	void RenderMesh(Mesh* mesh, bool enableLight);
	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle;

	MS modelStack, viewStack, projectionStack;

	Camera2 camera;
	Light light[1];
};

#endif