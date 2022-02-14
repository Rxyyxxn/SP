#include "Practical12.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "../Utility.h"
#include "LoadTGA.h"
#include <sstream>

Practical12::Practical12()
{
}

Practical12::~Practical12()
{
}

void Practical12::Init()
{	

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2500.f);
	projectionStack.LoadMatrix(projection);

	blightenable=true;
	total_chest = 4;

	// Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);

	//Enable back face culling
	glEnable(GL_CULL_FACE);


	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	// Get a handle for our "textColor" uniform
	m_programID = LoadShaders("Shader//Texture.vertexshader","Shader//Text.fragmentshader");
	m_parameters[U_COLOR_TEXTURE_ENABLED] =glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] =glGetUniformLocation(m_programID, "colorTexture");
	m_parameters[U_TEXT_ENABLED] =glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID,"textColor");

	

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

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(0, 10, 0);
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
	rotateAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

	// Init VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = nullptr;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(0, 1, 1), 20, 40);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("sphere", Color(0, 1, 1), 20, 40);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_QUAD]->textureID = LoadTGA("Image//Sand.tga");
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//color.tga");
	meshList[GEO_QUAD]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_QUAD]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_QUAD]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_QUAD]->material.kShininess = 1.0f;

	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 1.f);
	meshList[GEO_CUBE]->textureID = LoadTGA("Image//color.tga");
	meshList[GEO_CUBE]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CUBE]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CUBE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_CUBE]->material.kShininess = 1.0f;

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front",Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_BLEND] = MeshBuilder::GenerateQuad("blend", Color(1, 1, 1), 1.f);
	meshList[GEO_BLEND]->textureID = LoadTGA("Image//NYP.tga"); 

	meshList[GEO_MODEL4] = MeshBuilder::GenerateOBJMTL("model7","OBJ//patrick.obj", "OBJ//patrick.mtl"); 
	meshList[GEO_MODEL4]->textureID =LoadTGA("Image//Char_Patrick.tga");

	//spongebob text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("spongebob", 16,16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_MODEL1] = MeshBuilder::GenerateOBJMTL("model1","OBJ//longpalm.obj", "OBJ//longpalm.mtl");
	meshList[GEO_MODEL2] = MeshBuilder::GenerateOBJMTL("model2", "OBJ//chest.obj", "OBJ//chest.mtl");
	meshList[GEO_MODEL3] = MeshBuilder::GenerateOBJMTL("model3", "OBJ//largestone.obj", "OBJ//largestone.mtl");
	meshList[GEO_MODEL5] = MeshBuilder::GenerateOBJMTL("model4", "OBJ//shipdark.obj", "OBJ//shipdark.mtl");
	meshList[GEO_MODEL6] = MeshBuilder::GenerateOBJMTL("model5", "OBJ//tower.obj", "OBJ//tower.mtl");
	meshList[GEO_MODEL7] = MeshBuilder::GenerateOBJMTL("model6", "OBJ//largerock.obj", "OBJ//largerock.mtl");




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
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("holes", Color(0.35f, 0.33f, 0.27f), 10, 1.f, 1.f);
	meshList[GEO_CYLINDER_1] = MeshBuilder::GenerateCylinder("legs", Color(0.92, 1, 0.07), 10, 1.f, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.90f, 0.15f, 0.07f), 10, 1.f, 1.f);
	meshList[GEO_CONE_1] = MeshBuilder::GenerateCone("cone", Color(1, 1, 1), 10, 1.f, 1.f);
	meshList[GEO_TORUS] = MeshBuilder::GenerateTorus("torus", Color(0.63, 0.42, 0.07), 10, 20, 1.f, 0.5f);

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT],
		m_parameters[U_MATERIAL_DIFFUSE],
		m_parameters[U_MATERIAL_SPECULAR],
		m_parameters[U_MATERIAL_SHININESS]);
}

