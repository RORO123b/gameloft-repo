#pragma once
#include "Math.h"
#include "../Utilities/utilities.h"

class Camera
{
public:
    Vector3 position;
    Vector3 target;
    Vector3 up;

	GLfloat deltaTime;
    GLfloat fov;
    GLfloat nearPlane;
    GLfloat farPlane;
    GLfloat moveSpeed;
    GLfloat rotateSpeed;

    Vector3 xAxis;
    Vector3 yAxis;
    Vector3 zAxis;

    Matrix worldMatrix;
    Matrix viewMatrix;
	Matrix perspectiveMatrix;

    Camera();
	Camera(Vector3 pos, Vector3 targ, Vector3 upDir, GLfloat fov, GLfloat nearP, GLfloat farP, GLfloat deltaTime, GLfloat moveSpeed, GLfloat rotateSpeed);
    ~Camera();

    void moveOx(int sens);
    void moveOy(int sens);
    void moveOz(int sens);
    void rotateOx(int sens);
    void rotateOy(int sens);
    void rotateOz(int sens);
    void updateAxes();
    void updateWorldView();
    void setDeltaTime(GLfloat);

    
};
