#include "Shader.h"

std::string Shader::LoadShaderSrc(std::string path)
{
	std::ifstream stream;
	std::stringstream ss;

	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	stream.open(path);
	ss << stream.rdbuf();
	stream.close();

	return ss.str();
}

GLuint Shader::CompileShader(std::string shaderSrc, GLenum type)
{
	GLuint shader = glCreateShader(type);
	auto nativeSrc = shaderSrc.c_str();

	glShaderSource(shader, 1, &nativeSrc, nullptr);
	glCompileShader(shader);

	int success;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char message[1024];
		glGetProgramInfoLog(shader, 1024, nullptr, message);
		throw ShaderCompileException(message);
	}

	return shader;
}

GLuint Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	char message[1024];

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		char message[1024];
		glGetProgramInfoLog(program, 1024, nullptr, message);
		throw ProgramLinkException(message);
	}

	return program;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
	: m_vertexPath(vertexPath), m_fragmentPath(fragmentPath)
{
}

void Shader::LoadAndCompile()
{
	std::string vertexSrc = LoadShaderSrc(m_vertexPath);
	std::string fragmentSrc = LoadShaderSrc(m_fragmentPath);

	GLuint vertexShader = CompileShader(vertexSrc, GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentSrc, GL_FRAGMENT_SHADER);

	m_program = LinkProgram(vertexShader, fragmentShader);
}

void Shader::UseProgram()
{
	glUseProgram(m_program);
}

GLuint Shader::GetProgram()
{
	return m_program;
}

void Shader::SetFloatUniform(std::string location, GLfloat value)
{
	glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
}

void Shader::SetIntUniform(std::string location, GLint value)
{
	glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
}

void Shader::SetMat4Uniform(std::string location, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_program, location.c_str()), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetVec3Uniform(std::string location, const glm::vec3& vector)
{
	glUniform3fv(glGetUniformLocation(m_program, location.c_str()), 1, &vector[0]);
}

void Shader::SetVec4Uniform(std::string location, const glm::vec4& vector)
{
	glUniform4fv(glGetUniformLocation(m_program, location.c_str()), 1, &vector[0]);
}

void Shader::SetBoolUniform(std::string location, GLboolean value)
{
	glUniform1i(glGetUniformLocation(m_program, location.c_str()), (GLint)value);
}
