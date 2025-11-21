// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"

GLuint vboId;
GLuint lineVboId ;

Shaders myShaders;
Shaders myShaders2;
float angle;
float step = 0.5f;
float totalTime = 0.0f;
Camera myCamera;

int Init ( ESContext *esContext )
{
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );

	//triangle data (heap)
	Vertex verticesData[6];
	Vertex lineVerticesData[2];

	verticesData[0].pos.x =  -0.5f;  verticesData[0].pos.y =  0.5f;  verticesData[0].pos.z =  0.0f;
	verticesData[1].pos.x = -0.5f;  verticesData[1].pos.y = -0.5f;  verticesData[1].pos.z =  0.0f;
	verticesData[2].pos.x =  0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z =  0.0f;

	verticesData[0].color.x = 1.0f; verticesData[0].color.y = 0.0f; verticesData[0].color.z = 0.0f;
	verticesData[1].color.x = 0.0f; verticesData[1].color.y = 1.0f; verticesData[1].color.z = 0.0f;
	verticesData[2].color.x = 0.0f; verticesData[2].color.y = 0.0f; verticesData[2].color.z = 1.0f;

	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = 0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[4].pos.x = -0.5f;  verticesData[4].pos.y = 0.5f;  verticesData[4].pos.z = 0.0f;
	verticesData[5].pos.x = 0.5f;  verticesData[5].pos.y = -0.5f;  verticesData[5].pos.z = 0.0f;

	verticesData[3].color.x = 0.0f; verticesData[3].color.y = 1.0f; verticesData[3].color.z = 0.0f;
	verticesData[4].color.x = 1.0f; verticesData[4].color.y = 0.0f; verticesData[4].color.z = 0.0f;
	verticesData[5].color.x = 0.0f; verticesData[5].color.y = 0.0f; verticesData[5].color.z = 1.0f;

	lineVerticesData[0].pos.x = 0.0f;  lineVerticesData[0].pos.y = 1.0f;  lineVerticesData[0].pos.z = 0.0f;
	lineVerticesData[1].pos.x = 0.0f;  lineVerticesData[1].pos.y = -1.0f;  lineVerticesData[1].pos.z = 0.0f;

	lineVerticesData[0].color.x = 1.0f; lineVerticesData[0].color.y = 1.0f; lineVerticesData[0].color.z = 1.0f;
	lineVerticesData[1].color.x = 1.0f; lineVerticesData[1].color.y = 1.0f; lineVerticesData[1].color.z = 1.0f;

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lineVboId );
	glBindBuffer(GL_ARRAY_BUFFER, lineVboId );
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVerticesData), lineVerticesData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//creation of shaders and program 
	return myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs") || myShaders2.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs");

}

void Draw ( ESContext *esContext )
{

	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);

	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	Matrix mRotation = myCamera.viewMatrix * myCamera.perspectiveMatrix;
	if(myShaders.colorAttribute != -1) {
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}
	
	if(myShaders.matrixUniform != -1) {
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)(mRotation.m));
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glUseProgram(myShaders2.program);

	glBindBuffer(GL_ARRAY_BUFFER, lineVboId);

	if (myShaders2.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders2.positionAttribute);
		glVertexAttribPointer(myShaders2.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (myShaders2.colorAttribute != -1) {
		glEnableVertexAttribArray(myShaders2.colorAttribute);
		glVertexAttribPointer(myShaders2.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	}

	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
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
	glDeleteBuffers(1, &vboId);
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