void Practical12::Update(double dt)
{
	Vector3 spongebob = model_pos - camera.position;
	float distance = sqrt(pow(spongebob.x, 2) + pow(spongebob.y, 2) + pow(spongebob.z, 2));
	Vector3 chest_1 = chest1_pos - camera.position;
	chest1_distance = sqrt(pow(chest_1.x, 2) + pow(chest_1.y, 2) + pow(chest_1.z, 2));
	Vector3 chest_2 = chest2_pos - camera.position;
	chest2_distance = sqrt(pow(chest_2.x, 2) + pow(chest_2.y, 2) + pow(chest_2.z, 2));
	Vector3 chest_3 = chest3_pos - camera.position;
	chest3_distance = sqrt(pow(chest_3.x, 2) + pow(chest_3.y, 2) + pow(chest_3.z, 2));
	Vector3 patrick = patrick_pos - camera.position;
	patrick_distance = sqrt(pow(patrick.x, 2) + pow(patrick.y, 2) + pow(patrick.z, 2));

	//Mouse Inputs
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
		
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Converting Viewport space to UI space
		double x, y;
		Application::GetCursorPos(&x, &y);
		unsigned w = Application::GetWindowWidth();
		unsigned h = Application::GetWindowHeight();
		float posX = x/w*80; //convert (0,800) to (0,80)
		float posY = 60-(y/h*60); //convert (600,0) to (0,60)
		std::cout << "posX:" << posX << " , posY:" << posY << std::endl;
		//if (posX > BUTTON_LEFT && posX < BUTTON_RIGHT && posY > BUTTON_BOTTOM && posY < BUTTON_TOP)
		//{
		//	std::cout << "Hit!" << std::endl;
		//	//trigger user action or function
		//}
		//else
		//{
		//	std::cout << "Miss!" << std::endl;
		//}
	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	float LSPEED=10.f;

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
	if (Application::IsKeyPressed('9'))
	{
		blightenable = true;
	}
	if (Application::IsKeyPressed('0'))
	{
		blightenable = false;
	}
	static bool talk = false;
	static int count = 0;
	if ( Application::IsKeyPressed('F'))
	{
		talk = true;
	}
	else if(!Application::IsKeyPressed('F'))
	{
		talk = false;
	}
	static bool pat_talk = false;
	static int pat_count = 0;
	if (Application::IsKeyPressed('Q'))
	{
		pat_talk = true;
	}
	else if (!Application::IsKeyPressed('Q'))
	{
		pat_talk = false;
	}
	//std::cout << talk << std::endl;
	

	if (distance <= 25)
	{
		if (talk)
		{
			count++;
		}
		if (count>=1)
		{
			dialog = "Collect 3 chest to continue";
			total_chest = 3;
			tc.str("");
			tc << total_chest <<" Chest left to collect";

			count++;
			chest1_have = 0;
		}

		else
		{
			dialog = "Press F to talk";
		}
	}
	else
	{
		//count = 0;
		dialog = "";
	}
	
	if (chest1_distance <= 25)
	{
		if (Application::IsKeyPressed('E')&&chest1_have==0)
		{
			total_chest -= 1;
			tc.str("");
			tc << total_chest << " Chest left to collect";
			chest1_have ++;
		}
		else
		{
			chest_1_dia = "Press E to collect chest";
		}
	}
	else
	{
		chest_1_dia = "";
	}
	if (chest2_distance <= 25)
	{
		if (Application::IsKeyPressed('E') && chest2_have == 0)
		{
			total_chest -= 1;
			tc.str("");
			tc << total_chest << " Chest left to collect";
			chest2_have++;
		}
		else
		{
			chest_2_dia = "Press E to collect chest";
		}
	}
	else
	{
		chest_2_dia = "";
	}
	if (chest3_distance <= 25)
	{
		if (Application::IsKeyPressed('E') && chest3_have == 0)
		{
			total_chest -= 1;
			tc.str("");
			tc << total_chest << " Chest left to collect";
			chest3_have++;
		}
		else
		{
			chest_3_dia = "Press E to collect chest";
		}
	}
	else
	{
		chest_3_dia = "";
	}
	if (total_chest==4)
	{
		tc.str("");
		tc << "Talk to spongebob";
	}
	else if (total_chest==0)
	{
		tc.str("");
		tc <<"Talk to patrick";
	}
	
	if (patrick_distance <= 25)
	{
		if (total_chest == 0)
		{
			if (pat_talk)
			{
				pat_count++;
			}
			if (pat_count >= 1)
			{
				pat_dia = "Thank you for the chest";
				tc.str("");
				tc << "Chest given to patrick";

				pat_count++;
			}

			else
			{
				pat_dia = "Press Q to talk";
			}
		}
	}
	rotateAngle += (float)(10 * dt);
	camera.Update(dt);
}

