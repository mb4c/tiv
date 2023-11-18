#pragma once

#include <cstdint>
#include <string>
#include <OpenImageIO/imagebuf.h>
#include <OpenImageIO/imagebufalgo.h>
#include <glad/gl.h>

using namespace OIIO;

class Texture
{
public:
	explicit Texture(const ImageBuf& imageBuf);
	Texture() = default;

	uint32_t m_TextureID{};
	ImageBuf m_Image;
	ImageSpec m_Spec;

};
