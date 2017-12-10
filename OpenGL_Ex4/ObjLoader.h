#pragma once

#include <glm/glm.hpp>  
#include <vector>  
#include <string>  
#include <cstring>  
#include <iostream>  
#include <Windows.h> 

using namespace std;

class ObjLoader
{
public:
	ObjLoader();
	virtual ~ObjLoader();
	bool loadOBJ(const char * path, 
				 std::vector<glm::vec3> &out_vertices, 
				 std::vector<glm::vec2> &out_uvs, 
				 std::vector<glm::vec3> &out_normals);
protected:
private:
};