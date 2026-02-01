// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <vector>
#include <cstdio>

GLuint modelVboId;
GLuint modelIboId;
GLuint lineVboId;
int numIndices;

Shaders myShaders;
Shaders myShaders2;
Shaders modelShader;
float angle;
float step = 0.5f;
float totalTime = 0.0f;
Camera myCamera;

void ReadNfg(const char* filename, std::vector<Vertex>& vertices, std::vector<unsigned short>& indices)
{
	FILE* f = fopen(filename, "r");

	int numVertices = 0;
	fscanf(f, "NrVertices: %d", &numVertices);

	for (int i = 0; i < numVertices; ++i)
	{
		Vertex v;
		int index;

		fscanf(f, " %d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
			&index,
			&v.pos.x, &v.pos.y, &v.pos.z,
			&v.norm.x, &v.norm.y, &v.norm.z,
			&v.binorm.x, &v.binorm.y, &v.binorm.z,
			&v.tgt.x, &v.tgt.y, &v.tgt.z,
			&v.uv.x, &v.uv.y
		);

		v.color = Vector3(1.0f, 1.0f, 1.0f);

		vertices.push_back(v);
	}

	int numTriangles = 0;
	fscanf(f, " NrIndices: %d", &numTriangles);
	numTriangles /= 3;

	for (int i = 0; i < numTriangles; ++i)
	{
		int index;
		unsigned short idx1, idx2, idx3;

		fscanf(f, " %d. %hu, %hu, %hu", &index, &idx1, &idx2, &idx3);

		indices.push_back(idx1);
		indices.push_back(idx2);
		indices.push_back(idx3);
	}

	fclose(f);
}

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	ReadNfg("../../NewResourcesPacket/Models/Croco.nfg", vertices, indices);

	numIndices = indices.size();

	glGenBuffers(1, &modelVboId);
	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &modelIboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return modelShader.Init("../Resources/Shaders/modelShaderVS.vs", "../Resources/Shaders/modelShaderFS.fs");
}

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(modelShader.program);

	glBindBuffer(GL_ARRAY_BUFFER, modelVboId);
	
	Matrix mModel;
	mModel.SetRotationY(angle);

	Matrix mvp = mModel * myCamera.viewMatrix * myCamera.perspectiveMatrix;

	if (modelShader.mvpUniform != -1) {
		glUniformMatrix4fv(modelShader.mvpUniform, 1, GL_FALSE, (float*)(mvp.m));
	}

	if (modelShader.positionAttribute != -1) {
		glEnableVertexAttribArray(modelShader.positionAttribute);
		glVertexAttribPointer(modelShader.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelIboId);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	
	if (modelShader.positionAttribute != -1) {
		glDisableVertexAttribArray(modelShader.positionAttribute);
	}																															
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update ( ESContext *esContext, float deltaTime )
{
	totalTime += deltaTime;
	if (totalTime > Globals::frameTime) {
		myCamera.setDeltaTime(Globals::frameTime);
		totalTime -= Globals::frameTime;
		//angle += step;
		//if(angle >= 3.14 * 2)
			//angle -= 3.14 * 2;
	}

}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	if (!bIsPressed)
		return;

	switch (key)
	{
	case 'W': case 'w':
		myCamera.moveOz(-1);   // inainte
		break;

	case 'S': case 's':
		myCamera.moveOz(1);    // inapoi
		break;

	case 'A': case 'a':
		myCamera.moveOx(-1);   // stanga
		break;

	case 'D': case 'd':
		myCamera.moveOx(1);    // dreapta
		break;

	case 'Q': case 'q':
		myCamera.moveOy(1);    // sus
		break;

	case 'E': case 'e':
		myCamera.moveOy(-1);   // jos
		break;

	case VK_UP:
		myCamera.rotateOx(-1);
		break;

	case VK_DOWN:
		myCamera.rotateOx(1);
		break;

	case VK_LEFT:
		myCamera.rotateOy(-1);
		break;

	case VK_RIGHT:
		myCamera.rotateOy(1);
		break;

	case 'Z': case 'z':
		myCamera.rotateOz(-1);
		break;

	case 'X': case 'x':
		myCamera.rotateOz(1);
		break;
	}
}

void Mouse(ESContext* esContext, MouseButtons btn, MouseEvents event, int x, int y) 
{
	switch (btn)
	{
		case MOUSE_LEFT:
		if (event == BTN_DOWN && x < Globals::screenWidth / 2) {
			myCamera.rotateOx(-1);
		}
		else if (event == BTN_DOWN && x > Globals::screenWidth / 2){
			myCamera.rotateOx(1);
		}
		else if (event == BTN_UP && x < Globals::screenWidth / 2) {
			myCamera.rotateOx(-1);
		}
		else if (event == BTN_UP && x > Globals::screenWidth / 2){
			myCamera.rotateOx(1);
		}
		break;
		case MOUSE_RIGHT:
			if (event == BTN_DOWN && x < Globals::screenWidth / 2) {
				myCamera.rotateOy(-1);
			}
			else if (event == BTN_DOWN && x > Globals::screenWidth / 2) {
				myCamera.rotateOy(1);
			}
			else if (event == BTN_UP && x < Globals::screenWidth / 2) {
				myCamera.rotateOy(-1);
			}
			else if (event == BTN_UP && x > Globals::screenWidth / 2) {
				myCamera.rotateOy(1);
			}
		break;
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &modelVboId);
	glDeleteBuffers(1, &modelIboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();

	
	return 0;
}

