#pragma once

#include <cstdint>
#include "Texture.hpp"
#include "Shader.hpp"

class Tile
{
public:
	Tile(Texture texture, std::shared_ptr<Shader> shader);
	Tile() = default;
	void Draw();

	glm::vec2 arrayPos {};

	uint32_t VBO{}, VAO{}, EBO{};
	Texture m_Texture;
	std::shared_ptr<Shader> m_Shader;
	glm::mat4 transform = glm::mat4(1.0f);
};
