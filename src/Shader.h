#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "Exceptions.h"

class Shader
{
private:

	GLuint m_program;

	std::string m_vertexPath;
	std::string m_fragmentPath;

	std::string LoadShaderSrc(std::string path);
	GLuint CompileShader(std::string shaderSrc, GLenum type);
	GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

public:

	Shader(std::string vertexPath, std::string fragmentPath);
	void LoadAndCompile();

	void UseProgram();
	GLuint GetProgram();

	void SetFloatUniform(std::string location, GLfloat value);
	void SetIntUniform(std::string location, GLint value);
	void SetMat4Uniform(std::string location, const glm::mat4& matrix);
	void SetVec3Uniform(std::string location, const glm::vec3& vector);
	void SetVec4Uniform(std::string location, const glm::vec4& vector);
	void SetBoolUniform(std::string location, GLboolean value);

};