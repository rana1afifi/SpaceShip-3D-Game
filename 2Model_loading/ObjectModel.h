#pragma once
#include "GameHeader.h"


class ObjectModel
{
	char ObjectName[100]; 
	char TextureName[100];
	std::vector<glm::vec3> Vertices;
	std::vector<glm::vec2> Uvs;
	std::vector<glm::vec3> Normals;

	GLuint TextureIndex; 
	GLuint VertexBuffer;
	GLuint UVBuffer;

	glm::mat4 ModelMatrix; 


public:
	ObjectModel(char  Objname[], char texName[]); // construct object 
	~ObjectModel();
	
	bool Create(); 

	GLuint getVertexBuffer(); 
	GLuint getUVBuffer(); 
	GLuint getTexture(); 
	std::vector<glm::vec3> getVerticesVector();
	std::vector<glm::vec2> getUVsVector();
	std::vector<glm::vec3> getNormalsVector();

	bool Draw(GLuint programID, GLuint MatrixID, GLuint vertexPosition_modelspaceID, GLuint vertexUVID , GLuint TextureID);
	bool setModelMatrix(glm::mat4 model); 
	bool setTexture(char texName[]);



};

