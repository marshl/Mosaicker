#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <Windows.h>
#include <WindowsX.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <string>

#include <Magick++.h>

class Texture
{
public:
    Texture( GLenum TextureTarget, const std::string& FileName );
    ~Texture();

    bool Load();

    void Bind( GLenum TextureUnit );

    unsigned int width();
    unsigned int height();

    Magick::Color GetAverageColour();
    Magick::Color GetAverageInArea( size_t x, size_t y, size_t width, size_t height );

    std::string GetFilename() const;

    GLuint GetTexture() const;

private:
    Magick::Image* m_pImage;
    Magick::Blob m_blob;
    std::string m_filename;
    Magick::Color averageColour;

    GLuint m_textureObj;
    GLenum m_textureTarget;
};

#endif