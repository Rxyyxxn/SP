#include "Scene2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Init VBO here
	//Set background colour buffer every frames
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a defult VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Generate Buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	static const GLfloat vertex_buffer_data[] = {
	-0.7f, 0.3f, 0.0f,
	-0.6f, 0.0f, 0.0f,
	-0.3f, 0.3f, 0.0f,
	-0.3f, 0.0f, 0.0f,
	-0.1f, 0.3f, 0.0f,
	0.0f, 0.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data[] = {
	2.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 2.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	0.0f, 4.0f, 0.0f,
	0.0f, 5.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);


	static const GLfloat vertex_buffer_data_2[] = {
	0.7f, 0.3f, 0.0f,
	0.6f, 0.0f, 0.0f,
	0.3f, 0.3f, 0.0f,
	0.3f, 0.0f, 0.0f,
	0.1f, 0.3f, 0.0f,
	0.0f, 0.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_2), vertex_buffer_data_2, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_2[] = {
	2.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 2.0f, 0.0f,
	0.0f, 3.0f, 0.0f,
	0.0f, 5.0f, 0.0f,
	0.0f, 5.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_2),
		color_buffer_data_2, GL_STATIC_DRAW);


	static const GLfloat vertex_buffer_data_3[] = {
	-0.7f, -0.5f, 0.0f,
	-0.6f, -0.2f, 0.0f,
	-0.3f, -0.5f, 0.0f,
	-0.3f, -0.2f, 0.0f,
	-0.1f, -0.5f, 0.0f,
	0.0f, -0.2f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_3), vertex_buffer_data_3, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_3[] = {
	2.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 2.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 4.0f, 0.0f,
	0.0f, 5.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_3),
		color_buffer_data_3, GL_STATIC_DRAW);


	static const GLfloat vertex_buffer_data_4[] = {
	0.7f, -0.5f, 0.0f,
	0.6f, -0.2f, 0.0f,
	0.3f, -0.5f, 0.0f,
	0.3f, -0.2f, 0.0f,
	0.1f, -0.5f, 0.0f,
	0.0f, -0.2f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_4), vertex_buffer_data_4, GL_STATIC_DRAW);
	static const GLfloat color_buffer_data_4[] = {
	2.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 2.0f, 0.0f,
	0.0f, 2.0f, 0.0f,
	0.0f, 4.0f, 0.0f,
	0.0f, 5.0f, 0.0f,
	};
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_4),
		color_buffer_data_4, GL_STATIC_DRAW);

	/*m_programID = LoadShaders(
		"Shader//SimpleVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");*/


	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID,
		"MVP");

	rotateAngle = 5;
	translateX = 1;
	scaleAll = 2;

	// Use our shader
	glUseProgram(m_programID);
}

void Scene2::Update(double dt)
{
	rotateAngle += 10 * dt;
	translateX += 10 * dt;
	scaleAll += 2 * dt;
}

void Scene2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	//1
	scale.SetToScale(15, 15, 15);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle
	//2
	scale.SetToScale(10, 10, 10);
	rotate.SetToRotation(45, 0, 0, 1);
	translate.SetToTranslation(translateX, 20, 0);
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle
	//3
	scale.SetToScale(translateX, 18, 10);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(35, 25, 0);
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);//starting from vertex 0;3 vertices=1 triangle

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
