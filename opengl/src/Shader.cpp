#include "Shader.h"
#include "glad/glad.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iostream>


static std::string ReadFile(const std::string& path)
{
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fileStream.open(path, std::ios::binary); 
		std::stringstream buffer;
		buffer << fileStream.rdbuf();
		fileStream.close();
		return buffer.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ :: " << path << std::endl;
	}
}

static unsigned int CreateShader(GLenum shaderType,const std::string shaderSource)
{
	unsigned int shader = glCreateShader(shaderType);
	const char* src = &shaderSource[0];
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		int msgLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &msgLength);
		char* msg = (char*) _alloca(msgLength);
		glGetShaderInfoLog(shader, msgLength, &msgLength, msg);
		std::cout << "Failed To Compile Shader " << shaderType << " :: " << msg << '\n';
	}
	return shader;
}
Shader::Shader(const std::string shaderNames, std::initializer_list<std::string> paths) 
{
	m_RenderID = 0;
	unsigned int shadersCount = 0;
	std::unordered_map<std::string, std::string> shaderSources;
	std::vector<std::string> shaderPaths({ paths });
	std::stringstream ss;
	ss << shaderNames;
	std::string shaderName;
	std::vector<std::string>::iterator p_ShaderPaths = shaderPaths.begin();
	std::getline(ss, shaderName, '#');
	while (std::getline(ss, shaderName, '#'))
	{
		shaderSources.insert({ shaderName, ReadFile(*p_ShaderPaths++) });
		shadersCount++;
	}

	m_RenderID = glCreateProgram();
	std::vector<GLuint> shaders(shadersCount);
	std::vector<GLuint>::iterator i_Shaders = shaders.begin();
	for (auto&[shName, shSource] : shaderSources)
	{
		GLenum shaderType;
		if (shName.substr(0, 4) == "vert")
			shaderType = GL_VERTEX_SHADER;
		else if (shName.substr(0, 4) == "frag")
			shaderType = GL_FRAGMENT_SHADER;

		unsigned int sh = CreateShader(shaderType, shSource);
		*i_Shaders++ = sh;
		glAttachShader(m_RenderID, sh);
	}
	glLinkProgram(m_RenderID);
	int linked;
	glGetProgramiv(m_RenderID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		int msgLength;
		glGetProgramiv(m_RenderID, GL_INFO_LOG_LENGTH, &msgLength);
		char* msg = (char*)_alloca(msgLength);
		glGetProgramInfoLog(m_RenderID, msgLength, &msgLength, msg);
		std::cout << "Failed To Link Shaders :: " << msg << '\n';
	}
	else
	{
		for (GLuint sh : shaders)
		{
			glDetachShader(m_RenderID, sh);
			glDeleteShader(sh);
		}
	}
	
}


void Shader::Bind()
{
	glUseProgram(m_RenderID);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& uniformName)
{;
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[uniformName];
	}
	int location = glGetUniformLocation(m_RenderID, uniformName.c_str());
	m_UniformLocationCache.insert({ uniformName, location }); 
	return location;
}

void Shader::SetUniform(std::string uniformName, float e1, float e2, float e3, float e4)
{
	Bind();
	int location = GetUniformLocation(uniformName);
	glUniform4f(location, e1, e2, e3, e4);
}

Shader::~Shader()
{

}
