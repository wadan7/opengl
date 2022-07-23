#pragma once
#include <string>
#include <algorithm>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string shaderNames, std::initializer_list<std::string> paths);

	void Bind();
	void UnBind();
	int GetUniformLocation(const std::string& uniformName);
	void SetUniform(std::string uniformName, float e1, float e2, float e3, float e4);
	~Shader();
	

private:
	unsigned int m_RenderID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
	
};