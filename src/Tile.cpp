#include "Tile.hpp"

#include <utility>
#include "glad/gl.h"

Tile::Tile(Texture texture, std::shared_ptr<Shader> shader) : m_Texture(std::move(texture))
{
	float vertices[] = {
			// positions          // texture coords
			0.5f,  0.5f, 0.0f,    1.0f, 1.0f, // top right
			0.5f, -0.5f, 0.0f,    1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
	};



	m_Shader = std::move(shader);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



}

void Tile::Draw()
{
	m_Shader->Bind();
	m_Shader->SetMat4("model", transform);
	glBindTextureUnit(0, m_Texture.m_TextureID);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}