#include "Texture.hpp"

Texture::Texture(const ImageBuf& imageBuf)
{
	m_Image = imageBuf.copy(TypeDesc::UCHAR);
	if (m_Image.nchannels() == 3)
	{
		ImageBuf alpha;
		alpha.reset(OIIO::ImageSpec(m_Image.spec().width, m_Image.spec().height, 1, OIIO::TypeDesc::UCHAR));
		OIIO::ImageBufAlgo::zero(alpha);
		m_Image = ImageBufAlgo::channel_append(m_Image, alpha);
	}

	m_Spec = m_Image.spec();

	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

	glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTextureStorage2D(m_TextureID, 4, GL_RGBA8, m_Spec.width, m_Spec.height);
	glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Spec.width, m_Spec.height, GL_RGBA, GL_UNSIGNED_BYTE, m_Image.localpixels());
	glGenerateTextureMipmap(m_TextureID);

}
