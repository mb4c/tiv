#pragma once

#include <glad/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID{};

	Shader(const std::string& vertexCode, const std::string& fragmentCode);

	void Bind();
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetUInt(const std::string &name, unsigned int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetVec3(const std::string &name, glm::vec3 value) const;
	void SetVec4(const std::string &name, glm::vec4 value) const;
	void SetMat4(const std::string &name, glm::mat4 value) const;
	void SetMat3(const std::string &name, glm::mat3 value) const;
	static void CheckCompileErrors(unsigned int shader, const std::string& type);
};
