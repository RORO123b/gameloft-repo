// NewTrainingFramework.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "../Utilities/utilities.h" 
#include "../Utilities/TGA.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <vector>
#include <cstdio>
#include "SceneObject.h"
#include "ResourceManager.h"
#include "SceneManager.h"

GLuint vboId, vbold, iboId, modelVboId, modelIboId;
GLuint textureId;
Shader myShaders, lineShader, modelShader;
float totalTime = 0.0f;
const void* ptr_iboId;
GLsizei indexCount;
ResourceManager* rm;
SceneManager* sm;

Camera* myCamera;

int Init(ESContext* esContext)
{
	rm = ResourceManager::getInstance();
	rm->Init();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	sm = SceneManager::getInstance();
	sm->Init();
	myCamera = sm->getActiveCamera();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}
void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sm->Draw(esContext);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, float deltaTime)
{
	sm->Update(deltaTime);
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	if (!bIsPressed)
		return;

	switch (key)
	{
	case 'W': case 'w':
		myCamera->moveOz(-1);   // inainte
		break;

	case 'S': case 's':
		myCamera->moveOz(1);    // inapoi
		break;

	case 'A': case 'a':
		myCamera->moveOx(-1);   // stanga
		break;

	case 'D': case 'd':
		myCamera->moveOx(1);    // dreapta
		break;

	case 'Q': case 'q':
		myCamera->moveOy(1);    // sus
		break;

	case 'E': case 'e':
		myCamera->moveOy(-1);   // jos
		break;

	case VK_UP:
		myCamera->rotateOx(1);
		break;

	case VK_DOWN:
		myCamera->rotateOx(-1);
		break;

	case VK_LEFT:
		myCamera->rotateOy(1);
		break;

	case VK_RIGHT:
		myCamera->rotateOy(-1);
		break;

	case 'Z': case 'z':
		myCamera->rotateOz(-1);
		break;

	case 'X': case 'x':
		myCamera->rotateOz(1);
		break;
	}
}

void Mouse(ESContext* esContext, MouseButtons btn, MouseEvents event, int x, int y) 
{
	switch (btn)
	{
		case MOUSE_LEFT:
		if (event == BTN_DOWN && x < Globals::screenWidth / 2) {
			myCamera->rotateOx(-1);
		}
		else if (event == BTN_DOWN && x > Globals::screenWidth / 2){
			myCamera->rotateOx(1);
		}
		else if (event == BTN_UP && x < Globals::screenWidth / 2) {
			myCamera->rotateOx(-1);
		}
		else if (event == BTN_UP && x > Globals::screenWidth / 2){
			myCamera->rotateOx(1);
		}
		break;
		case MOUSE_RIGHT:
			if (event == BTN_DOWN && x < Globals::screenWidth / 2) {
				myCamera->rotateOy(-1);
			}
			else if (event == BTN_DOWN && x > Globals::screenWidth / 2) {
				myCamera->rotateOy(1);
			}
			else if (event == BTN_UP && x < Globals::screenWidth / 2) {
				myCamera->rotateOy(-1);
			}
			else if (event == BTN_UP && x > Globals::screenWidth / 2) {
				myCamera->rotateOy(1);
			}
		break;
	}
}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &vbold);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Game Engine", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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

