#pragma once

#include <string>
#include <exception>

struct CustomException : public std::exception
{
	const virtual std::string GetMessage() const noexcept
	{
		return "No message specified";
	}

	const char* what() const noexcept override
	{
		return GetMessage().c_str();
	}
};

struct InvalidParamsException : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Invalid input params";
	}
};

struct ModelNotFoundException : public CustomException
{
	std::string m_path;

	ModelNotFoundException(std::string modelPath)
		: m_path(modelPath)
	{
	}

	const std::string GetMessage() const noexcept override
	{
		return "Model not found for given path: " + m_path;
	}
};

struct ApplicationNotInitializedException : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Application is not initialized. Call Application::Init() to initialize it";
	}
};

struct ApplicationInitializationException : public CustomException
{
	std::string m_message;

	ApplicationInitializationException(std::string message)
		: m_message(message)
	{
	}

	const std::string GetMessage() const noexcept override
	{
		return m_message;
	}
};

struct ShaderCompileException : public CustomException
{
	std::string m_message;

	ShaderCompileException(std::string message)
		:m_message(message)
	{
	}

	const std::string GetMessage() const noexcept override
	{
		return "Shader compile error: " + m_message;
	}
};

struct ProgramLinkException : public CustomException
{
	std::string m_message;

	ProgramLinkException(std::string message)
		:m_message(message)
	{
	}

	const std::string GetMessage() const noexcept override
	{
		return "Program link error: " + m_message;
	}
};

struct ShaderNotLoadedException : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Cannot use shader because it's not loaded. Call Shader::Load(...)";
	}
};

struct NotImplementedException : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Not implemented exception";
	}
};

struct CannotLoadModel : public CustomException
{
	std::string m_path;

	CannotLoadModel(std::string path)
		: m_path(path)
	{
	}

	const std::string GetMessage() const noexcept override
	{
		return "Cannot load given modal from path: " + m_path;
	}
};

struct CannotLoadTexture : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Cannot load texture exception";
	}
};

struct CannotConvertException : public CustomException
{
	const std::string GetMessage() const noexcept override
	{
		return "Cannot convert exception";
	}
};