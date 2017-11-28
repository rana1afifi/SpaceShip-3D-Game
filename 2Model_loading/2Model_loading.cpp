// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Model Loading" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );
	glfwSetMousePos(1024/2, 768/2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");

	// Load the texture
	GLuint Texture = loadBMP_custom("MarsMap.bmp");
	GLuint Texture2 = loadBMP_custom("Planetn.bmp");
	GLuint Texture3 = loadBMP_custom("mercury.bmp");
	GLuint Texture4 = loadBMP_custom("venus.bmp");
	GLuint Texture5 = loadBMP_custom("Planett.bmp");
	GLuint Texture01 = loadBMP_custom("mercury.bmp");
	GLuint Texture02 = loadBMP_custom("astroid.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	bool res = loadOBJ("Planet.obj", vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<glm::vec3> vertices2;
	std::vector<glm::vec2> uvs2;
	std::vector<glm::vec3> normals2; // Won't be used at the moment.
	bool res2 = loadOBJ("meteor.obj", vertices2, uvs2, normals2);

	// Load it into a VBO

	GLuint vertexbuffer2;
	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

	GLuint uvbuffer2;
	glGenBuffers(1, &uvbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, uvs2.size() * sizeof(glm::vec2), &uvs2[0], GL_STATIC_DRAW);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<glm::vec3> vertices3;
	std::vector<glm::vec2> uvs3;
	std::vector<glm::vec3> normals3; // Won't be used at the moment.
	bool res3 = loadOBJ("astroid.obj", vertices3, uvs3, normals3);

	// Load it into a VBO

	GLuint vertexbuffer3;
	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(glm::vec3), &vertices3[0], GL_STATIC_DRAW);

	GLuint uvbuffer3;
	glGenBuffers(1, &uvbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
	glBufferData(GL_ARRAY_BUFFER, uvs3.size() * sizeof(glm::vec2), &uvs3[0], GL_STATIC_DRAW);




	do{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
		);
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		//Object 2
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix2 = getProjectionMatrix();
		glm::mat4 ViewMatrix2 = getViewMatrix();
		glm::mat4 ModelMatrix2 = translate(mat4(), vec3(2.0f, 2.0f, 0.0f));
		glm::mat4 MVP2 = ProjectionMatrix2 * ViewMatrix2 * ModelMatrix2;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Texture2);
		glUniform1i(TextureID, 1);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//object 3
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix3 = getProjectionMatrix();
		glm::mat4 ViewMatrix3 = getViewMatrix();
		glm::mat4 ModelMatrix3 = translate(mat4(), vec3(-2.0f, 2.0f, 0.0f));
		glm::mat4 MVP3 = ProjectionMatrix3 * ViewMatrix3 * ModelMatrix3;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Texture3);
		glUniform1i(TextureID, 3);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());


		//Object 4
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix4 = getProjectionMatrix();
		glm::mat4 ViewMatrix4 = getViewMatrix();
		glm::mat4 ModelMatrix4 = translate(mat4(), vec3(2.0f, -2.0f, 0.0f));
		glm::mat4 MVP4 = ProjectionMatrix4 * ViewMatrix4 * ModelMatrix4;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, Texture4);
		glUniform1i(TextureID, 4);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		//object 5
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix5 = getProjectionMatrix();
		glm::mat4 ViewMatrix5 = getViewMatrix();
		glm::mat4 ModelMatrix5 = translate(mat4(), vec3(-2.0f, -2.0f, 0.0f));
		glm::mat4 MVP5 = ProjectionMatrix5 * ViewMatrix5 * ModelMatrix5;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP5[0][0]);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, Texture5);
		glUniform1i(TextureID, 5);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());



		///////////meteors
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix01 = getProjectionMatrix();
		glm::mat4 ViewMatrix01 = getViewMatrix();
		glm::mat4 ModelMatrix01 = translate(mat4(), vec3(1.0f,-5.0f, -3.0f));
		glm::mat4 MVP01 = ProjectionMatrix01 * ViewMatrix01 * ModelMatrix01;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP01[0][0]);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, Texture01);
		glUniform1i(TextureID, 7);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices2.size());





		///////////astroids
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix02 = getProjectionMatrix();
		glm::mat4 ViewMatrix02 = getViewMatrix();
		glm::mat4 ModelMatrix02 = translate(mat4(), vec3(0.0f, 0.0f, -3.0f));
		glm::mat4 MVP02 = ProjectionMatrix02 * ViewMatrix02 * ModelMatrix02;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP02[0][0]);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, Texture02);
		glUniform1i(TextureID, 8);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
		glVertexAttribPointer(
			vertexPosition_modelspaceID,  // The attribute we want to configure
			3,                            // size
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
		glVertexAttribPointer(
			vertexUVID,                   // The attribute we want to configure
			2,                            // size : U+V => 2
			GL_FLOAT,                     // type
			GL_FALSE,                     // normalized?
			0,                            // stride
			(void*)0                      // array buffer offset
			);
		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices3.size());








		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);

		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);

	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &uvbuffer2);

	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

