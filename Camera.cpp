#include "Camera.h"

Camera::Camera() {
	reset();
}

Camera::~Camera() {
}

void Camera::reset() {
	orientLookAt(glm::vec3(0.0f, 0.0f, DEFAULT_FOCUS_LENGTH), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	setViewAngle(VIEW_ANGLE);
	setNearPlane(NEAR_PLANE);
	setFarPlane(FAR_PLANE);
	screenWidth = screenHeight = 200;
	screenWidthRatio = 1.0f;
	rotU = rotV = rotW = 0;
}

//called by main.cpp as a part of the slider callback for controlling rotation
// the reason for computing the diff is to make sure that we are only incrementally rotating the camera
void Camera::setRotUVW(float u, float v, float w) {
	float diffU = u - rotU;
	float diffV = v - rotV;
	float diffW = w - rotW;
	rotateU(diffU);
	rotateV(diffV);
	rotateW(diffW);
	rotU = u;
	rotV = v;
	rotW = w;
}


void Camera::orientLookAt(glm::vec3 eyePoint, glm::vec3 lookatPoint, glm::vec3 upVec) {
	glm::vec3 lookVec = glm::normalize(lookatPoint - eyePoint);
	this->w = -1.0f * lookVec;
	this->u = glm::normalize(glm::cross(upVec, this->w));
	this->v = glm::cross(this->w, this->u);
	this->eyePoint = eyePoint;
}


void Camera::orientLookVec(glm::vec3 eyePoint, glm::vec3 lookVec, glm::vec3 upVec) {
	lookVec = glm::normalize(lookVec);
	this->w = -1.0f * lookVec;
	this->u = glm::normalize(glm::cross(upVec, this->w));
	this->v = glm::cross(this->w, this->u);
	this->eyePoint = eyePoint;
}

glm::mat4 Camera::getUnhingeMatrix() {
	float c = - this->nearPlane / this->farPlane;
	glm::mat4 unhingeMat4(1.0);
	unhingeMat4[2][2] = -1.0f / (c + 1.0f);
	unhingeMat4[3][2] = c / (c + 1.0f);
	unhingeMat4[2][3] = -1.0f;
	unhingeMat4[3][3] = 0.0f;
	return unhingeMat4;
}

glm::mat4 Camera::getScaleMatrix() {
	glm::mat4 scaleMat4(1.0);
	scaleMat4[0][0] = 1.0f / (glm::tan(this->viewAngle / 2) * this->screenWidthRatio * this->farPlane);
	scaleMat4[1][1] = 1.0f / (glm::tan(this->viewAngle / 2) * this->farPlane);
	scaleMat4[2][2] = 1.0f / this->farPlane;
	return scaleMat4;
}

glm::mat4 Camera::getProjectionMatrix() {
	return this->getUnhingeMatrix() * this->getScaleMatrix();
}

glm::mat4 Camera::getInverseScaleMatrix() {
	glm::mat4 invScaleMat4(1.0);
	invScaleMat4[0][0] = (glm::tan(this->viewAngle / 2) * this->screenWidthRatio * this->farPlane);
	invScaleMat4[1][1] = (glm::tan(this->viewAngle / 2) * this->farPlane);
	invScaleMat4[2][2] = this->farPlane;
	return invScaleMat4;
}

glm::mat4 Camera::getModelViewMatrix() {
	glm::mat4 modelViewMat4(1.0);
	modelViewMat4[0][0] = this->u.x;
	modelViewMat4[1][0] = this->u.y;
	modelViewMat4[2][0] = this->u.z;
	modelViewMat4[3][0] = -1.0f * glm::dot(this->u, this->eyePoint);
	modelViewMat4[0][1] = this->v.x;
	modelViewMat4[1][1] = this->v.y;
	modelViewMat4[2][1] = this->v.z;
	modelViewMat4[3][1] = -1.0f * glm::dot(this->v, this->eyePoint);
	modelViewMat4[0][2] = this->w.x;
	modelViewMat4[1][2] = this->w.y;
	modelViewMat4[2][2] = this->w.z;
	modelViewMat4[3][2] = -1.0f * glm::dot(this->w, this->eyePoint);
	return modelViewMat4;
}

glm::mat4 Camera::getInverseModelViewMatrix() {
	glm::mat4 inverseModelViewMat4(1.0);
	inverseModelViewMat4[0][0] = this->u.x;
	inverseModelViewMat4[0][1] = this->u.y;
	inverseModelViewMat4[0][2] = this->u.z;
	inverseModelViewMat4[3][0] = glm::dot(this->u, this->eyePoint);
	inverseModelViewMat4[1][0] = this->v.x;
	inverseModelViewMat4[1][1] = this->v.y;
	inverseModelViewMat4[1][2] = this->v.z;
	inverseModelViewMat4[3][1] = glm::dot(this->v, this->eyePoint);
	inverseModelViewMat4[2][0] = this->w.x;
	inverseModelViewMat4[2][1] = this->w.y;
	inverseModelViewMat4[2][2] = this->w.z;
	inverseModelViewMat4[3][2] = glm::dot(this->w, this->eyePoint);
	return inverseModelViewMat4;
}

// assuming viewAngle to be width related, so that hight will be calculated with screenWidthRatio
void Camera::setViewAngle (float _viewAngle) {
	this->viewAngle = _viewAngle * PI / 180.0f;
}

void Camera::setNearPlane (float _nearPlane) {
	this->nearPlane = _nearPlane;
}

void Camera::setFarPlane (float _farPlane) {
	this->farPlane = _farPlane;
}

void Camera::setScreenSize (int _screenWidth, int _screenHeight) {
	this->screenWidth = _screenWidth;
	this->screenHeight = _screenHeight;
	this->screenWidthRatio = this->screenWidth / this->screenHeight;
}

void Camera::rotateV(float degrees) {
	degrees = degrees * PI / 180.0f;
	// glm::mat4 rotMat(1.0);
	// rotMat[0][0] = glm::cos(degrees);
	// rotMat[2][0] = glm::sin(degrees);
	// rotMat[0][2] = -glm::sin(degrees);
	// rotMat[2][2] = glm::cos(degrees);
	// glm::mat4 mat = this->getInverseModelViewMatrix() * rotMat * this->getModelViewMatrix();
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), degrees, this->v);
	this->u = glm::vec3(mat * glm::vec4(this->u, 0.0f));
	// this->v = glm::vec3(mat * glm::vec4(this->v, 0.0f));
	this->w = glm::vec3(mat * glm::vec4(this->w, 0.0f));
}