void Practical12::Render()
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
		RenderSkybox();
		//patrick
		modelStack.PushMatrix();
		//modelStack.Translate(50, 0, -55);
		modelStack.Translate(patrick_pos.x, patrick_pos.y, patrick_pos.z);
			//chest text 2
			modelStack.PushMatrix();
			modelStack.Translate(5, 7, 0);
			modelStack.Rotate(0, 0, 1, 0);
			modelStack.Scale(0.8, 0.8, 0.8);
			RenderText(meshList[GEO_TEXT], pat_dia, Color(1, 1, 0));
			modelStack.PopMatrix();
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_MODEL4], true);
		modelStack.PopMatrix();

		//palm tree
		modelStack.PushMatrix();
		modelStack.Translate(-10, -1, -10);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_MODEL1], true);
		modelStack.PopMatrix();

		//palm tree
		modelStack.PushMatrix();
		modelStack.Translate(20, -1, -10);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMesh(meshList[GEO_MODEL1], true);
		modelStack.PopMatrix();

		if (chest1_have == 0)
		{
			//chest
			modelStack.PushMatrix();
			modelStack.Translate(chest1_pos.x, chest1_pos.y, chest1_pos.z);
			//modelStack.Translate(20, 5, 5);
			modelStack.Rotate(90, 0, 1, 0);
				//chest text 1
				modelStack.PushMatrix();
				modelStack.Translate(5, 5, 0);
				modelStack.Rotate(180, 0, 1, 0);
				modelStack.Scale(0.8, 0.8, 0.8);
				RenderText(meshList[GEO_TEXT], chest_1_dia, Color(1, 1, 0));
				modelStack.PopMatrix();
			modelStack.Scale(0.5, 0.5, 0.5);
			RenderMesh(meshList[GEO_MODEL2], true);
			modelStack.PopMatrix();
		}
		if (chest2_have==0)
		{
			//chest
			modelStack.PushMatrix();
			modelStack.Translate(chest2_pos.x, chest2_pos.y, chest2_pos.z);
			//modelStack.Translate(-20, -1, 5);
			modelStack.Rotate(-90, 0, 1, 0);
				//chest text 2
				modelStack.PushMatrix();
				modelStack.Translate(5, 5, 0);
				modelStack.Rotate(180, 0, 1, 0);
				modelStack.Scale(0.8, 0.8, 0.8);
				RenderText(meshList[GEO_TEXT], chest_2_dia, Color(1, 1, 0));
				modelStack.PopMatrix();
			modelStack.Scale(0.5, 0.5, 0.5);
			RenderMesh(meshList[GEO_MODEL2], true);
			modelStack.PopMatrix();

		}
		if (chest3_have == 0)
		{
			//chest
			modelStack.PushMatrix();
			modelStack.Translate(chest3_pos.x, chest3_pos.y, chest3_pos.z);
			//modelStack.Translate(-20, -1, 5);
			modelStack.Rotate(-90, 0, 1, 0);
				//chest text 3
				modelStack.PushMatrix();
				modelStack.Translate(5, 5, 0);
				modelStack.Rotate(180, 0, 1, 0);
				modelStack.Scale(0.8, 0.8, 0.8);
				RenderText(meshList[GEO_TEXT], chest_3_dia, Color(1, 1, 0));
				modelStack.PopMatrix();
			modelStack.Scale(0.5, 0.5, 0.5);
			RenderMesh(meshList[GEO_MODEL2], true);
			modelStack.PopMatrix();

		}


		modelStack.PushMatrix();
		modelStack.Translate(0, -1, 0);
		modelStack.Rotate(-90, 1 , 0 ,0);
		modelStack.Scale(180, 180, 180);
		RenderMesh(meshList[GEO_QUAD], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(model_pos.x,model_pos.y,model_pos.z);
		modelStack.Rotate(1, 1, 0, 0);
		modelStack.Translate(0, 0, 0);
			//spongebob text
			modelStack.PushMatrix();
			modelStack.Translate(-5, 10, 0);
			modelStack.Scale(0.8, 0.8, 0.8);
			RenderText(meshList[GEO_TEXT], dialog, Color(1, 1, 0));
			modelStack.PopMatrix();
			
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
			modelStack.Rotate(-90, 0, 0, 1);
				//Right_arms (yello_cylindar)
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

		modelStack.PushMatrix();
		modelStack.Translate(-30, -1,-80);
		modelStack.Rotate(0, 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_MODEL3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(60, -1, -90);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_MODEL5], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-50, -1, -20);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_MODEL6], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(60, -1, -20);
		modelStack.Rotate(170, 0, 1, 0);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_MODEL6], true);
		modelStack.PopMatrix();

		//No transform needed
		RenderTextOnScreen(meshList[GEO_TEXT], tc.str(),Color(0, 1, 0), 3, 0, 0);

		//No transform needed	
		RenderMeshOnScreen(meshList[GEO_QUAD], 30, 30, 20, 20);
}
void Practical12::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
void Practical12::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	//glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}
void Practical12::RenderTextOnScreen(Mesh* mesh, std::string text,Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		//Change this line inside for loop
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}

void Practical12::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}


void Practical12::RenderSkybox()
{
	const float OFFSET = 499;
	modelStack.PushMatrix();
	modelStack.Translate(0, -4, -OFFSET);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 2, OFFSET);
	modelStack.Rotate(180,0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-OFFSET, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(OFFSET, -4, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, OFFSET,-2);
	//modelStack.Rotate(1, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(1005, 1005, 1005);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -OFFSET, 0);
	//modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1010, 1010, 1010);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}
void Practical12::Exit()
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
