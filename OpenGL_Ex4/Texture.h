#pragma once

#include <gl/glut.h>   
#include <Windows.h> 
#include <iostream>

using namespace std;

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����

// ����power_of_two�����ж�һ�������ǲ���2����������  
int power_of_two(int n);

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name);