void Camera::rotateU(float degrees) {
	degrees = degrees * PI / 180.0f;
	// glm::mat4 rotMat(1.0);
	// rotMat[1][1] = glm::cos(degrees);
	// rotMat[1][2] = -glm::sin(degrees);
	// rotMat[2][1] = glm::sin(degrees);
	// rotMat[2][2] = glm::cos(degrees);
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), degrees, this->u);
	// this->u = glm::vec3(mat * glm::vec4(this->u, 0.0f));
	this->v = glm::vec3(mat * glm::vec4(this->v, 0.0f));
	this->w = glm::vec3(mat * glm::vec4(this->w, 0.0f));
}

void Camera::rotateW(float degrees) {
	degrees = degrees * PI / 180.0f;
	// glm::mat4 rotMat(1.0);
	// rotMat[0][0] = glm::cos(degrees);
	// rotMat[1][0] = -glm::sin(degrees);
	// rotMat[0][1] = glm::sin(degrees);
	// rotMat[1][1] = glm::cos(degrees);
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), degrees, this->w);
	this->u = glm::vec3(mat * glm::vec4(this->u, 0.0f));
	this->v = glm::vec3(mat * glm::vec4(this->v, 0.0f));
	// this->w = glm::vec3(mat * glm::vec4(this->w, 0.0f));
}

void Camera::translate(glm::vec3 v) {
	this->eyePoint + v;
}

void Camera::rotate(glm::vec3 point, glm::vec3 axis, float degrees) {
	degrees = degrees * PI / 180.0f;
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), degrees, axis);
	this->u = glm::vec3(mat * glm::vec4(this->u, 0.0f));
	this->v = glm::vec3(mat * glm::vec4(this->v, 0.0f));
	this->w = glm::vec3(mat * glm::vec4(this->w, 0.0f));
}


glm::vec3 Camera::getEyePoint() {
	return this->eyePoint;
}

glm::vec3 Camera::getLookVector() {
	return -this->w;
}

glm::vec3 Camera::getUpVector() {
	return this->v;
}

float Camera::getViewAngle() {
	return viewAngle * 180 / PI;
}

float Camera::getNearPlane() {
	return nearPlane;
}

float Camera::getFarPlane() {
	return farPlane;
}

int Camera::getScreenWidth() {
	return screenWidth;
}

int Camera::getScreenHeight() {
	return screenHeight;
}

float Camera::getScreenWidthRatio() {
	return screenWidthRatio;
}
