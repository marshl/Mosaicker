#include "Texture.h"

#include <cstdlib>
#include <iostream>

Texture::Texture( GLenum textureTarget, const std::string& filename )
{
    this->m_filename = filename;
    this->m_textureTarget = textureTarget;
}

Texture::~Texture()
{
    delete this->m_pImage;
}

bool Texture::Load()
{
    try
    {
        this->m_pImage = new Magick::Image();
        //char buffer[MAX_PATH];
        //strcpy_s( buffer, MAX_PATH, this->m_filename.c_str() );
        this->m_pImage->read( this->m_filename.c_str() );
        this->m_pImage->write( &m_blob, "RGBA" );
        glGenTextures( 1, &m_textureObj );
        glBindTexture( this->m_textureTarget, this->m_textureObj );
        glTexImage2D( m_textureTarget, 0, GL_RGBA, m_pImage->columns(), m_pImage->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data() );

        glTexParameterf( m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameterf( m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        Magick::Image copy(*this->m_pImage );
        Magick::Geometry geometry( 1, 1 );
        copy.resize( geometry );
        this->averageColour = copy.pixelColor( 0, 0 );
        
        return true;
    }
    catch ( Magick::Error& Error )
    {
        std::cout << "Error loading texture '" << this->m_filename << "': " << Error.what() << std::endl;
        return false;
    }

    
}

void Texture::Bind( GLenum TextureUnit )
{
    //glActiveTexture( TextureUnit );
    glBindTexture( m_textureTarget, m_textureObj );
}

GLuint Texture::GetTexture() const
{
    return this->m_textureObj;
}

unsigned int Texture::width()
{
    return this->m_pImage->columns();
}

unsigned int Texture::height()
{
    return this->m_pImage->rows();
}


Magick::Color Texture::GetAverageColour()
{
    return this->averageColour;
}

Magick::Color Texture::GetAverageInArea(size_t x, size_t y, size_t width, size_t height)
{
    const Magick::Geometry geometry( width, height, x, y );
    Magick::Image copy( *this->m_pImage, geometry );
    copy.resize( geometry );
    return copy.pixelColor( 0, 0 );
}

std::string Texture::GetFilename() const
{
    return this->m_filename;
}
