#include "Scene3.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"

bool Scaled,bgchange = false;
Scene3::Scene3()
{
}

Scene3::~Scene3()
{

}
void Scene3::Update(double dt)
{
	rotateAngle += 30 * dt;
	translateX += 10 * dt;
	bird_translate += 10 * dt;
	if (scaleAll >= 2)
	{
		Scaled = true;
	}
	else if (scaleAll <= 1)
	{
		Scaled = false;
	}

	if (Scaled == true)
	{
		scaleAll -= 1 * dt;
	}
	else if (Scaled == false)
	{
		scaleAll += 1 * dt;
	}

	if (bgcolour >= 0.3)
	{
		bgchange = true;
	}
	else if (bgcolour <= 0)
	{
		bgchange = false;
	}

	if (bgchange == true)
	{
		bgcolour -= 0.1 * dt;
	}
	else if (bgchange == false)
	{
		bgcolour += 0.1 * dt;
	}

}

void Scene3::Init()
{
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Init VBO here
	
	//Generate a defult VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Generate Buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	//rectangle
	static const GLfloat vertex_buffer_data[] = {
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data[] = {
	0.24f, 0.43f, 0.25f,
	0.24f, 0.43f, 0.25f,
	0.24f, 0.43f, 0.25f,
	0.24f, 0.43f, 0.25f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);

//--------------------------------------------------------------------
	//moon
	static const GLfloat vertex_buffer_data_2[] = {
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	-0.5f,0.0f,0.0f
	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_2), vertex_buffer_data_2, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_2[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_2),
		color_buffer_data_2, GL_STATIC_DRAW);

	//--------------------------------------------------------------------
	//triangle
	static const GLfloat vertex_buffer_data_3[] = {
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_3), vertex_buffer_data_3, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_3[] = {
	0.23f, 0.25f, 0.23f,
	0.66f, 0.74f, 0.74f,
	0.66f, 0.74f, 0.74f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_3),
		color_buffer_data_3, GL_STATIC_DRAW);
	//0.23f, 0.25f, 0.23f-grayish blue
	//0.66f, 0.74f, 0.74f-whitish grey
	//--------------------------------------------------------------------
	//pentagon
	static const GLfloat vertex_buffer_data_4[] = {
		-0.2f,-0.2f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.2f,-0.2f,0.0f,
		0.0f,0.0f,0.0f,
		0.2f,-0.2f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		0.2f,0.2f,0.0f,
		0.0f,0.0f,0.0f,
		0.2f,0.2f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		-0.2,0.2f,0.0f,
		0.0f,0.0f,0.0f,
		-0.2,0.2f,0.0f,
		-1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-0.2f,-0.2f,0.0f,
		0.0f,0.0f,0.0f

	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_4), vertex_buffer_data_4, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_4[] = {
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f,
	1.0f,1.0f,1.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_4),
		color_buffer_data_4, GL_STATIC_DRAW);

	//--------------------------------------------------------------------
	//House roof
	static const GLfloat vertex_buffer_data_5[] = {
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_5), vertex_buffer_data_5, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_5[] = {
	0.9f, 0.1f, 0.1f,
	0.9f, 0.1f, 0.1f,
	0.9f, 0.1f, 0.1f
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_5),
		color_buffer_data_5, GL_STATIC_DRAW);

	//--------------------------------------------------------------------
	//hse door
	static const GLfloat vertex_buffer_data_6[] = {
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.0f, -0.5f, 0.0f,
	-0.5f,0.0f,0.0f
	};
	//1-255 so RGB / 255
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_6), vertex_buffer_data_6, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_6[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_6),
		color_buffer_data_6, GL_STATIC_DRAW);

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID,
		"MVP");

	rotateAngle = 5;
	translateX = 1;
	scaleAll = 1;
	bgcolour = 0;
	bird_translate = -30;
	// Use our shader
	glUseProgram(m_programID);
}
	


void Scene3::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set background colour buffer every frames
	glClearColor(bgcolour, bgcolour, bgcolour, 0.0f);

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	glEnableVertexAttribArray(0);//1st attributebuffer: vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity(); /*no need camera for now, set it at
	World's origin*/
	projection.SetToOrtho(-40, +40, -30, +30, -10, +10); /*Our world is
	a cube defined by these boundaries*/

	//Ground
	scale.SetToScale(40, 20, 20);
	rotate.SetToRotation(0, 1, 0, 0);
	translate.SetToTranslation(0, -10, 0);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
	&MVP.a[0]); //update the shader with new MVP
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//--------------------------------------------------------------------
	//moon
	scale.SetToScale(10, 10, 10);
	rotate.SetToRotation(rotateAngle, 0, 0, 45);
	translate.SetToTranslation(30, 20, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//--------------------------------------------------------------------
	//mountain_1
	scale.SetToScale(20, 15, 25);
	rotate.SetToRotation(90, 0, 0, 90);
	translate.SetToTranslation(-23, -10, 0);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//mountain_2
	scale.SetToScale(25, 25, 25);
	rotate.SetToRotation(90, 0, 0, 90);
	translate.SetToTranslation(-15, -15, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//mountain_3
	scale.SetToScale(20, 25, 25);
	rotate.SetToRotation(90, 0, 0, 90);
	translate.SetToTranslation(-35, -10, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle
	//--------------------------------------------------------------------
	//Star_1
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(rotateAngle, 0, 0, 5);
	translate.SetToTranslation(-15, 10, 0);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP,0, 21);//starting from vertex 0;3 vertices=1 triangle

	//Star_2
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(rotateAngle, 0, 0, 10);
	translate.SetToTranslation(5, 5, 0);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 21);//starting from vertex 0;3 vertices=1 triangle

	//Star_3
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 20, 0);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 21);//starting from vertex 0;3 vertices=1 triangle
	//--------------------------------------------------------------------
	//House Roof
	scale.SetToScale(15, 15, 25);
	rotate.SetToRotation(90, 0, 0,90);
	translate.SetToTranslation(20, -5, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_5]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle
	
	//--------------------------------------------------------------------
	//hse
	scale.SetToScale(17, 17, 10);
	rotate.SetToRotation(45, 0, 0, 1);
	translate.SetToTranslation(20, -10, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//--------------------------------------------------------------------
	//hse door
	scale.SetToScale(7, 7,3);
	rotate.SetToRotation(45, 0, 0, 1);
	translate.SetToTranslation(20, -13.5f, 2);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_6]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_6]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	//--------------------------------------------------------------------
	//Bird
	scale.SetToScale(10, 2.5, 5);
	rotate.SetToRotation(rotateAngle, 1, 0, 0);
	translate.SetToTranslation(bird_translate, 25, 1);
	model = translate * rotate * scale; /*scale, followed by
	rotate, then lastly translate*/
	MVP = projection * view * model; /* Remember, matrix
	multiplication is the other way around*/
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
		&MVP.a[0]); //update the shader with new MVP

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_5]);
	glVertexAttribPointer(
		0,//attribute 0. must match the layout in the shader. usually 0 is for vertex
		3,//size
		GL_FLOAT,//type
		GL_FALSE,//normalised?
		0,//stride
		0//array buffer offset
	);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_5]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 21);//starting from vertex 0;3 vertices=1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene3::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
