#include "Assignment1.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "../Utility.h"

Assignment1::Assignment1()
{
}

Assignment1::~Assignment1()
{
}

void Assignment1::Init()
{	
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	// Set background color to dark blue
	glClearColor(0.23f, 0.52f, 0.52f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);

	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader");
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	glUseProgram(m_programID);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(-5, 10, 5);
	light[0].color.Set(1, 1, 1);
	light[0].power = 0.5;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 2.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

	//variable to rotate geometry
	count = 0;
	rotateAngle = 360;
	Ymovement = 0;
	Rotationcount = 0;
	hand_rotation = 60;


	//Initialize camera settings
	camera.Init(Vector3(0, 5, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("white_sphere", Color(1, 1, 1), 20, 40);
	meshList[GEO_SPHERE_1] = MeshBuilder::GenerateSphere("blue_sphere", Color(0, 1, 1), 20, 40);
	meshList[GEO_SPHERE_2] = MeshBuilder::GenerateSphere("black_sphere", Color(0, 0, 0), 20, 40);
	meshList[GEO_SPHERE_3] = MeshBuilder::GenerateSphere("yellow_sphere", Color(0.92, 1, 0.07), 20, 40);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("light", Color(1, 1, 1), 20, 40);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("body", Color(0.92, 1, 0.07), 1.f);
	meshList[GEO_CUBE_1] = MeshBuilder::GenerateCube("mouth", Color(0.61f, 0.13f, 0.06f), 1.f);
	meshList[GEO_CUBE_2] = MeshBuilder::GenerateCube("teeth", Color(1, 1, 1), 1.f);
	meshList[GEO_CUBE_3] = MeshBuilder::GenerateCube("pants", Color(0.63, 0.42, 0.07), 1.f);
	meshList[GEO_CUBE_4] = MeshBuilder::GenerateCube("pants", Color(0, 0, 0), 1.f);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("holes", Color(0.35f, 0.33f, 0.27f),10, 1.f, 1.f);
	meshList[GEO_CYLINDER_1] = MeshBuilder::GenerateCylinder("legs", Color(0.92, 1, 0.07), 10, 1.f, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.90f, 0.15f, 0.07f), 10, 1.f, 1.f);
	meshList[GEO_CONE_1] = MeshBuilder::GenerateCone("cone", Color(1,1,1), 10, 1.f, 1.f);
	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("torus", Color(0.63, 0.42, 0.07), 10, 20,1.f,0.5f);

}

void Assignment1::Update(double dt)
{
	float LSPEED=10.f;
	if (Application::IsKeyPressed('5'))
	{
		//to do: switch light type to POINT and pass the information to shader
		light[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('6'))
	{
		//to do: switch light type to DIRECTIONAL and pass the information to shader
		light[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}
	else if (Application::IsKeyPressed('7'))
	{
		//to do: switch light type to SPOT and pass the information to shader
		light[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	}

	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if (Application::IsKeyPressed('Z')&& count==0)
	{
		count += 1;
	}
	if(count>0)
	{
		if (Rotationcount == 0)
		{
			Ybool = true;
		}
		else if (Rotationcount >=100)
		{
			Ybool = false;
			if(Ymovement <= 0)
			{
			Rotationcount = 0;
			count = 0;
			}
		}
		if (Ybool == true && Ymovement<=8)
		{
			Ymovement += 4 * dt;
		}
		else if (Ybool==false && Ymovement >0)
		{
			Ymovement -= 4 * dt;
		}
		if (count <= 3 && Ymovement>=8)
		{
			rotateAngle += (float)(180 * dt);
			count += (float)(1 * dt);
			Rotationcount += (float)(50*dt);
		}
	}
	if (Application::IsKeyPressed('R'))
	{
		count = 0;
		m_count = 0;
		rotateAngle = 360;
		Ymovement = 0;
		Rotationcount = 0;
		hand_rotation = 60;
		light[0].position.Set(-5, 10, 5);
	}

	if (Application::IsKeyPressed('X') && m_count == 0)
	{
		m_count += 1;
	}
	if (m_count > 0 && m_count<3)
	{
		if (hand_rotation > 20)
		{
			hand_rotation -= (float)(20*dt);
			m_count += (float)(1 * dt);
		}
	}
	else if (m_count>=3 &&m_count<5)
	{

		std::cout << m_count << std::endl;
		std::cout << hand_rotation << std::endl;
		hand_rotation += (float)(20 * dt);
		m_count += (float)(1 * dt);
	}
	else if (m_count==4.9)
	{
		m_count = 0;
	}
	
	camera.Update(dt);
}

void Assignment1::Render()
{

		// Render VBO here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		viewStack.LoadIdentity();
		viewStack.LookAt(
			camera.position.x, camera.position.y,
			camera.position.z, camera.target.x, camera.target.y,
			camera.target.z, camera.up.x, camera.up.y, camera.up.z);
		modelStack.LoadIdentity();

		if (light[0].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
		}
		else if (light[0].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		}

		modelStack.PushMatrix();
		modelStack.Translate(0, Ymovement, 0);
		modelStack.Rotate(rotateAngle, 1, 0, 0);
		modelStack.Translate(0, 0, 0);
			
			//body holes(cylinder)
			modelStack.PushMatrix();
			modelStack.Translate(1.5f, 7.5f, 0.76f);
			modelStack.Scale(0.2f, 0.2f, 0.5f);
			modelStack.Rotate(90, 1, 0, 0);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();

			//Right eyes(sphere)
			modelStack.PushMatrix();
			modelStack.Translate(0.5, 7, 0.76f);
			modelStack.Scale(1, 1, 0.3f);
				//Right eyes(blue_sphere)
				modelStack.PushMatrix();
				modelStack.Translate(0, 0, 0.6f);
				modelStack.Scale(0.4f, 0.4f, 0.5f);
				modelStack.Rotate(0, 1, 0, 0);
					//Right eyes(black_sphere)
					modelStack.PushMatrix();
					modelStack.Translate(0, 0, 1);
					modelStack.Scale(0.2f, 0.2f, 0.3f);
					modelStack.Rotate(0, 1, 0, 0);
					RenderMesh(meshList[GEO_SPHERE_2], true);
					modelStack.PopMatrix();
				RenderMesh(meshList[GEO_SPHERE_1], true);
				modelStack.PopMatrix();	
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();

			//Left eyes(sphere)
			modelStack.PushMatrix();
			modelStack.Translate(-0.5, 7, 0.76f);
			modelStack.Scale(1, 1, 0.3f);
				//Left eyes(blue_sphere)
				modelStack.PushMatrix();
				modelStack.Translate(-0, 0, 0.6f);
				modelStack.Scale(0.4f, 0.4f, 0.5f);
				modelStack.Rotate(0, 1, 0, 0);
					//Left eyes(black_sphere)
					modelStack.PushMatrix();
					modelStack.Translate(0, 0, 1);
					modelStack.Scale(0.2f, 0.2f, 0.3f);
					modelStack.Rotate(0, 1, 0, 0);
					RenderMesh(meshList[GEO_SPHERE_2], true);
					modelStack.PopMatrix();
				RenderMesh(meshList[GEO_SPHERE_1], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_SPHERE], true);
			modelStack.PopMatrix();

			//Nose(yellow_sphere)
			modelStack.PushMatrix();
			modelStack.Translate(0, 6.3f, 1);
			modelStack.Scale(0.3f, 0.3f, 1);
			modelStack.Rotate(0, 1, 0, 0);
			RenderMesh(meshList[GEO_SPHERE_3], true);
			modelStack.PopMatrix();

			//Mouth(red_cube)
			modelStack.PushMatrix();
			modelStack.Translate(0, 5.2, 0.55f);
			modelStack.Scale(2, 1, 1);
				//Left Teeth(white_cube)
				modelStack.PushMatrix();
				modelStack.Translate(-0.1, 0.28, 0.45);
				modelStack.Scale(0.1, 0.4, 0.2);
				RenderMesh(meshList[GEO_CUBE_2], true);
				modelStack.PopMatrix();

				//Right Teeth(white_cube)
				modelStack.PushMatrix();
				modelStack.Translate(0.1, 0.28, 0.45);
				modelStack.Scale(0.1, 0.4, 0.2);
				RenderMesh(meshList[GEO_CUBE_2], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_CUBE_1], true);
			modelStack.PopMatrix();

			
			//Lower body(white_cube)
			modelStack.PushMatrix();
			modelStack.Translate(0, 3.65, 0);
			modelStack.Scale(4, 0.7, 2);
				//1 red cone for tie
				modelStack.PushMatrix();
				modelStack.Translate(0, 0.3, 0.5);
				modelStack.Scale(0.15, 0.5, 0.1);
				modelStack.Rotate(180, 1, 0, 0);
				RenderMesh(meshList[GEO_CONE], true);
				modelStack.PopMatrix();
				//2 red cone for tie
				modelStack.PushMatrix();
				modelStack.Translate(0, -0.2, 0.5);
				modelStack.Scale(0.1, 0.5, 0.1);
				RenderMesh(meshList[GEO_CONE], true);
				modelStack.PopMatrix();
				//3 red cone for tie
				modelStack.PushMatrix();
				modelStack.Translate(0, -0.75, 0.5);
				modelStack.Scale(0.1, 0.6, 0.1);
				modelStack.Rotate(180, 1, 0, 0);
				RenderMesh(meshList[GEO_CONE], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_CUBE_2], true);
			modelStack.PopMatrix();

			//Lower pants(brown_cube)
			modelStack.PushMatrix();
			modelStack.Translate(0, 2.95, 0);
			modelStack.Scale(4, 0.7, 2);
				//1_Lower pants(black_cube)
				modelStack.PushMatrix();
				modelStack.Translate(-0.3, 0.3, 0.5);
				modelStack.Scale(0.1, 0.2, 0.1);
				RenderMesh(meshList[GEO_CUBE_4], true);
				modelStack.PopMatrix();
				//2_Lower pants(black_cube)
				modelStack.PushMatrix();
				modelStack.Translate(-0.15, 0.3, 0.5);
				modelStack.Scale(0.1, 0.2, 0.1);
				RenderMesh(meshList[GEO_CUBE_4], true);
				modelStack.PopMatrix();
				//3_Lower pants(black_cube)
				modelStack.PushMatrix();
				modelStack.Translate(0.15, 0.3, 0.5);
				modelStack.Scale(0.1, 0.2, 0.1);
				RenderMesh(meshList[GEO_CUBE_4], true);
				modelStack.PopMatrix();
				//4_Lower pants(black_cube)
				modelStack.PushMatrix();
				modelStack.Translate(0.3, 0.3, 0.5);
				modelStack.Scale(0.1, 0.2, 0.1);
				RenderMesh(meshList[GEO_CUBE_4], true);
				modelStack.PopMatrix();

				//Left Lower pants(brown_torus)
				modelStack.PushMatrix();
				modelStack.Translate(-0.2, -0.9, 0);
				modelStack.Scale(0.13, 1, 0.23);
				RenderMesh(meshList[GEO_TORUS], true);
				modelStack.PopMatrix();

				//Right Lower pants(brown_torus)
				modelStack.PushMatrix();
				modelStack.Translate(0.2, -0.9, 0);
				modelStack.Scale(0.13, 1, 0.23);
				RenderMesh(meshList[GEO_TORUS], true);
				modelStack.PopMatrix();
				
				//Right_Legs (yellow_cylinder)
				modelStack.PushMatrix();
				modelStack.Translate(0.2, -1.5, 0);
				modelStack.Scale(0.07, 3, 0.17);
					//Right_Shoes (black_cube)
					modelStack.PushMatrix();
					modelStack.Translate(0, -0.6, 1);
					modelStack.Scale(3.8, 0.25, 4.5);
					RenderMesh(meshList[GEO_CUBE_4], true);
					modelStack.PopMatrix();	
				RenderMesh(meshList[GEO_CYLINDER_1], true);
				modelStack.PopMatrix();
				
				//Left_Legs (yellow_cylinder)
				modelStack.PushMatrix();
				modelStack.Translate(-0.2, -1.5, 0);
				modelStack.Scale(0.07, 3, 0.17);
					//Left_Shoes (black_cube)
					modelStack.PushMatrix();
					modelStack.Translate(0, -0.6, 1);
					modelStack.Scale(3.8, 0.25, 4.5);
					RenderMesh(meshList[GEO_CUBE_4], true);
					modelStack.PopMatrix();
				RenderMesh(meshList[GEO_CYLINDER_1], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_CUBE_3], true);
			modelStack.PopMatrix();
			
			//Right_Sleeves (white_cone)
			modelStack.PushMatrix();
			modelStack.Translate(2.35, 4.8, 0);
			modelStack.Scale(0.8, 0.5, 0.5);
			//initial rotation
			//modelStack.Rotate(-90, 0, 0, 1);
			//change angle to wave 0,0,y(60-20)
			modelStack.Rotate(hand_rotation, 0, 0, 1);
				//Right_arms (yello_cylinder)
				modelStack.PushMatrix();
				modelStack.Rotate(-60, 0, 0, 1);
				modelStack.Translate(0.5, 1.5, 0);
				modelStack.Scale(0.5, 3, 0.5);	
					//Right_hands (yellow_sphere)
					modelStack.PushMatrix();
					modelStack.Translate(0.2, 0.7, 0);
					modelStack.Scale(1.3, 0.3, 1.5);
					RenderMesh(meshList[GEO_SPHERE_3], true);
					modelStack.PopMatrix();
				RenderMesh(meshList[GEO_CYLINDER_1], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_CONE_1], true);
			modelStack.PopMatrix();
			//Left_Sleeves (white_cone)
			modelStack.PushMatrix();
			modelStack.Translate(-2.35, 4.8, 0);
			modelStack.Scale(0.8, 0.5, 0.5);
			modelStack.Rotate(90, 0, 0, 1);
				//Right_arms (yello_cylinder)
				modelStack.PushMatrix();
				modelStack.Rotate(60, 0, 0, 1);
				modelStack.Translate(-0.5, 1.5, 0);
				modelStack.Scale(0.5, 3, 0.5);
					//Right_hands (yellow_sphere)
					modelStack.PushMatrix();
					modelStack.Translate(-0.2, 0.7, 0);
					modelStack.Scale(1.3, 0.3, 1.5);
					RenderMesh(meshList[GEO_SPHERE_3], true);
					modelStack.PopMatrix();
				RenderMesh(meshList[GEO_CYLINDER_1], true);
				modelStack.PopMatrix();
			RenderMesh(meshList[GEO_CONE_1], true);
			modelStack.PopMatrix();
			
		//Body (Cube)
		modelStack.Translate(0, 6, 0);
		modelStack.Scale(4, 4, 2);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();
		//pan
		
		//flip the pan

		//flip patty

		//render axe
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PushMatrix();
		modelStack.Translate(light[0].position.x, light[0].position.y,
			light[0].position.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false);

		modelStack.PopMatrix();
		modelStack.PushMatrix();
		//to do: transformation code here
		modelStack.Translate(0, 0.25, 0);
		modelStack.Scale(50, 50, 50);
		modelStack.Rotate(270, 1, 0, 0);
		RenderMesh(meshList[GEO_QUAD], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Translate(-8, 6, 7);
		modelStack.Scale(3, 2, 3);
			modelStack.PushMatrix();
			modelStack.Translate(0, -2, 0);
			modelStack.Scale(0.3, 3, 0.3);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();	
		RenderMesh(meshList[GEO_CONE_1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(8, 6, 7);
		modelStack.Scale(3, 2, 3);
			modelStack.PushMatrix();
			modelStack.Translate(0, -2, 0);
			modelStack.Scale(0.3, 3, 0.3);
			RenderMesh(meshList[GEO_CYLINDER], true);
			modelStack.PopMatrix();
		RenderMesh(meshList[GEO_CONE_1], true);
		modelStack.PopMatrix();
		
}
void Assignment1::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = projectionStack.Top() * viewStack.Top() *
		modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE,
		&modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose =
			modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1,
			GL_FALSE, &modelView_inverse_transpose.a[0]);
		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1,
			&mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1,
			&mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1,
			&mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS],
			mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	mesh->Render();

}
void Assignment1::Exit()
{
	// Cleanup VBO here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
		{
			delete meshList[i];
		}
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
