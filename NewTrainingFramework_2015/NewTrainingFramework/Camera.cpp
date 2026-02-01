#include "stdafx.h"
#include "Camera.h"
#include "../Utilities/Math.h"
#include "Globals.h"

Camera::Camera()
{
    position = Vector3(0.0f, 0.0f, 200.0f);
    target = Vector3(0.0f, 0.0f, 0.0f);
    up = Vector3(0.0f, 1.0f, 0.0f);

    fov = 0.785398f;
    nearPlane = 0.2f;
    farPlane = 1000.0f;

    moveSpeed = 100.0f;
    rotateSpeed = 1.0f;

	perspectiveMatrix.SetPerspective(fov, Globals::screenWidth / Globals::screenHeight, nearPlane, farPlane);
    updateAxes();
    updateWorldView();
}

Camera::Camera(Vector3 pos, Vector3 targ, Vector3 upDir, GLfloat fov, GLfloat nearP, GLfloat farP, GLfloat deltaTime, GLfloat moveSpeed, GLfloat rotateSpeed)
{
    position = pos;
    target = targ;
    up = upDir;
    this->fov = fov;
    nearPlane = nearP;
    farPlane = farP;
    this->moveSpeed = moveSpeed;
    this->rotateSpeed = rotateSpeed;
    updateAxes();
	updateWorldView();
}

Camera::~Camera() {}

void Camera::updateAxes()
{
    zAxis = -(target - position).Normalize();
    yAxis = up.Normalize();
    xAxis = zAxis.Cross(yAxis).Normalize();
}

void Camera::updateWorldView()
{
    Matrix R;

    R.m[0][0] = xAxis.x;
    R.m[0][1] = xAxis.y;
    R.m[0][2] = xAxis.z;
    R.m[0][3] = 0;

    R.m[1][0] = yAxis.x;
    R.m[1][1] = yAxis.y;
    R.m[1][2] = yAxis.z;
    R.m[1][3] = 0;

    R.m[2][0] = zAxis.x;
    R.m[2][1] = zAxis.y;
    R.m[2][2] = zAxis.z;
    R.m[2][3] = 0;

    R.m[3][0] = 0;
    R.m[3][1] = 0;
    R.m[3][2] = 0;
    R.m[3][3] = 1;

    Matrix T;
    T.SetTranslation(position);

    worldMatrix = R * T;

    Matrix T1;
    T1.SetTranslation(-position);

    
    viewMatrix = T1 * R.Transpose();
}

void Camera::moveOx(int sens)
{
    Vector3 forward =  Vector3(1.0f, 0.0f, 0.0f) * sens;
    Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
    position += vectorDeplasare;
    target += vectorDeplasare;
    updateAxes();
    updateWorldView();
}

void Camera::moveOy(int sens)
{
    Vector3 forward =  Vector3(0.0f, 1.0f, 0.0f) * sens;
    Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
    position += vectorDeplasare;
    target += vectorDeplasare;
    updateAxes();
    updateWorldView();
}

void Camera::moveOz(int sens)
{
    Vector3 forward =  Vector3(0.0f, 0.0f, 1.0f) * sens;
    Vector3 vectorDeplasare = forward * moveSpeed * deltaTime;
    position += vectorDeplasare;
    target += vectorDeplasare;
    updateAxes();
    updateWorldView();
}

void Camera::rotateOx(int sens)
{
	Matrix mRotateOX = Matrix().SetRotationX(rotateSpeed * deltaTime * sens);
	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 rotatedLocalUp = localUp * mRotateOX;
    up = Vector3(rotatedLocalUp * worldMatrix);
    up = up.Normalize();
    Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
    Vector4 rotatedTarget = localTarget * mRotateOX;
    target = Vector3(rotatedTarget * worldMatrix);
    updateAxes();
    updateWorldView();
}
void Camera::rotateOy(int sens)
{
    Matrix mRotateOY = Matrix().SetRotationY(rotateSpeed * deltaTime * sens);
    Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
    Vector4 rotatedTarget = localTarget * mRotateOY;
    target = Vector3(rotatedTarget * worldMatrix);
    updateAxes();
    updateWorldView();
}
void Camera::rotateOz(int sens)
{
    Matrix mRotateOZ = Matrix().SetRotationZ(rotateSpeed * deltaTime * sens);
    Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 rotatedLocalUp = localUp * mRotateOZ;
    up = Vector3(rotatedLocalUp * worldMatrix);
    up = up.Normalize();
    Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
    Vector4 rotatedTarget = localTarget * mRotateOZ;
    target = Vector3(rotatedTarget * worldMatrix);
    updateAxes();
    updateWorldView();
}

void Camera::setDeltaTime(float dt) {
    deltaTime = dt;
}