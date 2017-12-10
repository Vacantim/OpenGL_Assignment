#pragma once

#include <gl/glut.h>   
#include <Windows.h> 
#include <iostream>

using namespace std;

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

// 函数power_of_two用于判断一个整数是不是2的整数次幂  
int power_of_two(int n);

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name);
