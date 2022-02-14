#include "Scene1.h"
#include "GL\glew.h"

#include "shader.hpp"


Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
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

	//Load vertex and fragment shaders
	m_programID = LoadShaders(
		"Shader//SimpleVertexShader.vertexshader",
		"Shader//SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(m_programID);
}

void Scene1::Update(double dt)
{
}

void Scene1::Render()
{
	// Render VBO here
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);//1st attributebuffer: vertices
	glEnableVertexAttribArray(1); // 2nd attribute buffer : colors

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

void Scene1::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
