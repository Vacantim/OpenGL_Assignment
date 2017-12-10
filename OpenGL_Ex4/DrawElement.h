#pragma once

#include <gl/glut.h>   
#include <Windows.h> 
#include <iostream>

using namespace std;

//绘制单位立方体
void DrawCube(GLfloat xP, GLfloat yP, GLfloat zP, GLfloat xS, GLfloat yS, GLfloat zS, GLuint tex);

void DrawCube(GLfloat xP, GLfloat yP, GLfloat zP, GLfloat xS, GLfloat yS, GLfloat zS);

void Draw(void);

