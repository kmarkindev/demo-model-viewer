#pragma once

#include <exception>

struct InvalidParamsException : std::exception
{
	const char* what() const noexcept override
	{
		return "Invalid input params";
	}
};

struct ModelNotFoundException : std::exception
{
	std::string m_path;

	ModelNotFoundException(std::string modelPath)
		: m_path(modelPath)
	{
	}

	const char* what() const noexcept override
	{
		return ("Model not found for given path: " + m_path).c_str();
	}
};

struct ApplicationNotInitializedException : std::exception
{
	const char* what() const noexcept override
	{
		return "Application is not initialized. Call Application::Init() to initialize it";
	}
};

struct ApplicationInitializationException : std::exception
{
	std::string m_message;

	ApplicationInitializationException(std::string message)
		: m_message(message)
	{
	}

	const char* what() const noexcept override
	{
		return m_message.c_str();
	}
};

struct ShaderCompileException : std::exception
{
	std::string m_message;

	ShaderCompileException(std::string message)
		:m_message(message)
	{
	}

	const char* what() const noexcept override
	{
		return ("Shader compile error: " + m_message).c_str();
	}
};

struct ProgramLinkException : std::exception
{
	std::string m_message;

	ProgramLinkException(std::string message)
		:m_message(message)
	{
	}

	const char* what() const noexcept override
	{
		return ("Program link error: " + m_message).c_str();
	}
};

struct ShaderNotLoadedException : std::exception
{
	const char* what() const noexcept override
	{
		return "Cannot use shader because it's not loaded. Call Shader::Load(...)";
	}
};

struct NotImplementedException : std::exception
{
	const char* what() const noexcept override
	{
		return "Not implemented exception";
	}
};

struct CannotLoadModel : std::exception
{
	std::string m_path;

	CannotLoadModel(std::string path)
		: m_path(path)
	{
	}

	const char* what() const noexcept override
	{
		return ("Cannot load given modal from path: " + m_path).c_str();
	}
};