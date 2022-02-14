#ifndef Practical12_H
#define Practical12_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../Light.h"
#include <string>
#include <sstream>
class Practical12 : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_LIGHTBALL,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_BLEND,
		GEO_MODEL1,
		GEO_MODEL2,
		GEO_MODEL3,
		GEO_MODEL4,
		GEO_MODEL5,
		GEO_MODEL6,
		GEO_MODEL7,
		GEO_TEXT,
		GEO_CUBE_1,
		GEO_CUBE_2,
		GEO_CUBE_3,
		GEO_CUBE_4,
		GEO_CYLINDER,
		GEO_CYLINDER_1,
		GEO_CONE,
		GEO_CONE_1,
		GEO_TORUS,
		GEO_SPHERE_1,
		GEO_SPHERE_2,
		GEO_SPHERE_3,
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
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,

	};
public:
	Practical12();
	~Practical12();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	void RenderSkybox();
	virtual void Exit();
	Vector3 model_pos = Vector3(0, -1, -80);
	Vector3 chest1_pos = Vector3(50, -1, 0);
	Vector3 chest2_pos = Vector3(-50, -1, 0);
	Vector3 chest3_pos = Vector3(-50, -1, 50);
	Vector3 patrick_pos = Vector3(30, 0, -30);
	std::string dialog = "";
	std::string chest_left = "";
	std::string chest_1_dia = "";
	std::string chest_2_dia = "";
	std::string chest_3_dia = "";
	std::string pat_dia = "";
	std::ostringstream tc;

private:
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	unsigned m_vertexArrayID;
	Mesh *meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	float rotateAngle,chest1_distance, chest2_distance, chest3_distance,patrick_distance;
	bool blightenable;
	int chest1_have, chest2_have, chest3_have,total_chest;
	MS modelStack, viewStack, projectionStack;

	Camera3 camera;
	Light light[1];
	
};

#endif